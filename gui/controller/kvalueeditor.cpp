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

#include "kvalueeditor.h"

// lib
#include "kvaluecolumn.h"
#include "kdataranges.h"
#include "kdatacursor.h"
#include "kbytearrayview.h"
// Okteta core
#include <kabstractbytearraymodel.h>
// Qt
#include <QtGui/QKeyEvent>


namespace KHEUI {

KValueEditor::KValueEditor( KValueColumn *VC, KDataCursor *BC, KByteArrayView* view, KController *parent )
  : KEditor( BC, view, parent ),
  ValueColumn( VC ),
  InEditMode( false ),
  EditModeByInsert( false )
{
}


bool KValueEditor::handleKeyPress( QKeyEvent *KeyEvent )
{
  bool KeyUsed = true;

  // TODO: for now we don't touch it if there are selections
  if( !View->BufferRanges->hasSelection() )
  {
    //
    switch( KeyEvent->key() )
    {
      case Qt::Key_Plus:
        doValueEditAction( IncValue );
        break;
      case Qt::Key_Minus:
        doValueEditAction( DecValue );
        break;
      case Qt::Key_Space:
        if( !InEditMode )
        {
          KeyUsed = false;
          break;
        }
      case Qt::Key_Enter:
      case Qt::Key_Return:
        doValueEditAction( InEditMode?LeaveValue:EnterValue );
        break;
      case Qt::Key_Escape:
        if( InEditMode )
          doValueEditAction( CancelValue );
        else
          KeyUsed = false;
        break;
      case Qt::Key_Backspace:
        if( InEditMode )
          doValueEditAction( ValueBackspace );
        else
          KeyUsed = false;
        break;
      default:
        // is plain char?
        if( KeyEvent->text().length() > 0
            && ( !(KeyEvent->modifiers()&( Qt::CTRL | Qt::ALT | Qt::META )) ) )
        {
          int Input = KeyEvent->text()[0].toAscii();
          // no usable char?
          if( Input < 32 )
          {
            KeyUsed = false;
            break;
          }

          if( InEditMode )
            doValueEditAction( ValueAppend, Input );
          else
          {
            unsigned char InputValue = 0;
            const KHECore::ValueCodec *ByteCodec = ValueColumn->byteCodec();
            // valid digit?
            if( ByteCodec->appendDigit(&InputValue,Input) )
            {
              if( View->OverWrite )
                doValueEditAction( ValueEdit, InputValue );
              else
              {
                int Index = BufferCursor->realIndex();
                if( View->ByteArrayModel->insert(Index,(char*)&InputValue,1) > 0 )
                {
                  InEditMode = true;
                  EditModeByInsert = true;
                  OldValue = EditValue = InputValue;
                  ByteCodec->encode( ByteBuffer, 0, EditValue );

                  BufferCursor->gotoRealIndex();
                  View->ensureCursorVisible();
                  View->updateCursors();
                }
              }
            }
          }
        }
        else
          KeyUsed = false;
    }
  }
  else
    KeyUsed = false;

  return KeyUsed ? true : KEditor::handleKeyPress(KeyEvent);
}


void KValueEditor::doValueEditAction( KValueEditAction Action, int Input )
{
  // we are not yet in edit mode?
  if( !InEditMode )
  {
    int ValidIndex = BufferCursor->validIndex();
    // no valid cursor position?
    if( ValidIndex == -1 || (!View->OverWrite && Input == -1) || BufferCursor->isBehind() )
      return;

    InEditMode = true;
    EditModeByInsert = false; // default, to be overwritten if so

    // save old value
    OldValue = EditValue = (unsigned char)View->ByteArrayModel->datum(ValidIndex);
  }

  const KHECore::ValueCodec *ByteCodec = ValueColumn->byteCodec();
  // 
  unsigned char NewValue = EditValue;
  bool StayInEditMode = true;
  bool MoveToNext = false;

  switch( Action )
  {
    case ValueEdit:
      NewValue = Input;
      EditValue = NewValue^255; // force update
      EditModeByInsert = true;
      break;
    case ValueBackspace:
      if( NewValue > 0 )
        ByteCodec->removeLastDigit( &NewValue );
      break;
    case EnterValue:
      EditValue ^= 255; // force update
      break;
    case IncValue:
      if( NewValue < 255 )
        ++NewValue;
      break;
    case DecValue:
      if( NewValue > 0 )
        --NewValue;
      break;
    case ValueAppend:
      if( ByteCodec->appendDigit(&NewValue,Input) )
        if( EditModeByInsert && NewValue >= ByteCodec->digitsFilledLimit() )
        {
          StayInEditMode = false;
          MoveToNext = true;
        }
      break;
    case LeaveValue:
      StayInEditMode = false;
      MoveToNext = EditModeByInsert;
      break;
    case CancelValue:
      NewValue = OldValue;
      StayInEditMode = false;
      break;
  }

  // change happened?
  if( NewValue != EditValue )
  {
    // sync value
    EditValue = NewValue;
    ByteCodec->encode( ByteBuffer, 0, EditValue );
    View->ByteArrayModel->replace( BufferCursor->index(), 1, (char*)&EditValue, 1 );
  }

  View->updateCursors();

  if( !StayInEditMode )
  {
    View->pauseCursor();
    InEditMode = false;
    if( MoveToNext )
      BufferCursor->gotoNextByte();
    View->unpauseCursor();
  }
}

}
