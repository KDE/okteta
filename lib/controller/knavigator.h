/***************************************************************************
                          knavigator.h  -  description
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


#ifndef KHE_KNAVIGATOR_H
#define KHE_KNAVIGATOR_H

// lib specific
#include "kcontroller.h"

namespace KHE
{

class KNavigator : public KController
{
  protected:
    enum KMoveAction { MoveBackward, MoveWordBackward, MoveForward, MoveWordForward,
                       MoveUp, MovePgUp, MoveDown, MovePgDown,
                       MoveLineStart, MoveHome, MoveLineEnd, MoveEnd };
  public:
    KNavigator( KHexEdit *HE, KController *P );

  public: // KEditor API
    virtual bool handleKeyPress( QKeyEvent *KeyEvent );

  protected:
    /** moves the cursor according to the action, handles all drawing */
    void moveCursor( KMoveAction Action, bool Select );
};

}

#endif
