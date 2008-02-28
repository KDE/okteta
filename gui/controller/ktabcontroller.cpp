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

#include "ktabcontroller.h"

// lib
#include "kvaluecolumn.h"
#include "kcharcolumn.h"
#include "kbytearrayview.h"
// Qt
#include <QtGui/QKeyEvent>


namespace KHEUI {

KTabController::KTabController( KByteArrayView* view, KController *parent )
  : KController( view, parent ),
    TabChangesFocus( false )
{
}


bool KTabController::handleKeyPress( QKeyEvent *KeyEvent )
{
  bool KeyUsed = false;

  bool ShiftPressed =  KeyEvent->modifiers() & Qt::SHIFT;

  if( KeyEvent->key() == Qt::Key_Tab )
  {
    // are we in the char column?
    if( View->cursorColumn() == KByteArrayView::CharColumnId )
    {
        // in last column we care about tab changes focus
      if( View->ValueColumn->isVisible() && (!TabChangesFocus || ShiftPressed) )
      {
        View->setCursorColumn( KByteArrayView::ValueColumnId );
        KeyUsed = true;
      }
    }
    // value column then
    else
    {
      if( View->CharColumn->isVisible() )
      {
          // in last column we care about tab changes focus
        if( View->CharColumn->isVisible() && (!TabChangesFocus || !ShiftPressed) )
        {
          View->setCursorColumn( KByteArrayView::CharColumnId );
          KeyUsed = true;
        }
      }
    }
  }

  return KeyUsed ? true : KController::handleKeyPress(KeyEvent);
}

}
