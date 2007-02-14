/***************************************************************************
                          kcontroller.cpp  -  description
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



// lib specific
#include "kbytearrayview.h"
#include "kcontroller.h"


namespace KHEUI {

KController::KController( KByteArrayView* HE, KController *P )
  : Parent( P ), HexEdit( HE )
{
}

bool KController::handleKeyPress( QKeyEvent *KeyEvent )
{
  return Parent ? Parent->handleKeyPress( KeyEvent ) : false;
}

}
