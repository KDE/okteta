/***************************************************************************
                          kcontroller.cpp  -  description
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


#include "kcontroller.h"

// lib
#include "kbytearrayview.h"


namespace KHEUI {

KController::KController( KByteArrayView* view, KController *parent )
  : Parent( parent ), View( view )
{
}

bool KController::handleKeyPress( QKeyEvent *KeyEvent )
{
  return Parent ? Parent->handleKeyPress( KeyEvent ) : false;
}

}
