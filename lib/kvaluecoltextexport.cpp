/***************************************************************************
                          kvaluecoltextexport.cpp  -  description
                             -------------------
    begin                : Wed Sep 3 2003
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
// lib specific
#include "kbufferlayout.h"
#include "kvaluecolumn.h"
#include "kvaluecoltextexport.h"
#include "helper.h"


using namespace KHE;

KValueColTextExport::KValueColTextExport( const KValueColumn* HC, char *D, KCoordRange CR )
 : KBufferColTextExport( HC, D, CR, HC->codingWidth() ),
   CodingWidth( HC->codingWidth() ),
   CodingFunction( HC->codingFunction() )
{
}


KValueColTextExport::~KValueColTextExport()
{
}


// Duh, hacky, hacky, is because codingFunction writes one byte (\0) behind the end
// TODO: write the last into a buffer and copy without the \0
void KValueColTextExport::print( char **T ) const
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
    CodingFunction( t, *Data );
    e = t + CodingWidth;
  }

  *T += NoOfCharsPerLine;
  memset( e, ' ', *T-e );
  ++PrintLine;
}

