/*
    This file is part of the Okteta Gui library, part of the KDE project.

    Copyright 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library. If not, see <http://www.gnu.org/licenses/>.
*/

#include "dropper.h"

// lib
#include <bytearraytableranges.h>
#include <bytearraytablecursor.h>
#include <bytearraytablelayout.h>
#include <abstractbytearrayview.h>
// Okteta core
#include <abstractbytearraymodel.h>
// Qt
#include <QtGui/QApplication>
#include <QtGui/QDragEnterEvent>
#include <QtGui/QDragMoveEvent>
#include <QtGui/QDragLeaveEvent>
#include <QtGui/QDropEvent>


namespace KHEUI
{

static const char OctetStreamFormatName[] = "application/octet-stream";


Dropper::Dropper( AbstractByteArrayView* view )
  : mByteArrayView( view ), mIsActive( false )
{
}

bool Dropper::isActive() const { return mIsActive; }

bool Dropper::handleDragEnterEvent( QDragEnterEvent* dragEnterEvent )
{
    bool eventUsed = false;

    if( !mByteArrayView->isReadOnly()
        && mByteArrayView->canReadData(dragEnterEvent->mimeData()) )
    {
        mIsActive = true;
        // TODO: store value edit data
        ByteArrayTableCursor* tableCursor = mByteArrayView->tableCursor();
        // TODO: behind state should not be controllable, add cursorData for (re)storingdragEnterEvent
        mBeforeDragCursorPos = tableCursor->index();
        mBeforeDragCursorIsBehind = tableCursor->isBehind();
        mCursorIsMovedByDrag = false;

        eventUsed = true;
    }

    return eventUsed;
}


bool Dropper::handleDragMoveEvent( QDragMoveEvent* dragMoveEvent )
{
    bool eventUsed = false;

    if( !mByteArrayView->isReadOnly()
        && mByteArrayView->canReadData(dragMoveEvent->mimeData()) )
    {
        mCursorIsMovedByDrag = true;

        // let text cursor follow mouse
        mByteArrayView->pauseCursor();
       //TODO: just for following skip the value edit, remember we are and get back
        mByteArrayView->finishByteEdit();
        mByteArrayView->placeCursor( dragMoveEvent->pos() );
        mByteArrayView->unpauseCursor();

        eventUsed = true;
    }

    return eventUsed;
}


bool Dropper::handleDragLeaveEvent( QDragLeaveEvent* dragLeaveEvent )
{
Q_UNUSED( dragLeaveEvent )

    const bool eventUsed = true;
    // bye... and thanks for all the cursor movement...
    mIsActive = false;
    if( mCursorIsMovedByDrag )
    {
        mByteArrayView->pauseCursor();
        // TODO: get back to value edit mode if we were in
        ByteArrayTableCursor* tableCursor = mByteArrayView->tableCursor();
        tableCursor->gotoIndex( mBeforeDragCursorPos );
        if( mBeforeDragCursorIsBehind )
            tableCursor->stepBehind();
        mByteArrayView->unpauseCursor();
    }

    return eventUsed;
}


bool Dropper::handleDropEvent( QDropEvent* dropEvent )
{
    bool eventUsed = false;

    if( !mByteArrayView->isReadOnly()
        && mByteArrayView->canReadData(dropEvent->mimeData()) )
    {
        // leave state
        mIsActive = false;

        // is this an internal dnd?
        if( dropEvent->source() == mByteArrayView )
            handleInternalDrag( dropEvent );
        else
        {
        //mByteArrayView->tableRanges()->removeSelection();
            mByteArrayView->pasteData( dropEvent->mimeData() );
        }
    }

    return eventUsed;
}


void Dropper::handleInternalDrag( QDropEvent* dropEvent )
{
    // TODO: this should 

    // get drag origin
    KHE::Section selection = mByteArrayView->tableRanges()->removeSelection();

    ByteArrayTableCursor* tableCursor = mByteArrayView->tableCursor();
    KHECore::AbstractByteArrayModel* byteArrayModel = mByteArrayView->byteArrayModel();

    int insertIndex = tableCursor->realIndex();

    // is this a move?
    if( dropEvent->proposedAction() == Qt::MoveAction )
    {
        // ignore the copy hold in the event but only move
        int newCursorIndex;
        // need to swap?
        if( selection.end() < insertIndex )
        {
            newCursorIndex = insertIndex;
            const int firstIndex = selection.start();
            selection.set( selection.nextBehindEnd(), insertIndex-1 );
            insertIndex = firstIndex;
        }
        else
            newCursorIndex = insertIndex + selection.width();

        const bool success = byteArrayModel->swap( insertIndex, selection );
        if( success )
        {
            tableCursor->gotoCIndex( newCursorIndex );
            mByteArrayView->tableRanges()->addChangedRange( KHE::Section(insertIndex,selection.end()) );
            emit mByteArrayView->cursorPositionChanged( tableCursor->realIndex() );
        }
    }
    // is a copy
    else
    {
        // TODO: should this be a method of AbstractByteArrayModel, to reuse piece data?

        // get data
        const QByteArray data = dropEvent->mimeData()->data( OctetStreamFormatName );

        if( !data.isEmpty() )
        {
            if( mByteArrayView->isOverwriteMode() )
            {
                const int length = mByteArrayView->layout()->length();
                if( !tableCursor->isBehind() && length > 0 )
                {
                    KHE::Section overwriteRange = KHE::Section::fromWidth( insertIndex, data.size() );
                    overwriteRange.restrictEndTo( length-1 );
                    if( overwriteRange.isValid() )
                        byteArrayModel->replace( overwriteRange, data.data(), overwriteRange.width() );
                }
            }
            else
                byteArrayModel->insert( insertIndex, data.data(), data.size() );
        }
    }
}

Dropper::~Dropper() {}

}
