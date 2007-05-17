/***************************************************************************
                          kcharcoltextexport.cpp  -  description
                             -------------------
    begin                : Wed Sep 03 2003
    copyright            : (C) 2003 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License version 2 as published by the Free Software Foundation.       *
 *                                                                         *
 ***************************************************************************/


#include "kcharcoltextexport.h"

// lib
#include "kdatalayout.h"
#include "kcharcolumn.h"
#include "kcharcodec.h"


namespace KHEUI {

KCharColTextExport::KCharColTextExport( const KCharColumn* TC, const char *D, const KCoordRange &CR, const QString &CodecName )
 : KDataColTextExport( TC, D, CR, 1 ),
   CharCodec( KHECore::KCharCodec::createCodec(CodecName) ),
   SubstituteChar( TC->substituteChar() ),
   UndefinedChar( TC->undefinedChar() )
{
}


KCharColTextExport::~KCharColTextExport()
{
  delete CharCodec;
}


void KCharColTextExport::print( QString *T ) const
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
    T->append( whiteSpace(t-e) );

    // print char
    KHECore::KChar B = CharCodec->decode( *PrintData );

    T->append( B.isUndefined() ? KHECore::KChar(UndefinedChar) : !B.isPrint() ? KHECore::KChar(SubstituteChar) : B );
    e = t + 1;
  }

  T->append( whiteSpace(NoOfCharsPerLine-e) );

  ++PrintLine;
}

}
