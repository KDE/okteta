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
#include "kdataranges.h"
#include "kdatacursor.h"
#include "kbytearrayview.h"
// lib
#include <kwordbufferservice.h>
// Qt
#include <QtGui/QKeyEvent>


namespace KHEUI {

KNavigator::KNavigator( KByteArrayView* view, KController *parent )
  : KController( view, parent )
{
}

bool KNavigator::handleKeyPress( QKeyEvent *KeyEvent )
{
  bool KeyUsed = true;

  //bool clearUndoRedoInfo = true;
  bool ShiftPressed =  KeyEvent->modifiers() & Qt::SHIFT;
  bool ControlPressed = KeyEvent->modifiers() & Qt::CTRL;
  //bool AltPressed = KeyEvent->modifiers() & Qt::ALT;

  // we only care for cursor keys and the like, won't hardcode any other keys
  // we also don't check whether the commands are allowed
  // as the commands are also available as API so the check has to be done
  // in each command anyway
  switch( KeyEvent->key() )
  {
    case Qt::Key_Left:
      moveCursor( ControlPressed ? MoveWordBackward : MoveBackward, ShiftPressed );
      break;
    case Qt::Key_Right:
      moveCursor( ControlPressed ? MoveWordForward : MoveForward, ShiftPressed );
      break;
    case Qt::Key_Up:
      moveCursor( ControlPressed ? MovePgUp : MoveUp, ShiftPressed );
      break;
    case Qt::Key_Down:
      moveCursor( ControlPressed ? MovePgDown : MoveDown, ShiftPressed );
      break;
    case Qt::Key_Home:
      moveCursor( ControlPressed ? MoveHome : MoveLineStart, ShiftPressed );
      break;
    case Qt::Key_End:
      moveCursor( ControlPressed ? MoveEnd : MoveLineEnd, ShiftPressed );
      break;
    case Qt::Key_PageUp:
      moveCursor( MovePgUp, ShiftPressed );
      break;
    case Qt::Key_PageDown:
      moveCursor( MovePgDown, ShiftPressed );
      break;

    default:
      KeyUsed = false;
  }

  return KeyUsed ? true : KController::handleKeyPress(KeyEvent);
}


void KNavigator::moveCursor( KMoveAction Action, bool Select )
{
  View->pauseCursor( true );

  KDataCursor *BufferCursor = View->BufferCursor;
  KDataRanges *BufferRanges = View->BufferRanges;

  if( Select )
  {
    if( !BufferRanges->selectionStarted() )
      BufferRanges->setSelectionStart( BufferCursor->realIndex() );
  }
  else
    BufferRanges->removeSelection();

  View->resetInputContext();
  switch( Action )
  {
    case MoveBackward:     BufferCursor->gotoPreviousByte(); break;
    case MoveWordBackward: {
      KHECore::KWordBufferService WBS( View->ByteArrayModel, View->Codec );
      int NewIndex = WBS.indexOfPreviousWordStart( BufferCursor->realIndex() );
      BufferCursor->gotoIndex( NewIndex );
    }
    break;
    case MoveForward:      BufferCursor->gotoNextByte();     break;
    case MoveWordForward:  {
      KHECore::KWordBufferService WBS( View->ByteArrayModel, View->Codec );
      int NewIndex = WBS.indexOfNextWordStart( BufferCursor->realIndex() );
      BufferCursor->gotoCIndex( NewIndex );
    }
    break;
    case MoveUp:           BufferCursor->gotoUp();             break;
    case MovePgUp:         BufferCursor->gotoPageUp();         break;
    case MoveDown:         BufferCursor->gotoDown();           break;
    case MovePgDown:       BufferCursor->gotoPageDown();       break;
    case MoveLineStart:    BufferCursor->gotoLineStart();      break;
    case MoveHome:         BufferCursor->gotoStart();          break;
    case MoveLineEnd:      BufferCursor->gotoLineEnd();        break;
    case MoveEnd:          BufferCursor->gotoEnd();            break;
  }

  if( Select )
    BufferRanges->setSelectionEnd( BufferCursor->realIndex() );

  if( BufferRanges->isModified() )
    View->emitSelectionSignals(); // TODO: can this be moved somewhere
  emit View->cursorPositionChanged( BufferCursor->realIndex() );
  View->updateChanged();
  View->ensureCursorVisible();

  View->unpauseCursor();
}

}
