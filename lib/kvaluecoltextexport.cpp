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


// lib specific
#include "kbufferlayout.h"
#include "kvaluecolumn.h"
#include "kvaluecoltextexport.h"
#include "helper.h"


using namespace KHE;

KValueColTextExport::KValueColTextExport( const KValueColumn* HC, const char *D, KCoordRange CR )
  : KBufferColTextExport( HC, D, CR, HC->byteCodec()->encodingWidth() ),
   ByteCodec( KByteCodec::createCodec(HC->coding()) )
{
}


KValueColTextExport::~KValueColTextExport()
{
  delete ByteCodec;
}



void KValueColTextExport::print( QString &T ) const
{
  int p = 0;
  int pEnd = NoOfBytesPerLine;
  // correct boundaries
  if( PrintLine == CoordRange.start().line() )
    p = CoordRange.start().pos();
  if( PrintLine == CoordRange.end().line() )
    pEnd = CoordRange.end().pos()+1;

  QString E;
  E.setLength( ByteCodec->encodingWidth() );
  // draw individual chars
  uint e = 0;
  for( ; p<pEnd; ++p, ++PrintData )
  {
    // get next position
    uint t = Pos[p];
    // clear spacing
    T.append( whiteSpace(t-e) );
    ByteCodec->encode( E, 0, *PrintData );
    T.append( E );
    e = t + ByteCodec->encodingWidth();
  }

  T.append( whiteSpace(NoOfCharsPerLine-e) );
  ++PrintLine;
}

