/***************************************************************************
                          kchareditor.cpp  -  description
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
#include <QKeyEvent>
// commonlib specific
#include <kcharcodec.h>
// lib specific
#include "kcharcolumn.h"
#include "khexedit.h"
#include "kchareditor.h"


namespace KHEUI {


KCharEditor::KCharEditor( KCharColumn *CC, KBufferCursor *BC, KHexEdit *HE, KController *P )
  : KEditor( BC, HE, P ),
  CharColumn( CC )
{
}


bool KCharEditor::handleKeyPress( QKeyEvent *KeyEvent )
{
  bool KeyUsed = false;
  // some input that should be inserted?
  if( KeyEvent->text().length() > 0
      && !(KeyEvent->modifiers()&( Qt::CTRL | Qt::ALT | Qt::META )) )
  {
    QChar C = KeyEvent->text()[0];
    if( C.isPrint() )
    {
      QByteArray D( 1, 0 );
      if( CharColumn->codec()->encode(D.data(),C) )
      {
        //         clearUndoRedoInfo = false;
        HexEdit->insert( D );
        KeyUsed = true;
      }
    }
  }

  return KeyUsed ? true : KEditor::handleKeyPress(KeyEvent);
}

}
