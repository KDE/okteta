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


// app specific
#include "kbufferlayout.h"
#include "kbuffercursor.h"

using namespace KHE;

KBufferCursor::KBufferCursor( const KBufferLayout *L )
 : Layout( L ),
   Index( -1 ),
   Coord( 0, 0 ),
   Behind( false )
{
}

KBufferCursor::KBufferCursor( const KBufferCursor &C )
{
  Layout = C.Layout;
  Index = C.Index;
  Coord = C.Coord;
  Behind = C.Behind;
}


KBufferCursor::~KBufferCursor()
{
}

KBufferCursor &KBufferCursor::operator=( const KBufferCursor &C )
{
  Index = C.Index;
  Coord = C.Coord;
  Behind = C.Behind;

  return *this;
}


bool KBufferCursor::operator==( const KBufferCursor &C ) const
{
  return Index == C.Index && Behind == C.Behind && DigitPos == C.DigitPos;
}


void KBufferCursor::gotoPreviousByte()
{
  if( Behind )
  {
    Behind = false;
    return;
  }

  if( Index == 0 )
    return;

  --Index;

  if( !Coord.goLeft() )
    Coord.gotoEndOfPreviousLine( Layout->noOfBytesPerLine()-1 );
}


void KBufferCursor::gotoPreviousByte( int D )
{
  if( Behind )
  {
    --D;
    Behind = false;
  }
  if( Index < D )
  {
    if( Index == 0 )
      return;
    gotoStart();
  }
  gotoIndex( Index - D );
}


void KBufferCursor::gotoNextByte()
{
  if( Index == Layout->length()-1 )
  {
    Behind = true;
    return;
  }

  ++Index;

  if( !Coord.goRight(Layout->noOfBytesPerLine()-1) )
    Coord.gotoStartOfNextLine();

  if( Behind )
    Behind = false;
}


void KBufferCursor::gotoNextByte( int D ) // TODO: think about consistency with gotoNextByte!!!
{
  if( Behind )
  {
    ++D;
    Behind = false;
  }
  // would we end behind the end?
  if( Layout->length()-1 < Index+D )
    gotoEnd();
  else
    gotoIndex( Index + D );
}


void KBufferCursor::gotoNextByteInLine()
{
  if( Index == Layout->length()-1 )
  {
    Behind = true;
    return;
  }

  ++Index;

  if( !Coord.goRight(Layout->noOfBytesPerLine()-1) )
    Behind = true;
}


void KBufferCursor::gotoUp()
{
  // can we even go up?
  if( Coord.isBelow(Layout->startLine()) )
  {
    Coord.goUp();
    if( Coord.isPriorInLineThan(Layout->start()) )
    {
      Coord.setPos( Layout->startPos() );
      Index = 0;
      Behind = false;
    }
    else
    {
      Index -= Layout->noOfBytesPerLine();
      if( Behind && !atLineEnd() )
      {
        Coord.goRight();
        ++Index;
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
    {
      // set to end
      Coord.setPos( Layout->finalPos() );
      Index = Layout->length() - 1;
      Behind = true;
    }
    else
      Index += Layout->noOfBytesPerLine();
  }
}


int KBufferCursor::indexAtLineStart() const
{
  return Layout->indexAtLineStart( Coord.line() );
}


int KBufferCursor::indexAtLineEnd() const
{
  return Layout->indexAtLineEnd( Coord.line() );
}


void KBufferCursor::gotoLineStart()
{
  Index = Layout->indexAtLineStart( Coord.line() );
  Coord.goLineStart( Layout->start() );
  Behind = false;
}


void KBufferCursor::gotoLineEnd()
{
  Index = Layout->indexAtLineEnd( Coord.line() );
  Coord.goLineEnd( Layout->noOfBytesPerLine()-1, Layout->final() );
  Behind = true;
}


void KBufferCursor::gotoStart()
{
  Index = 0;
  Coord = Layout->start();
  Behind = false;
}


void KBufferCursor::gotoEnd()
{
  Index = Layout->length()-1;
  Coord = Layout->final();
  Behind = true;
}


void KBufferCursor::gotoCIndex( int i )
{
  Index = Layout->correctIndex( i );
  Coord = Layout->coordOfIndex( Index );
  Behind = i > Index;
}


void KBufferCursor::gotoCCoord( const KBufferCoord &C )
{
  Coord = Layout->correctCoord( C );
  Index = Layout->indexAtCCoord( Coord );
  Behind = C > Coord;
}


void KBufferCursor::gotoIndex( int i )
{
  Index = i;
  Coord = Layout->coordOfIndex( Index );
  Behind = false;
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
      Coord.goRight();
      ++Index;
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


bool KBufferCursor::atStart() const
{
  return Index == 0;
}

bool KBufferCursor::atEnd() const
{
  return Index == Layout->length() - 1;
}

/*inline*/ bool KBufferCursor::behindEnd() const
{
  return Index == Layout->length();
}


bool KBufferCursor::atLineStart() const
{
  return Layout->atLineStart( Coord );
}

bool KBufferCursor::atLineEnd() const
{
  return Layout->atLineEnd( Coord );
}
