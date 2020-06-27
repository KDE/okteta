/*
    This file is part of the Okteta Gui library, made within the KDE community.

    SPDX-FileCopyrightText: 2008-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "mousepaster.hpp"

// lib
#include <bytearraycolumnview.hpp>
#include <bytearraytableranges.hpp>
#include <bytearraytablecursor.hpp>
#include <bytearraytablelayout.hpp>
// Qt
#include <QMouseEvent>

namespace Okteta {

MousePaster::MousePaster(AbstractByteArrayView* view, AbstractMouseController* parent)
    : AbstractMouseController(view, parent)
{
}

MousePaster::~MousePaster() = default;

bool MousePaster::handleMousePressEvent(QMouseEvent* mouseEvent)
{
    bool eventUsed = false;

    if (mouseEvent->button() == Qt::MidButton && !mView->isReadOnly()) {
        mView->pauseCursor();
        mView->finishByteEdit();

        ByteArrayTableRanges* tableRanges = mView->tableRanges();
        tableRanges->removeSelection();

        if (tableRanges->isModified()) {
            mView->updateChanged();
            mView->viewport()->setCursor(mView->isReadOnly() ? Qt::ArrowCursor : Qt::IBeamCursor);
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
    if (mouseEvent->button() == Qt::MidButton && !mView->isReadOnly()) {
        const QPoint releasePoint = mView->viewportToColumns(mouseEvent->pos());

        mView->pauseCursor();
        mView->finishByteEdit();

        mView->placeCursor(releasePoint);

        ByteArrayTableRanges* tableRanges = mView->tableRanges();
        ByteArrayTableCursor* tableCursor = mView->tableCursor();
        // replace no selection?
        if (tableRanges->hasSelection() && !tableRanges->selectionIncludes(tableCursor->index())) {
            tableRanges->removeSelection();
        }

        mView->pasteFromClipboard(QClipboard::Selection);

        // ensure selection changes to be drawn TODO: create a insert/pasteAtCursor that leaves out drawing
        mView->updateChanged();

        mView->ensureCursorVisible();
        mView->unpauseCursor();

        emit mView->cursorPositionChanged(tableCursor->realIndex());

        if (tableRanges->selectionJustStarted()) {
            tableRanges->removeSelection();
        }

        if (!mView->isOverwriteMode()) {
            emit mView->cutAvailable(tableRanges->hasSelection());
        }
        emit mView->copyAvailable(tableRanges->hasSelection());
        emit mView->hasSelectedDataChanged(tableRanges->hasSelection());

        eventUsed = true;
    }

    return eventUsed ? true : AbstractMouseController::handleMouseReleaseEvent(mouseEvent);
}

}
