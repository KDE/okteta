/***************************************************************************
                          kbuffercursor.cpp  -  description
                             -------------------
    begin                : Don Mai 29 2003
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
#include <kdebug.h>

// app specific
#include "kbufferlayout.h"
#include "kbuffercursor.h"

using namespace KHE;

KBufferCursor::KBufferCursor( const KBufferLayout *L )
 : Layout( L ),
   Index( 0 ),
   Coord( 0, 0 ),
   Behind( false ),
   AppendPosEnabled( false )
{
}

KBufferCursor::~KBufferCursor()
{
}


bool KBufferCursor::operator==( const KBufferCursor &C ) const
{
  return Index == C.Index && Behind == C.Behind ;
}


void KBufferCursor::setAppendPosEnabled( bool APE )
{
  AppendPosEnabled = APE;
  // reposition Cursor
  int Length = Layout->length();
  if( realIndex() >= Length && Coord.pos() < Layout->noOfBytesPerLine()-1 && Length > 0 )
  {
    if( AppendPosEnabled )
    {
      ++Index;
      Coord.goRight();
      Behind = false;
    }
    else
    {
      --Index;
      Coord.goLeft();
      Behind = true;
    }
  }
}


void KBufferCursor::gotoPreviousByte()
{
  if( Behind )
    Behind = false;
  else if( Index > 0 )
  {
    --Index;
    Coord.goCLeft( Layout->noOfBytesPerLine()-1 );
  }
}


void KBufferCursor::gotoPreviousByte( int D )
{
  if( Behind )
  {
    --D;
    Behind = false;
  }
  if( D > Index )
  {
    if( Index == 0 )
      return;
    gotoStart();
  }
  gotoIndex( Index - D );
}


void KBufferCursor::gotoNextByte()
{
  int Length = Layout->length();

  if( Index < Length )
  {
    if( Index == Length-1 )
      stepToEnd();
    else
    {
      ++Index;
      Coord.goCRight( Layout->noOfBytesPerLine()-1 );
      Behind = false;
    }
  }
}


void KBufferCursor::gotoNextByte( int D ) // TODO: think about consistency with gotoNextByte!!!
{
  if( Behind )
  {
    ++D;
    Behind = false;
  }
  // would we end behind the end?
  if( Index+D >= Layout->length() )
    gotoEnd();
  else
    gotoIndex( Index + D );
}


void KBufferCursor::gotoNextByteInLine()
{
  int Length = Layout->length();

  if( Index < Length )
  {
    if( Index == Length-1 )
      stepToEnd();
    else
    {
      ++Index;

      if( Coord.pos() < Layout->noOfBytesPerLine()-1 )
        Coord.goRight();
      else
        Behind = true;
    }
  }
}


void KBufferCursor::gotoUp()
{
  // can we even go up?
  if( Coord.isBelow(Layout->startLine()) )
  {
    Coord.goUp();
    if( Coord.isPriorInLineThan(Layout->start()) )
    {
      Index = 0;
      Coord.setPos( Layout->startPos() );
      Behind = false;
    }
    else
    {
      Index -= Layout->noOfBytesPerLine();
      if( Behind && !atLineEnd() )
      {
        ++Index;
        Coord.goRight();
        Behind = false;
      }
    }
  }
}


void KBufferCursor::gotoDown()
{
  if( Coord.isAbove(Layout->finalLine()) )
  {
    Coord.goDown();
    // behind End?
    if( Coord.isLaterInLineThan(Layout->final()) )
      gotoEnd();
    else
      Index += Layout->noOfBytesPerLine();
  }
}


void KBufferCursor::gotoLineStart()
{
  int OldIndex = Index;
  Index = Layout->indexAtLineStart( Coord.line() );
  Coord.goLeft( OldIndex-Index );
  Behind = false;
}


void KBufferCursor::gotoLineEnd()
{
  if( Index < Layout->length() )
  {
    int OldIndex = Index;
    Index = Layout->indexAtLineEnd( Coord.line() );
    Coord.goRight( Index-OldIndex );

    stepToEnd();
  }
}


void KBufferCursor::gotoStart()
{
  Index = 0;
  Coord = Layout->start();
  Behind = false;
}


void KBufferCursor::gotoEnd()
{
  int Length = Layout->length();
  if( Length > 0 )
  {
    Index = Length-1;
    Coord = Layout->final();

    stepToEnd();
  }
  else
    gotoStart();
}


void KBufferCursor::gotoCIndex( int i )
{
  if( Layout->length() > 0 )
  {
    Index = Layout->correctIndex( i );
    Coord = Layout->coordOfIndex( Index );
    Behind = i > Index;
  }
  else
    gotoStart();
}


void KBufferCursor::gotoCCoord( const KBufferCoord &C )
{
  if( Layout->length() > 0 )
  {
    Coord = Layout->correctCoord( C );
    Index = Layout->indexAtCoord( Coord );
    if( C > Coord )
      stepToEnd();
    else
      Behind = false;
  }
  else
    gotoStart();
}


void KBufferCursor::stepToEnd()
{
  if( AppendPosEnabled && (Coord.pos() < Layout->noOfBytesPerLine()-1) )
  {
    ++Index;
    Coord.goRight();
    Behind = false;
  }
  else
    Behind = true;
}


void KBufferCursor::gotoIndex( int i )
{
  Index = i;
  Coord = Layout->coordOfIndex( Index );
  Behind = false;
}


void KBufferCursor::gotoRealIndex()
{
  if( Behind )
  {
    ++Index;
    Coord = Layout->coordOfIndex( Index );
    Behind = false;
  }
}


void KBufferCursor::gotoCoord( const KBufferCoord &C )
{
  Index = Layout->indexAtCoord( C );
  Coord = C;
  Behind = false;
}


void KBufferCursor::updateCoord()
{
  Coord = Layout->coordOfIndex( Index );
}

// page down should be: one page minus one line
// -> if in the very first line page down will put the cursor on the same page into the last line
void KBufferCursor::gotoPageUp()
{
  int NoOfLinesPerPage = Layout->noOfLinesPerPage();
  int NewIndex = Index - NoOfLinesPerPage * Layout->noOfBytesPerLine();
  if( NewIndex < 0 )
    gotoStart();
  else
  {
    Index = NewIndex;
    Coord.goUp( NoOfLinesPerPage );
    if( Behind && !atLineEnd() )
    {
      ++Index;
      Coord.goRight();
      Behind = false;
    }
  }
}


void KBufferCursor::gotoPageDown()
{
  int NoOfLinesPerPage = Layout->noOfLinesPerPage();
  int NewIndex = Index + NoOfLinesPerPage * Layout->noOfBytesPerLine();
  if( NewIndex >= Layout->length() )
    gotoEnd();
  else
  {
    Index = NewIndex;
    Coord.goDown( NoOfLinesPerPage );
  }
}


int KBufferCursor::validIndex()       const { return Index < Layout->length() ? Index : -1; }
int KBufferCursor::indexAtLineStart() const { return Layout->indexAtLineStart( Coord.line() ); }
int KBufferCursor::indexAtLineEnd()   const { return Layout->indexAtLineEnd( Coord.line() ); }


bool KBufferCursor::atStart()     const { return Index == 0; }
bool KBufferCursor::atEnd()       const { return Index == Layout->length() - 1; }
bool KBufferCursor::atAppendPos() const { return realIndex() >= Layout->length(); }


bool KBufferCursor::atLineStart() const { return Layout->atLineStart( Coord ); }
bool KBufferCursor::atLineEnd()   const { return Layout->atLineEnd( Coord ); }
