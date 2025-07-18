/*
    This file is part of the Okteta Gui library, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "dropper.hpp"

// lib
#include <bytearraytableranges.hpp>
#include <bytearraytablecursor.hpp>
#include <bytearraytablelayout.hpp>
#include <abstractbytearrayview.hpp>
#include <abstractbytearrayview_p.hpp>
// Okteta core
#include <Okteta/AbstractByteArrayModel>
// Qt
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDragLeaveEvent>
#include <QDropEvent>
#include <QMimeData>

namespace Okteta {

static constexpr char DropperOctetStreamFormatName[] = "application/octet-stream";

bool Dropper::handleDragEnterEvent(QDragEnterEvent* dragEnterEvent)
{
    bool eventUsed = false;

    if (!mByteArrayView->isEffectiveReadOnly()
        && mByteArrayView->canInsertBytesFromMimeData(dragEnterEvent->mimeData())) {
        mIsActive = true;
        // TODO: store value edit data
        const ByteArrayTableCursor* const tableCursor = mByteArrayView->tableCursor();
        // TODO: behind state should not be controllable, add cursorData for (re)storingdragEnterEvent
        mBeforeDragCursorPos = tableCursor->index();
        mBeforeDragCursorIsBehind = tableCursor->isBehind();
        mCursorIsMovedByDrag = false;

        eventUsed = true;
    }

    return eventUsed;
}

bool Dropper::handleDragMoveEvent(QDragMoveEvent* dragMoveEvent)
{
    bool eventUsed = false;

    if (!mByteArrayView->isEffectiveReadOnly()
        && mByteArrayView->canInsertBytesFromMimeData(dragMoveEvent->mimeData())) {
        mCursorIsMovedByDrag = true;

        // let text cursor follow mouse
        mByteArrayView->pauseCursor();
        // TODO: just for following skip the value edit, remember we are and get back
        mByteArrayView->finishByteEditor();
        mByteArrayView->placeCursor(dragMoveEvent->pos());
        mByteArrayView->unpauseCursor();

        eventUsed = true;
    }

    return eventUsed;
}

bool Dropper::handleDragLeaveEvent(QDragLeaveEvent* dragLeaveEvent)
{
    Q_UNUSED(dragLeaveEvent)

    const bool eventUsed = true;
    // bye... and thanks for all the cursor movement...
    mIsActive = false;
    if (mCursorIsMovedByDrag) {
        mByteArrayView->pauseCursor();
        // TODO: get back to value edit mode if we were in
        ByteArrayTableCursor* const tableCursor = mByteArrayView->tableCursor();
        tableCursor->gotoIndex(mBeforeDragCursorPos);
        if (mBeforeDragCursorIsBehind) {
            tableCursor->stepBehind();
        }
        mByteArrayView->unpauseCursor();
    }

    return eventUsed;
}

bool Dropper::handleDropEvent(QDropEvent* dropEvent)
{
    bool eventUsed = false;

    if (!mByteArrayView->isEffectiveReadOnly()
        && mByteArrayView->canInsertBytesFromMimeData(dropEvent->mimeData())) {
        // leave state
        mIsActive = false;

        // is this an internal dnd?
        auto* sourceByteArrayView = qobject_cast<AbstractByteArrayView*>(dropEvent->source());
        if (sourceByteArrayView
            && sourceByteArrayView->byteArrayModel() == mByteArrayView->byteArrayModel()) {
            handleInternalDrag(dropEvent, sourceByteArrayView->d_func());
        } else {
            // mByteArrayView->tableRanges()->removeSelection();
            mByteArrayView->insertBytesFromMimeData(dropEvent->mimeData());
        }
    }

    return eventUsed;
}

void Dropper::handleInternalDrag(QDropEvent* dropEvent, AbstractByteArrayViewPrivate* sourceByteArrayView)
{
    // get drag origin
    AddressRange selection = sourceByteArrayView->tableRanges()->removeSelection();

    ByteArrayTableCursor* const tableCursor = mByteArrayView->tableCursor();
    AbstractByteArrayModel* byteArrayModel = mByteArrayView->byteArrayModel();

    Address insertIndex = tableCursor->realIndex();

    // is this a move?
    if (dropEvent->proposedAction() == Qt::MoveAction) {
        // ignore the copy hold in the event but only move
        Address newCursorIndex;
        // need to swap?
        if (selection.end() < insertIndex) {
            newCursorIndex = insertIndex;
            const Address firstIndex = selection.start();
            selection.set(selection.nextBehindEnd(), insertIndex - 1);
            insertIndex = firstIndex;
        } else {
            newCursorIndex = insertIndex + selection.width();
        }

        const bool success = byteArrayModel->swapBytes(insertIndex, selection);
        if (success) {
            tableCursor->gotoCIndex(newCursorIndex);
            mByteArrayView->emitCursorPositionChanged();
        }
    }
    // is a copy
    else {
        // TODO: should this be a method of AbstractByteArrayModel, to reuse piece data?

        // get data
        const QByteArray data =
            dropEvent->mimeData()->data(QLatin1String(DropperOctetStreamFormatName));

        if (!data.isEmpty()) {
            if (mByteArrayView->isOverwriteMode()) {
                const Size length = mByteArrayView->tableLayout()->length();
                if (!tableCursor->isBehind() && length > 0) {
                    AddressRange overwriteRange = AddressRange::fromWidth(insertIndex, data.size());
                    overwriteRange.restrictEndTo(length - 1);
                    if (overwriteRange.isValid()) {
                        std::ignore = byteArrayModel->replaceBytes(overwriteRange, reinterpret_cast<const Byte*>(data.constData()), overwriteRange.width());
                    }
                }
            } else {
                std::ignore = byteArrayModel->insertBytes(insertIndex, reinterpret_cast<const Byte*>(data.constData()), data.size());
            }
        }
    }
}

}
