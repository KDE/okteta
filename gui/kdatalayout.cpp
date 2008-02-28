/*
    This file is part of the Okteta Gui library, part of the KDE project.

    Copyright 2003 Friedrich W. H. Kossebau <kossebau@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library. If not, see <http://www.gnu.org/licenses/>.
*/

#include "kdatalayout.h"


namespace KHEUI {


KDataLayout::KDataLayout( int NoBpL, int SO, int L )
 : NoOfBytesPerLine( NoBpL ),
   StartOffset( SO ),
   Length( L )
{
  calcStart();
  calcEnd();
}


KDataLayout::~KDataLayout()
{
}


bool KDataLayout::setStartOffset( int SO )
{
  // rejecting <0
  if( SO < 0 )
    SO = 0;

  if( StartOffset == SO )
    return false;

  StartOffset = SO;

  calcStart();
  calcEnd();
  return true;
}


bool KDataLayout::setNoOfBytesPerLine( int N )
{
  // rejecting <1
  if( N < 1 )
    N = 1;

  // no changes?
  if( NoOfBytesPerLine == N )
    return false;

  NoOfBytesPerLine = N;

  calcStart();
  calcEnd();
  return true;
}


bool KDataLayout::setLength( int L )
{
  // rejecting < 0
  if( L < 0 )
    L = 0;

  // no changes?
  if( Length == L )
    return false;

  Length = L;

  calcEnd();
  return true;
}


void KDataLayout::setNoOfLinesPerPage( int N )
{
  NoOfLinesPerPage = N;
}


void KDataLayout::calcStart()
{
  ContentCoords.setStart( KCoord::fromIndex(StartOffset,NoOfBytesPerLine) );
}


void KDataLayout::calcEnd()
{
  ContentCoords.setEnd( (Length>0)?KCoord::fromIndex(Length-1+StartOffset,NoOfBytesPerLine):
                                   KCoord(-1,ContentCoords.start().line()) );
}


int KDataLayout::indexAtCLineStart( int L ) const
{
  return ( L <= ContentCoords.start().line() ) ? 0:
         ( L > ContentCoords.end().line() ) ?    Length-1:
                                                 L * NoOfBytesPerLine - StartOffset;
}


int KDataLayout::indexAtCLineEnd( int L ) const
{
  return ( L < ContentCoords.start().line() ) ? 0:
         ( L >= ContentCoords.end().line() ) ?  Length-1:
                                                (L+1)*NoOfBytesPerLine-StartOffset-1;
}


int KDataLayout::indexAtCCoord( const KCoord &C ) const
{
  int Index = indexAtCoord( C );

  return ( Index <= 0 ) ?      0:
         ( Index >= Length ) ? Length-1:
                               Index;
}


int KDataLayout::lineAtCIndex( int Index ) const
{
  return ( Index <= 0 ) ?      ContentCoords.start().line():
         ( Index >= Length ) ? ContentCoords.end().line():
                               lineAtIndex(Index);
}


KCoord KDataLayout::coordOfCIndex( int Index ) const
{
  return ( Index <= 0 ) ?      ContentCoords.start():
         ( Index >= Length ) ? ContentCoords.end():
                               coordOfIndex(Index);
}


int KDataLayout::indexAtLineStart( int L ) const
{
  return ( L == ContentCoords.start().line() ) ? 0 : L*NoOfBytesPerLine-StartOffset;
}


int KDataLayout::indexAtLineEnd( int L ) const
{
  return ( L == ContentCoords.end().line() ) ? Length-1 : (L+1)*NoOfBytesPerLine-StartOffset-1;
}


int KDataLayout::indexAtCoord( const KCoord &C ) const
{
  return C.indexByLineWidth( NoOfBytesPerLine ) - StartOffset;
}

int KDataLayout::lineAtIndex( int Index ) const
{
  return (Index+StartOffset)/NoOfBytesPerLine;
}

KCoord KDataLayout::coordOfIndex( int Index ) const
{
  return KCoord::fromIndex( Index+StartOffset, NoOfBytesPerLine );
}

KCoordRange KDataLayout::coordRangeOfIndizes( const KHE::KSection &Indizes ) const
{
  return KCoordRange(
           KCoord::fromIndex(Indizes.start()+StartOffset, NoOfBytesPerLine),
           KCoord::fromIndex(Indizes.end()+StartOffset,   NoOfBytesPerLine) );
}



int KDataLayout::correctIndex( int I ) const
{
  return ( I <= 0 ) ?      0:
         ( I >= Length ) ? Length-1:
                           I;
}


KCoord KDataLayout::correctCoord( const KCoord &C ) const
{
  return ( C <= ContentCoords.start() ) ?  ContentCoords.start():
         ( C >= ContentCoords.end() ) ?    ContentCoords.end():
         ( C.pos() >= NoOfBytesPerLine ) ? KCoord( NoOfBytesPerLine-1, C.line() ):
                                           C;
}


bool KDataLayout::atLineStart( const KCoord &C ) const
{
  return ( C.line() == ContentCoords.start().line() ) ? C.pos() == ContentCoords.start().pos():
                                                        C.pos() == 0;
}

bool KDataLayout::atLineEnd( const KCoord &C ) const
{
  return ( C.line() == ContentCoords.end().line() ) ? C.pos() == ContentCoords.end().pos():
                                                      C.pos() == NoOfBytesPerLine-1;
}


KHE::KSection KDataLayout::positions( int Line ) const
{
  return KHE::KSection( firstPos(Line), lastPos(Line) );
}


int KDataLayout::firstPos( const KCoord &C ) const
{
  return ( ContentCoords.start().isLaterInLineThan(C) ) ? ContentCoords.start().pos() : C.pos();
}

int KDataLayout::lastPos( const KCoord &C ) const
{
  return ( ContentCoords.end().isPriorInLineThan(C) ) ? ContentCoords.end().pos() : C.pos();
}

int KDataLayout::firstPos( int Line ) const
{
  return Line == ContentCoords.start().line() ? ContentCoords.start().pos() : 0;
}

int KDataLayout::lastPos( int Line ) const
{
  return ( Line == ContentCoords.end().line() ) ? ContentCoords.end().pos() : NoOfBytesPerLine-1;
}

bool KDataLayout::hasContent( int Line ) const
{
  return ContentCoords.includesLine( Line );
}

}
