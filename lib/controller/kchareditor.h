/***************************************************************************
                          kchareditor.h  -  description
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


#ifndef KHE_KCHAREDITOR_H
#define KHE_KCHAREDITOR_H

// lib specific
#include "keditor.h"

namespace KHE
{

class KCharColumn;

class KCharEditor : public KEditor
{
  public:
    KCharEditor( KCharColumn *CC, KBufferCursor *BC, KHexEdit *HE, KController *P );

  public: // KEditor API
    virtual bool handleKeyPress( QKeyEvent *KeyEvent );

  protected:
    KCharColumn *CharColumn;
};

}

#endif
