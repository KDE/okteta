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
#include "khexedit.h"
#include "kcontroller.h"


using namespace KHE;

KController::KController( KHexEdit* HE, KController *P )
  : Parent( P ), HexEdit( HE )
{
}

bool KController::handleKeyPress( QKeyEvent *KeyEvent )
{
  return Parent ? Parent->handleKeyPress( KeyEvent ) : false;
}
