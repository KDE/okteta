/***************************************************************************
                          koffsetcoltextexport.cpp  -  description
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


#include "koffsetcolumn.h" 
#include "koffsetcoltextexport.h"

using namespace KHE;


KOffsetColTextExport::KOffsetColTextExport( const KOffsetColumn *OffsetColumn )
  : CodingWidth( OffsetColumn->codingWidth() ),
    FirstLineOffset( OffsetColumn->firstLineOffset() ),
    Delta( OffsetColumn->delta() ),
    printFunction( OffsetColumn->printFunction() )
{
}

int KOffsetColTextExport::charsPerLine() const
{
  return CodingWidth;
}


void KOffsetColTextExport::printFirstLine( char **T, int Line ) const
{
  PrintLine = Line;
  print( T );
}

void KOffsetColTextExport::printNextLine( char **T ) const
{
  print( T );
}

void KOffsetColTextExport::print( char **T ) const
{
  printFunction( *T, FirstLineOffset + Delta*PrintLine );
  *T += CodingWidth;
  ++PrintLine;
}
