/***************************************************************************
                          keditor.cpp  -  description
                             -------------------
    begin                : Sa Dez 4 2004
    copyright            : (C) 2004 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License version 2 as published by the Free Software Foundation.       *
 *                                                                         *
 ***************************************************************************/


#include "keditor.h"

// lib
#include "kdataranges.h"
#include "kdatalayout.h"
#include "kdatacursor.h"
#include "kbytearrayview.h"
// hexedit core
#include <kabstractbytearraymodel.h>
#include <kwordbufferservice.h>
// Qt
#include <QtGui/QKeyEvent>


namespace KHEUI {

KEditor::KEditor( KDataCursor *BC, KByteArrayView* HE, KController *P )
  : KController( HE, P ),
  BufferCursor( BC )
{
}


bool KEditor::handleKeyPress( QKeyEvent *KeyEvent )
{
  bool clearUndoRedoInfo = true;
  bool ShiftPressed =  KeyEvent->modifiers() & Qt::SHIFT;
  bool ControlPressed = KeyEvent->modifiers() & Qt::CTRL;
  bool AltPressed = KeyEvent->modifiers() & Qt::ALT;

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
//   startTimer->start( 100, true );

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
        const int Index = BufferCursor->realIndex();
        if( Index < HexEdit->BufferLayout->length() )
          HexEdit->ByteArrayModel->remove( KHE::KSection::fromWidth(Index,1) );
      }
      break;

      case WordDelete: // kills data until the start of the next word
        if( !HexEdit->OverWrite )
        {
          const int Index = BufferCursor->realIndex();
          if( Index < HexEdit->BufferLayout->length() )
          {
            KHECore::KWordBufferService WBS( HexEdit->ByteArrayModel, HexEdit->Codec );
            int End = WBS.indexOfBeforeNextWordStart( Index );
            HexEdit->ByteArrayModel->remove( KHE::KSection(Index,End) );
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
          HexEdit->ByteArrayModel->remove( KHE::KSection::fromWidth(DeleteIndex,1) );
      }
      break;
    case WordBackspace:
    {
      int LeftIndex = BufferCursor->realIndex() - 1;
      if( LeftIndex >= 0 )
      {
        KHECore::KWordBufferService WBS( HexEdit->ByteArrayModel, HexEdit->Codec );
        int WordStart = WBS.indexOfPreviousWordStart( LeftIndex );
        if( !HexEdit->OverWrite )
          HexEdit->ByteArrayModel->remove( KHE::KSection(WordStart,LeftIndex) );
      }
    }
  }

  HexEdit->ensureCursorVisible();
}

}
