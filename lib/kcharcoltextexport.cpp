/***************************************************************************
                          kcharcoltextexport.cpp  -  description
                             -------------------
    begin                : Wed Sep 03 2003
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

#include <iostream>
// c specific
#include <string.h>
// app specific
#include "kbufferlayout.h"
#include "kcharcolumn.h"
#include "kcharcoltextexport.h"

using namespace KHE;

static const unsigned char TELowestPrintableChar = 32;


KCharColTextExport::KCharColTextExport( const KCharColumn* TC, char *D, KCoordRange CR )
 : KBufferColTextExport( TC, D, CR, 1 ),
   SubstituteChar( TC->substituteChar() )
{
}


KCharColTextExport::~KCharColTextExport()
{
}


void KCharColTextExport::print( char **T ) const
{
  int p = 0;
  int pEnd = NoOfBytesPerLine;
  // correct boundaries
  if( PrintLine == CoordRange.start().line() )
    p = CoordRange.start().pos();
  if( PrintLine == CoordRange.end().line() )
    pEnd = CoordRange.end().pos()+1;

  // draw individual chars
  char *e = *T;
  for( ; p<pEnd; ++p, ++Data )
  {
    // get next position
    char *t = *T + Pos[p];
    // clear spacing
    memset( e, ' ', t-e );

    unsigned char D = *Data;
    *t = D<TELowestPrintableChar ? SubstituteChar : D;
    e = t + 1;
  }

  *T += NoOfCharsPerLine;
  memset( e, ' ', *T-e );
  ++PrintLine;
}
