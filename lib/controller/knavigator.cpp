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
#include <qevent.h>
// lib specific
#include "kdatabuffer.h"
#include "kbufferranges.h"
#include "kbuffercursor.h"
#include "khexedit.h"
#include "knavigator.h"


using namespace KHE;

KNavigator::KNavigator( KHexEdit* HE, KController *P )
  : KController( HE, P )
{
}

bool KNavigator::handleKeyPress( QKeyEvent *KeyEvent )
{
  bool KeyUsed = true;

  //bool clearUndoRedoInfo = true;
  bool ShiftPressed =  KeyEvent->state() & Qt::ShiftButton;
  bool ControlPressed = KeyEvent->state() & Qt::ControlButton;
  //bool AltPressed = KeyEvent->state() & AltButton;

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
    case Qt::Key_Prior:
      moveCursor( MovePgUp, ShiftPressed );
      break;
    case Qt::Key_Next:
      moveCursor( MovePgDown, ShiftPressed );
      break;

    default:
      KeyUsed = false;
  }

  if( !KeyUsed && Parent )
    KeyUsed = Parent->handleKeyPress( KeyEvent );

  return KeyUsed;
}


void KNavigator::moveCursor( KMoveAction Action, bool Select )
{
  HexEdit->pauseCursor( true );

  KBufferCursor *BufferCursor = HexEdit->BufferCursor;
  KBufferRanges *BufferRanges = HexEdit->BufferRanges;

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
      int NewIndex = BufferCursor->realIndex();
      NewIndex = HexEdit->DataBuffer->indexOfPreviousWordStart( NewIndex, KDataBuffer::Readable );
      BufferCursor->gotoIndex( NewIndex );
    }
    break;
    case MoveForward:      BufferCursor->gotoNextByte();     break;
    case MoveWordForward:  {
      int NewIndex = BufferCursor->realIndex();
      NewIndex = HexEdit->DataBuffer->indexOfNextWordStart( NewIndex, KDataBuffer::Readable );
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

  HexEdit->repaintChanged();
  HexEdit->ensureCursorVisible();

  HexEdit->unpauseCursor();
}
