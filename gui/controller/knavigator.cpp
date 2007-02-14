/***************************************************************************
                          knavigator.cpp  -  description
                             -------------------
    begin                : Sa Dez 4 2004
    copyright            : (C) 2004 by Friedrich W. H. Kossebau
    email                : Friedrich.W.H@Kossebau.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License version 2 as published by the Free Software Foundation.       *
 *                                                                         *
 ***************************************************************************/


// qt specific
#include <QtGui/QKeyEvent>
// lib specific
#include <kwordbufferservice.h>
// lib specific
#include "kdataranges.h"
#include "kdatacursor.h"
#include "kbytearrayview.h"
#include "knavigator.h"


namespace KHEUI {

KNavigator::KNavigator( KByteArrayView* HE, KController *P )
  : KController( HE, P )
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
  HexEdit->pauseCursor( true );

  KDataCursor *BufferCursor = HexEdit->BufferCursor;
  KDataRanges *BufferRanges = HexEdit->BufferRanges;

  if( Select )
  {
    if( !BufferRanges->selectionStarted() )
      BufferRanges->setSelectionStart( BufferCursor->realIndex() );
  }
  else
    BufferRanges->removeSelection();

  HexEdit->resetInputContext();
  switch( Action )
  {
    case MoveBackward:     BufferCursor->gotoPreviousByte(); break;
    case MoveWordBackward: {
      KHECore::KWordBufferService WBS( HexEdit->ByteArrayModel, HexEdit->Codec );
      int NewIndex = WBS.indexOfPreviousWordStart( BufferCursor->realIndex() );
      BufferCursor->gotoIndex( NewIndex );
    }
    break;
    case MoveForward:      BufferCursor->gotoNextByte();     break;
    case MoveWordForward:  {
      KHECore::KWordBufferService WBS( HexEdit->ByteArrayModel, HexEdit->Codec );
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
    HexEdit->emitSelectionSignals(); // TODO: can this be moved somewhere
  emit HexEdit->cursorPositionChanged( BufferCursor->realIndex() );
  HexEdit->updateChanged();
  HexEdit->ensureCursorVisible();

  HexEdit->unpauseCursor();
}

}
