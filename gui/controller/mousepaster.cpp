/*
    SPDX-FileCopyrightText: 2008-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "mousepaster.hpp"

// lib
#include <abstractbytearrayview_p.hpp>
#include <bytearraytableranges.hpp>
#include <bytearraytablecursor.hpp>
// Qt
#include <QMouseEvent>

namespace Okteta {

MousePaster::MousePaster(AbstractByteArrayViewPrivate* view, AbstractMouseController* parent)
    : AbstractMouseController(view, parent)
{
}

MousePaster::~MousePaster() = default;

bool MousePaster::handleMousePressEvent(QMouseEvent* mouseEvent)
{
    bool eventUsed = false;

    if (mouseEvent->button() == Qt::MiddleButton && !mView->isEffectiveReadOnly()) {
        mView->pauseCursor();
        mView->finishByteEditor();

        ByteArrayTableRanges* const tableRanges = mView->tableRanges();
        tableRanges->removeSelection();

        if (tableRanges->isModified()) {
            mView->updateChanged();
            mView->setMouseCursor(mView->isEffectiveReadOnly() ? Qt::ArrowCursor : Qt::IBeamCursor);
        }
        mView->unpauseCursor();

        eventUsed = true;
    }

    return eventUsed ? true : AbstractMouseController::handleMousePressEvent(mouseEvent);
}

bool MousePaster::handleMouseReleaseEvent(QMouseEvent* mouseEvent)
{
    bool eventUsed = false;
    // middle mouse button paste?
    if (mouseEvent->button() == Qt::MiddleButton && !mView->isEffectiveReadOnly()) {
        const QPoint releasePoint = mView->viewportToColumns(mouseEvent->pos());

        mView->pauseCursor();
        mView->finishByteEditor();

        mView->placeCursor(releasePoint);

        ByteArrayTableRanges* const tableRanges = mView->tableRanges();
        const ByteArrayTableCursor* const tableCursor = mView->tableCursor();
        // replace no selection?
        if (tableRanges->hasSelection() && !tableRanges->selectionIncludes(tableCursor->index())) {
            tableRanges->removeSelection();
        }

        mView->pasteFromClipboard(QClipboard::Selection);

        mView->ensureCursorVisible();

        if (tableRanges->selectionJustStarted()) {
            tableRanges->removeSelection();
        }

        // ensure selection changes to be drawn TODO: create a insert/pasteAtCursor that leaves out drawing
        mView->updateChanged();
        mView->unpauseCursor();
        mView->emitSelectionUpdates();

        eventUsed = true;
    }

    return eventUsed ? true : AbstractMouseController::handleMouseReleaseEvent(mouseEvent);
}

}
