/***************************************************************************
                          kcontroller.h  -  description
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


#ifndef KHE_KCONTROLLER_H
#define KHE_KCONTROLLER_H


class QKeyEvent;

namespace KHE
{

class KHexEdit;

class KController
{
  protected:
    KController( KHexEdit *HE, KController *P );

  public: // KController API
    virtual bool handleKeyPress( QKeyEvent *KeyEvent );

  protected:
    KController *Parent;
    KHexEdit *HexEdit;
};

}

#endif
