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

#include "kdatacursor.h"

// lib
#include "kdatalayout.h"
// Okteta core
#include <arraychangemetricslist.h>


namespace KHEUI {

KDataCursor::KDataCursor( const KDataLayout *L )
 : Layout( L ),
   Index( 0 ),
   Coord( 0, 0 ),
   Behind( false ),
   AppendPosEnabled( false )
{
}

KDataCursor::~KDataCursor()
{
}


bool KDataCursor::operator==( const KDataCursor &C ) const
{
  return Index == C.Index && Behind == C.Behind ;
}


void KDataCursor::setAppendPosEnabled( bool APE )
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


void KDataCursor::gotoPreviousByte()
{
  if( Behind )
    Behind = false;
  else if( Index > 0 )
  {
    --Index;
    Coord.goCLeft( Layout->noOfBytesPerLine()-1 );
  }
}


void KDataCursor::gotoPreviousByte( int D )
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


void KDataCursor::gotoNextByte()
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


void KDataCursor::gotoNextByte( int D ) // TODO: think about consistency with gotoNextByte!!!
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


void KDataCursor::gotoNextByteInLine()
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


void KDataCursor::gotoUp()
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


void KDataCursor::gotoDown()
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


void KDataCursor::gotoLineStart()
{
  int OldIndex = Index;
  Index = Layout->indexAtLineStart( Coord.line() );
  Coord.goLeft( OldIndex-Index );
  Behind = false;
}


void KDataCursor::gotoLineEnd()
{
  if( Index < Layout->length() )
  {
    int OldIndex = Index;
    Index = Layout->indexAtLineEnd( Coord.line() );
    Coord.goRight( Index-OldIndex );

    stepToEnd();
  }
}


void KDataCursor::gotoStart()
{
  Index = 0;
  Coord = Layout->start();
  Behind = false;
}


void KDataCursor::gotoEnd()
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


void KDataCursor::gotoCIndex( int i )
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


void KDataCursor::gotoCCoord( const KCoord &C )
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


void KDataCursor::stepToEnd()
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


void KDataCursor::gotoIndex( int i )
{
  Index = i;
  Coord = Layout->coordOfIndex( Index );
  Behind = false;
}


void KDataCursor::gotoRealIndex()
{
  if( Behind )
  {
    ++Index;
    Coord = Layout->coordOfIndex( Index );
    Behind = false;
  }
}


void KDataCursor::gotoCoord( const KCoord &C )
{
  Index = Layout->indexAtCoord( C );
  Coord = C;
  Behind = false;
}


void KDataCursor::updateCoord()
{
  Coord = Layout->coordOfIndex( Index );
}

// page down should be: one page minus one line
// -> if in the very first line page down will put the cursor on the same page into the last line
void KDataCursor::gotoPageUp()
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


void KDataCursor::gotoPageDown()
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


int KDataCursor::validIndex()       const { return Index < Layout->length() ? Index : -1; }
int KDataCursor::indexAtLineStart() const { return Layout->indexAtLineStart( Coord.line() ); }
int KDataCursor::indexAtLineEnd()   const { return Layout->indexAtLineEnd( Coord.line() ); }


bool KDataCursor::atStart()     const { return Index == 0; }
bool KDataCursor::atEnd()       const { return Index == Layout->length() - 1; }
bool KDataCursor::atAppendPos() const { return realIndex() >= Layout->length(); }


bool KDataCursor::atLineStart() const { return Layout->atLineStart( Coord ); }
bool KDataCursor::atLineEnd()   const { return Layout->atLineEnd( Coord ); }

// TODO: oldLength is a hack, as DataLayout is already updated and used by e.g. gotoCIndex
void KDataCursor::adaptToChanges( const KHE::ArrayChangeMetricsList &changeList, int oldLength )
{
    for( int i=0; i<changeList.size(); ++i )
    {
        const KHE::ArrayChangeMetrics &change = changeList[i];
        // cursor affected?
        if( Index >= change.offset() )
        {
            switch( change.type() )
            {
            case KHE::ArrayChangeMetrics::Replacement:
                oldLength += change.lengthChange();
                if( oldLength > 0 )
                {
                    // step behind removed range if inside 
                    const int newIndexAfterRemove = ( Index >= change.offset()+change.removeLength() ) ?
                                                    Index - change.removeLength() :
                                                    change.offset();
                    const int newIndex = newIndexAfterRemove + change.insertLength();
                    // if the cursor gets behind, it will never get inside again.
                    if( newIndex >= oldLength )
                    {
                        gotoEnd();
                        return;
                    }
                    Index = newIndex;
                }
                else
                    Index = 0;
            case KHE::ArrayChangeMetrics::Swapping:
                if( Index < change.secondStart() )
                {
                    Index += change.secondLength();
                }
                else if( Index <= change.secondEnd() )
                {
                    Index -= change.firstLength();
                }
            default:
                ;
            }
        }
    }

    const bool wasBehind = ( Index >= oldLength );
    if( wasBehind )
        Index = oldLength - 1;
    updateCoord();
    if( wasBehind )
        stepToEnd();
}

}
