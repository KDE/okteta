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
#include <qevent.h>
// lib specific
#include "kcharcolumn.h"
#include "kcharcodec.h"
#include "khexedit.h"
#include "kchareditor.h"


using namespace KHE;


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
      && !(KeyEvent->state()&( Qt::ControlButton | Qt::AltButton | Qt::MetaButton )) )
  {
    QChar C = KeyEvent->text()[0];
    if( C.isPrint() )
    {
      QByteArray D( 1 );
      if( CharColumn->codec()->encode(&D[0],C) )
      {
        //         clearUndoRedoInfo = false;
        HexEdit->insert( D );
        KeyUsed = true;
      }
    }
  }

  return KeyUsed ? true : KEditor::handleKeyPress(KeyEvent);
}
