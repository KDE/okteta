/***************************************************************************
                          kbordercoltextexport.cpp  -  description
                             -------------------
    begin                : Sam Aug 30 2003
    copyright            : (C) 2003 by Friedrich W. H. Kossebau
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
#include <qstring.h>
// lib specific
#include "kbordercoltextexport.h"


using namespace KHE;

static const uint BorderColumnTEWidth = 3;


int KBorderColTextExport::charsPerLine() const
{
  return BorderColumnTEWidth;
}

void KBorderColTextExport::printFirstLine( QString &T, int /*Line*/ ) const
{
  print( T );
}

void KBorderColTextExport::printNextLine( QString &T ) const
{
  print( T );
}

void KBorderColTextExport::print( QString &T ) const
{
  T.append( " | " );
}
