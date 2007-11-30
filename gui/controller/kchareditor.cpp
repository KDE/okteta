/***************************************************************************
                          kchareditor.cpp  -  description
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


#include "kchareditor.h"

// lib
#include "kcharcolumn.h"
#include "kbytearrayview.h"
// commonlib
#include <kcharcodec.h>
// Qt
#include <QtGui/QKeyEvent>


namespace KHEUI {


KCharEditor::KCharEditor( KCharColumn *CC, KDataCursor *BC, KByteArrayView *view, KController *parent )
  : KEditor( BC, view, parent ),
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
        View->insert( D );
        KeyUsed = true;
      }
    }
  }

  return KeyUsed ? true : KEditor::handleKeyPress(KeyEvent);
}

}
