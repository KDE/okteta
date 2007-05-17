/***************************************************************************
                          kchareditor.h  -  description
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


#ifndef KHE_UI_KCHAREDITOR_H
#define KHE_UI_KCHAREDITOR_H


// lib
#include "keditor.h"


namespace KHEUI
{

class KCharColumn;

class KCharEditor : public KEditor
{
  public:
    virtual ~KCharEditor(){}
    KCharEditor( KCharColumn *CC, KDataCursor *BC, KByteArrayView *HE, KController *P );

  public: // KEditor API
    virtual bool handleKeyPress( QKeyEvent *KeyEvent );

  protected:
    KCharColumn *CharColumn;
};

}

#endif
