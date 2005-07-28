/***************************************************************************
                          keditor.cpp  -  description
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



// lib specific
#include "kdatabuffer.h"
#include "kbufferranges.h"
#include "kbufferlayout.h"
#include "kbuffercursor.h"
#include "kwordbufferservice.h"
#include "khexedit.h"
#include "keditor.h"
//Added by qt3to4:
#include <QKeyEvent>


using namespace KHE;

KEditor::KEditor( KBufferCursor *BC, KHexEdit* HE, KController *P )
  : KController( HE, P ),
  BufferCursor( BC )
{
}


bool KEditor::handleKeyPress( QKeyEvent *KeyEvent )
{
  bool clearUndoRedoInfo = true;
  bool ShiftPressed =  KeyEvent->state() & Qt::ShiftModifier;
  bool ControlPressed = KeyEvent->state() & Qt::ControlModifier;
  bool AltPressed = KeyEvent->state() & Qt::AltModifier;

  bool KeyUsed = true;
  // we only care for cursor keys and the like, won't hardcode any other keys
  // we also don't check whether the commands are allowed
  // as the commands are also available as API so the check has to be done
  // in each command anyway
  switch( KeyEvent->key() )
  {
    case Qt::Key_Delete:
      if( ShiftPressed )
        HexEdit->cut();
      else if( HexEdit->BufferRanges->hasSelection() )
        HexEdit->removeSelectedData();
      else
      {
        doEditAction( ControlPressed ? WordDelete : CharDelete );
        clearUndoRedoInfo = false;
      }
      break;

    case Qt::Key_Insert:
      if( ShiftPressed )
        HexEdit->paste();
      else if( ControlPressed )
        HexEdit->copy();
      else
        HexEdit->setOverwriteMode( !HexEdit->OverWrite );
      break;

    case Qt::Key_Backspace:
      if( AltPressed )
      {
        if( ControlPressed )
          break;
        else if( ShiftPressed )
        {
//           HexEdit->redo();
          break;
        }
        else
        {
//           HexEdit->undo();
          break;
        }
      }
      else if( HexEdit->BufferRanges->hasSelection() )
      {
        HexEdit->removeSelectedData();
        break;
      }

      doEditAction( ControlPressed ? WordBackspace : CharBackspace );
      clearUndoRedoInfo = false;
      break;
    case Qt::Key_F16: // "Copy" key on Sun keyboards
      HexEdit->copy();
      break;
    case Qt::Key_F18: // "Paste" key on Sun keyboards
      HexEdit->paste();
      break;
    case Qt::Key_F20: // "Cut" key on Sun keyboards
      HexEdit->cut();
      break;

    default:
        KeyUsed = false;
  }

//   if( clearUndoRedoInfo )
//     clearUndoRedo();
//   changeIntervalTimer->start( 100, true );

  return KeyUsed ? true : KController::handleKeyPress(KeyEvent);
}



void KEditor::doEditAction( KEditAction Action )
{
  HexEdit->pauseCursor( true );

  switch( Action )
  {
    case CharDelete:
      if( !HexEdit->OverWrite )
      {
        int Index = BufferCursor->realIndex();
        if( Index < HexEdit->BufferLayout->length() )
        {
          HexEdit->removeData( KSection(Index,1,false) );
          if( Index == HexEdit->BufferLayout->length() )
            BufferCursor->gotoEnd();
        }
      }
      break;

      case WordDelete: // kills data until the start of the next word
        if( !HexEdit->OverWrite )
        {
          int Index = BufferCursor->realIndex();
          if( Index < HexEdit->BufferLayout->length() )
          {
            KWordBufferService WBS( HexEdit->DataBuffer, HexEdit->Codec );
            int End = WBS.indexOfBeforeNextWordStart( Index );
            HexEdit->removeData( KSection(Index,End) );
            if( Index == HexEdit->BufferLayout->length() )
              BufferCursor->gotoEnd();
          }
        }
        break;

    case CharBackspace:
      if( HexEdit->OverWrite )
        BufferCursor->gotoPreviousByte();
      else
      {
        int DeleteIndex = BufferCursor->realIndex() - 1;
        if( DeleteIndex >= 0 )
        {
          HexEdit->removeData( KSection(DeleteIndex,1,false) );
          if( DeleteIndex == HexEdit->BufferLayout->length() )
            BufferCursor->gotoEnd();
          else
            BufferCursor->gotoPreviousByte();
        }
      }
      break;
    case WordBackspace:
    {
      int LeftIndex = BufferCursor->realIndex() - 1;
      if( LeftIndex >= 0 )
      {
        KWordBufferService WBS( HexEdit->DataBuffer, HexEdit->Codec );
        int WordStart = WBS.indexOfPreviousWordStart( LeftIndex );
        if( !HexEdit->OverWrite )
          HexEdit->removeData( KSection(WordStart,LeftIndex) );
        if( WordStart == HexEdit->BufferLayout->length() )
          BufferCursor->gotoEnd();
        else
          BufferCursor->gotoIndex(WordStart);
      }
    }
  }

  HexEdit->repaintChanged();
  HexEdit->ensureCursorVisible();

  HexEdit->unpauseCursor();

  //emit cursorPositionChanged( BufferCursor->index() );
 // emit bufferChanged();
}

