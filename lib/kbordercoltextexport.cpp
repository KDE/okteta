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


// c specific
#include <string.h>
// app specific
#include "kbordercoltextexport.h"

using namespace KHE;

int KBorderColTextExport::charsPerLine() const
{
  return 3;
}

void KBorderColTextExport::printFirstLine( char **T, int /*Line*/ ) const
{
  print( T );
}

void KBorderColTextExport::printNextLine( char **T ) const
{
  print( T );
}

void KBorderColTextExport::print( char **T ) const
{
  memcpy( *T, " | ", 3 );
  *T += 3;
}
