/***************************************************************************
                          kbufferlayout.cpp  -  description
                             -------------------
    begin                : Thu Jun 12 2003
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

 
// #include <iostream>

#include "kbufferlayout.h"

using namespace KHE;

#if 0
KBufferLayout::KBufferLayout()
 : NoOfBytesPerLine( 1 ),
   StartOffset( 0 ),
   Length( 0 )
//    Start( 0,0 ),
//    Final( 0,0 )
{
}
#endif


KBufferLayout::KBufferLayout( int NoBpL, int SO, int L )
 : NoOfBytesPerLine( NoBpL ),
   StartOffset( SO ),
   Length( L )
{
  calcStart();
  calcEnd();
}


KBufferLayout::~KBufferLayout()
{
}


bool KBufferLayout::setStartOffset( int SO )
{
  if( StartOffset == SO )
    return false;

  // accepting the value, rejecting <0
  StartOffset = SO<0?0:SO;

  calcStart();
  calcEnd();
  return true;
}


bool KBufferLayout::setNoOfBytesPerLine( int N )
{
  // no changes?
  if( NoOfBytesPerLine == N )
    return false;

  // accpting the value, rejecting 0
  NoOfBytesPerLine = N<1 ? 1 : N;

  calcStart();
  calcEnd();
  return true;
}


void KBufferLayout::setNoOfLinesPerPage( int N )
{
  NoOfLinesPerPage = N;
}


bool KBufferLayout::setLength( int L )
{
  if( Length == L )
    return false;

  // accepting the value, rejecting < 0
  Length = L<0?0:L;

  calcEnd();
  return true;
}


void KBufferLayout::calcStart()
{
  ContentCoords.setStart( KBufferCoord(StartOffset,NoOfBytesPerLine,false) );
}

void KBufferLayout::calcEnd()
{
  if( Length > 0 )
    ContentCoords.setEnd( KBufferCoord(Length-1+StartOffset,NoOfBytesPerLine,false) );
  else
    ContentCoords.setEnd( KBufferCoord() );
}


int KBufferLayout::indexAtCLineStart( int L ) const
{
  if( L <= ContentCoords.start().line() )
    return 0;
  else if( L > ContentCoords.end().line() )
    return Length-1;
  else
    return L * NoOfBytesPerLine - StartOffset;
}


int KBufferLayout::indexAtCLineEnd( int L ) const
{
  if( L < ContentCoords.start().line() )
    return 0;
  else if( L >= ContentCoords.end().line() )
    return Length-1;
  else
    return (L+1) * NoOfBytesPerLine - StartOffset - 1;
}


int KBufferLayout::indexAtCCoord( const KBufferCoord &C ) const
{
  int Index = C.indexByLineWidth( NoOfBytesPerLine ) - StartOffset;

  if( Index < 0 )
    return 0;
  else if( Index >= Length )
    return Length-1;
  else
    return Index;
}


int KBufferLayout::lineAtCIndex( int Index ) const
{
  if( Index < 0 )
    return ContentCoords.start().line();
  else if( Index >= Length )
    return ContentCoords.end().line();
  else
  return (Index+StartOffset)/NoOfBytesPerLine;
}


KBufferCoord KBufferLayout::coordOfCIndex( int Index ) const
{
  if( Index < 0 )
    return ContentCoords.start();
  else if( Index >= Length )
    return ContentCoords.end();
  else
    return KBufferCoord( Index+StartOffset, NoOfBytesPerLine, false );
}


int KBufferLayout::indexAtLineStart( int L ) const
{
  if( L == ContentCoords.start().line() )
    return 0;
  else
    return L * NoOfBytesPerLine - StartOffset;
}


int KBufferLayout::indexAtLineEnd( int L ) const
{
  if( L == ContentCoords.end().line() )
    return Length-1;
  else
    return (L+1) * NoOfBytesPerLine - StartOffset - 1;
}


int KBufferLayout::indexAtCoord( const KBufferCoord &C ) const
{
  return C.indexByLineWidth( NoOfBytesPerLine ) - StartOffset;
}

int KBufferLayout::lineAtIndex( int Index ) const
{
  return (Index+StartOffset)/NoOfBytesPerLine;
}

KBufferCoord KBufferLayout::coordOfIndex( int Index ) const
{
  return KBufferCoord( Index+StartOffset, NoOfBytesPerLine, false );
}



int KBufferLayout::correctIndex( int I ) const
{
  if( I < 0 )
    return 0;
  if( I >= Length )
    return Length-1;
  return I;
}


KBufferCoord KBufferLayout::correctCoord( const KBufferCoord &C ) const
{
  if( C < ContentCoords.start() )
    return ContentCoords.start();
  if( C > ContentCoords.end() )
    return ContentCoords.end();
  if( C.pos() >= NoOfBytesPerLine )
    return KBufferCoord( NoOfBytesPerLine-1, C.line() );
  return C;
}


bool KBufferLayout::atLineStart( const KBufferCoord &C ) const
{
  if( C.line() == ContentCoords.start().line() )
    return C.pos() == ContentCoords.start().pos();
  return C.pos() == 0;
}

bool KBufferLayout::atLineEnd( const KBufferCoord &C ) const
{
  if( C.line() == ContentCoords.end().line() )
    return C.pos() == ContentCoords.end().pos();
  return C.pos() == NoOfBytesPerLine-1;
}


KSection KBufferLayout::positions( int Line ) const
{
  return KSection( firstPos(Line), lastPos(Line) );
}


int KBufferLayout::firstPos( const KBufferCoord &C ) const
{
  return ( ContentCoords.start().isLaterInLineThan(C) ) ? ContentCoords.start().pos() : C.pos();
}

int KBufferLayout::lastPos( const KBufferCoord &C ) const
{
  return ( ContentCoords.end().isPriorInLineThan(C) ) ? ContentCoords.end().pos() : C.pos();
}

int KBufferLayout::firstPos( int Line ) const
{
  return Line == ContentCoords.start().line() ? ContentCoords.start().pos() : 0;
}

int KBufferLayout::lastPos( int Line ) const
{
  return ( Line == ContentCoords.end().line() ) ? ContentCoords.end().pos() : NoOfBytesPerLine-1;
}

bool KBufferLayout::hasContent( int Line ) const
{
  return ContentCoords.includesLine( Line );
}
