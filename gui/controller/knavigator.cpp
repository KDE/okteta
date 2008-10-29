/*
    This file is part of the Okteta Gui library, part of the KDE project.

    Copyright 2004 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "knavigator.h"

// lib
#include "bytearraytableranges.h"
#include "bytearraytablecursor.h"
#include "bytearraycolumnview.h"
#include "kvalueeditor.h"
// lib
#include <kwordbufferservice.h>
// Qt
#include <QtGui/QKeyEvent>


namespace KHEUI {

KNavigator::KNavigator( ByteArrayColumnView* view, KController *parent )
  : KController( view, parent )
{
}

bool KNavigator::handleKeyPress( QKeyEvent *keyEvent )
{
    bool keyUsed = true;

    const bool shiftPressed =  keyEvent->modifiers() & Qt::SHIFT;
    const bool controlPressed = keyEvent->modifiers() & Qt::CTRL;
    //bool AltPressed = keyEvent->modifiers() & Qt::ALT;

    // we only care for cursor keys and the like, won't hardcode any other keys
    // we also don't check whether the commands are allowed
    // as the commands are also available as API so the check has to be done
    // in each command anyway
    switch( keyEvent->key() )
    {
    case Qt::Key_Left:
        moveCursor( controlPressed ? MoveWordBackward : MoveBackward, shiftPressed );
        break;
    case Qt::Key_Right:
        moveCursor( controlPressed ? MoveWordForward : MoveForward, shiftPressed );
        break;
    case Qt::Key_Up:
        moveCursor( controlPressed ? MovePgUp : MoveUp, shiftPressed );
        break;
    case Qt::Key_Down:
        moveCursor( controlPressed ? MovePgDown : MoveDown, shiftPressed );
        break;
    case Qt::Key_Home:
        moveCursor( controlPressed ? MoveHome : MoveLineStart, shiftPressed );
        break;
    case Qt::Key_End:
        moveCursor( controlPressed ? MoveEnd : MoveLineEnd, shiftPressed );
        break;
    case Qt::Key_PageUp:
        moveCursor( MovePgUp, shiftPressed );
        break;
    case Qt::Key_PageDown:
        moveCursor( MovePgDown, shiftPressed );
        break;
    default:
        keyUsed = false;
    }

    return keyUsed ? true : KController::handleKeyPress(keyEvent);
}


void KNavigator::moveCursor( KMoveAction action, bool select )
{
    mView->pauseCursor();
    mView->valueEditor()->finishEdit();

    ByteArrayTableCursor* tableCursor = mView->tableCursor();
    ByteArrayTableRanges* tableRanges = mView->tableRanges();

    if( select )
    {
        if( !tableRanges->selectionStarted() )
            tableRanges->setSelectionStart( tableCursor->realIndex() );
    }
    else
        tableRanges->removeSelection();

    switch( action )
    {
    case MoveBackward:     tableCursor->gotoPreviousByte(); break;
    case MoveWordBackward: {
            const KHECore::KWordBufferService WBS( mView->byteArrayModel(), mView->charCodec() );
            const int newIndex = WBS.indexOfPreviousWordStart( tableCursor->realIndex() );
            tableCursor->gotoIndex( newIndex );
        }
        break;
    case MoveForward:      tableCursor->gotoNextByte();     break;
    case MoveWordForward:  {
            const KHECore::KWordBufferService WBS( mView->byteArrayModel(), mView->charCodec() );
            const int newIndex = WBS.indexOfNextWordStart( tableCursor->realIndex() );
            tableCursor->gotoCIndex( newIndex );
        }
        break;
    case MoveUp:           tableCursor->gotoUp();             break;
    case MovePgUp:         tableCursor->gotoPageUp();         break;
    case MoveDown:         tableCursor->gotoDown();           break;
    case MovePgDown:       tableCursor->gotoPageDown();       break;
    case MoveLineStart:    tableCursor->gotoLineStart();      break;
    case MoveHome:         tableCursor->gotoStart();          break;
    case MoveLineEnd:      tableCursor->gotoLineEnd();        break;
    case MoveEnd:          tableCursor->gotoEnd();            break;
    }

    if( select )
        tableRanges->setSelectionEnd( tableCursor->realIndex() );

    if( tableRanges->isModified() )
        mView->emitSelectionSignals(); // TODO: can this be moved somewhere
    emit mView->cursorPositionChanged( tableCursor->realIndex() );
    mView->updateChanged();
    mView->ensureCursorVisible();

    mView->unpauseCursor();
}

}
