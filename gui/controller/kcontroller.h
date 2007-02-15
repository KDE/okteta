/***************************************************************************
                          kcontroller.h  -  description
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


#ifndef KHE_UI_KCONTROLLER_H
#define KHE_UI_KCONTROLLER_H


class QKeyEvent;

namespace KHEUI
{

class KByteArrayView;

class KController
{
  protected:
    virtual ~KController(){}
    KController( KByteArrayView *HE, KController *P );

  public: // KController API
    virtual bool handleKeyPress( QKeyEvent *KeyEvent );

  protected:
    KController *Parent;
    KByteArrayView *HexEdit;
};

}

#endif
