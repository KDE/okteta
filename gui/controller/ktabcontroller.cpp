/***************************************************************************
                          ktabcontroller.cpp  -  description
                             -------------------
    begin                : So Dez 5 2004
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


#include "ktabcontroller.h"

// lib
#include "kvaluecolumn.h"
#include "kcharcolumn.h"
#include "kbytearrayview.h"
// Qt
#include <QtGui/QKeyEvent>


namespace KHEUI {

KTabController::KTabController( KByteArrayView* HE, KController *P )
  : KController( HE, P ),
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
    if( HexEdit->cursorColumn() == KByteArrayView::CharColumnId )
    {
        // in last column we care about tab changes focus
      if( HexEdit->ValueColumn->isVisible() && (!TabChangesFocus || ShiftPressed) )
      {
        HexEdit->setCursorColumn( KByteArrayView::ValueColumnId );
        KeyUsed = true;
      }
    }
    // value column then
    else
    {
      if( HexEdit->CharColumn->isVisible() )
      {
          // in last column we care about tab changes focus
        if( HexEdit->CharColumn->isVisible() && (!TabChangesFocus || !ShiftPressed) )
        {
          HexEdit->setCursorColumn( KByteArrayView::CharColumnId );
          KeyUsed = true;
        }
      }
    }
  }

  return KeyUsed ? true : KController::handleKeyPress(KeyEvent);
}

}
