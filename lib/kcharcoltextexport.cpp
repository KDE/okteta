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


// lib specific
#include "kbufferlayout.h"
#include "kcharcolumn.h"
#include "kcharcodec.h"
#include "kcharcoltextexport.h"

using namespace KHE;


KCharColTextExport::KCharColTextExport( const KCharColumn* TC, const char *D, KCoordRange CR, const QString &CodecName )
 : KBufferColTextExport( TC, D, CR, 1 ),
   CharCodec( KCharCodec::createCodec(CodecName) ),
   SubstituteChar( TC->substituteChar() ),
   UndefinedChar( TC->undefinedChar() )
{
}


KCharColTextExport::~KCharColTextExport()
{
  delete CharCodec;
}


void KCharColTextExport::print( QString &T ) const
{
  int p = 0;
  int pEnd = NoOfBytesPerLine;
  // correct boundaries
  if( PrintLine == CoordRange.start().line() )
    p = CoordRange.start().pos();
  if( PrintLine == CoordRange.end().line() )
    pEnd = CoordRange.end().pos()+1;

  // draw individual chars
  uint e = 0;
  for( ; p<pEnd; ++p, ++PrintData )
  {
    // get next position
    uint t = Pos[p];
    // clear spacing
    T.append( whiteSpace(t-e) );

    // print char
    KHEChar B = CharCodec->decode( *PrintData );

    T.append( B.isUndefined() ? KHEChar(UndefinedChar) : !B.isPrint() ? KHEChar(SubstituteChar) : B );
    e = t + 1;
  }

  T.append( whiteSpace(NoOfCharsPerLine-e) );

  ++PrintLine;
}
