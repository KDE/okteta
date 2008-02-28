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

#include "keditor.h"

// lib
#include "kdataranges.h"
#include "kdatalayout.h"
#include "kdatacursor.h"
#include "kbytearrayview.h"
// Okteta core
#include <kabstractbytearraymodel.h>
#include <kwordbufferservice.h>
// Qt
#include <QtGui/QKeyEvent>


namespace KHEUI {

KEditor::KEditor( KDataCursor *BC, KByteArrayView* view, KController *parent )
  : KController( view, parent ),
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
        View->cut();
      else if( View->BufferRanges->hasSelection() )
        View->removeSelectedData();
      else
      {
        doEditAction( ControlPressed ? WordDelete : CharDelete );
        clearUndoRedoInfo = false;
      }
      break;

    case Qt::Key_Insert:
      if( ShiftPressed )
        View->paste();
      else if( ControlPressed )
        View->copy();
      else
        View->setOverwriteMode( !View->OverWrite );
      break;

    case Qt::Key_Backspace:
      if( AltPressed )
      {
        if( ControlPressed )
          break;
        else if( ShiftPressed )
        {
//           View->redo();
          break;
        }
        else
        {
//           View->undo();
          break;
        }
      }
      else if( View->BufferRanges->hasSelection() )
      {
        View->removeSelectedData();
        break;
      }

      doEditAction( ControlPressed ? WordBackspace : CharBackspace );
      clearUndoRedoInfo = false;
      break;
    case Qt::Key_F16: // "Copy" key on Sun keyboards
      View->copy();
      break;
    case Qt::Key_F18: // "Paste" key on Sun keyboards
      View->paste();
      break;
    case Qt::Key_F20: // "Cut" key on Sun keyboards
      View->cut();
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
  View->pauseCursor( true );

  switch( Action )
  {
    case CharDelete:
      if( !View->OverWrite )
      {
        const int Index = BufferCursor->realIndex();
        if( Index < View->BufferLayout->length() )
          View->ByteArrayModel->remove( KHE::KSection::fromWidth(Index,1) );
      }
      break;

      case WordDelete: // kills data until the start of the next word
        if( !View->OverWrite )
        {
          const int Index = BufferCursor->realIndex();
          if( Index < View->BufferLayout->length() )
          {
            KHECore::KWordBufferService WBS( View->ByteArrayModel, View->Codec );
            int End = WBS.indexOfBeforeNextWordStart( Index );
            View->ByteArrayModel->remove( KHE::KSection(Index,End) );
          }
        }
        break;

    case CharBackspace:
      if( View->OverWrite )
        BufferCursor->gotoPreviousByte();
      else
      {
        int DeleteIndex = BufferCursor->realIndex() - 1;
        if( DeleteIndex >= 0 )
          View->ByteArrayModel->remove( KHE::KSection::fromWidth(DeleteIndex,1) );
      }
      break;
    case WordBackspace:
    {
      int LeftIndex = BufferCursor->realIndex() - 1;
      if( LeftIndex >= 0 )
      {
        KHECore::KWordBufferService WBS( View->ByteArrayModel, View->Codec );
        int WordStart = WBS.indexOfPreviousWordStart( LeftIndex );
        if( !View->OverWrite )
          View->ByteArrayModel->remove( KHE::KSection(WordStart,LeftIndex) );
      }
    }
  }

  View->ensureCursorVisible();
}

}
