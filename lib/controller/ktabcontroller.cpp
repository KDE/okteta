/***************************************************************************
                          ktabcontroller.cpp  -  description
                             -------------------
    begin                : So Dez 5 2004
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
#include "kvaluecolumn.h"
#include "kcharcolumn.h"
#include "khexedit.h"
#include "ktabcontroller.h"


using namespace KHE;

KTabController::KTabController( KHexEdit* HE, KController *P )
  : KController( HE, P ),
    TabChangesFocus( false )
{
}


bool KTabController::handleKeyPress( QKeyEvent *KeyEvent )
{
  bool KeyUsed = false;

  bool ShiftPressed =  KeyEvent->state() & Qt::ShiftButton;

  if( KeyEvent->key() == Qt::Key_Tab )
  {
    // are we in the char column?
    if( HexEdit->cursorColumn() == KHexEdit::CharColumnId )
    {
        // in last column we care about tab changes focus
      if( HexEdit->ValueColumn->isVisible() && (!TabChangesFocus || ShiftPressed) )
      {
        HexEdit->setCursorColumn( KHexEdit::ValueColumnId );
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
          HexEdit->setCursorColumn( KHexEdit::CharColumnId );
          KeyUsed = true;
        }
      }
    }
  }

  return KeyUsed ? true : KController::handleKeyPress(KeyEvent);
}
