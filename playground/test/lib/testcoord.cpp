/***************************************************************************
                          testcoord.cpp  -  description
                            -------------------
    begin                : Mit Apr 21 2004
    copyright            : (C) 2004 by Friedrich W. H. Kossebau
    email                : Friedrich.W.H@Kossebau.de
***************************************************************************/

/***************************************************************************
*                                                                         *
*   This library is free software; you can redistribute it and/or         *
*   modify it under the terms of the GNU Library General Public           *
*   License version 2 as published by the Free Software Foundation.       *
*                                                                         *
***************************************************************************/


// kde specific
#include <kde/test/slottest.h>
#include <kde/test/testrunner.h>
// app specific
#include "testcoord.h"

// namespaces
using namespace KDE::Test;
using namespace KHE;

// local variables
static const int Line = 3;
static const int Pos = 15;
static const int Index = 43;
static const int LineWidth = 19;
static const int MaxPos = LineWidth-1;
static const int Steps = 5;


CoordTest::CoordTest() 
 :  SlotTest( "CoordTest" ) 
{
}

void CoordTest::setUp() 
{
  Coord.set( Pos, Line );
}

void CoordTest::tearDown() 
{
}



//---------------------------------------------------------------------------- Tests -----

void CoordTest::testCoordCopyEquals() 
{
  KBufferCoord C1( Coord );
  KT_ASSERT( "Test ==", C1 == Coord );    
  KBufferCoord C2;
  KT_ASSERT( "Test !=", C2 != C1 );    
  KT_ASSERT( "isAtStart()", C2.isAtStart() );
  C2 = KBufferCoord( C1.pos(), C1.line() );
  KT_ASSERT( "!isAtStart()", !C2.isAtStart() );
  KBufferCoord C3;
  C3 = C1;
  KT_ASSERT( "operator=", C1 == C3 );
}


void CoordTest::testCoordSet() 
{
  KT_ASSERT_EQUALS( "pos()", Coord.pos(), Pos );
  KT_ASSERT_EQUALS( "line()", Coord.line(), Line );

  // setX
  KBufferCoord C1;
  C1.setPos( Pos );
  KT_ASSERT_EQUALS( "setPos()", C1.pos(), Pos );    
  C1.setLine( Line );
  KT_ASSERT_EQUALS( "setLine()", C1.line(), Line );    
  
  // set
  KBufferCoord C2;
  C2.set( Pos, Line );
  KT_ASSERT( "set()", C2 == C1  );
}


void CoordTest::testCoordGoto() 
{
  KBufferCoord C2 = Coord;
  C2.gotoStart();
  KT_ASSERT( "gotoStart()", C2.isAtStart() );
 
  // goX
  C2 = Coord;
  C2.goRight(); C2.goDown(); C2.goLeft(); C2.goUp();
  KT_ASSERT( "going clockwise by one", C2 == Coord ); 
  C2.goUp(); C2.goLeft(); C2.goDown(); C2.goRight();
  KT_ASSERT( "going against clockwise by one", C2 == Coord ); 
  
  // goX( n )
  C2.goRight( Steps ); C2.goDown( Steps ); C2.goLeft( Steps ); C2.goUp( Steps );
  KT_ASSERT( "going clockwise by 5" , C2 == Coord ); 
  C2.goUp( Steps ); C2.goLeft( Steps ); C2.goDown( Steps ); C2.goRight( Steps );
  KT_ASSERT( "going against clockwise by 5", C2 == Coord );   
  
  // goControlled
  KBufferCoord C1( MaxPos, Coord.line() );
  C2 = C1;
  C1.gotoStartOfNextLine();
  C2.goCRight( MaxPos );
  KT_ASSERT( "gotoStartOfNextLine(), goCRight()", C1 == C2 );
  C1.gotoEndOfPreviousLine( MaxPos );
  C2.goCLeft( MaxPos );
  KT_ASSERT( "gotoEndOfPreviousLine(), goCLeft()", C1 == C2 ); 
  
  // goLimited
  C1 = C2 = Coord;
  C2.goLineStart( C1 );
  KT_ASSERT( "goLineStart(), same line", C1 == C2 );
  C2.goUp();
  C2.goLineStart( C1 );
  KT_ASSERT( "goLineStart(), another line", C2.pos() == 0 );
  C2.goLineEnd( MaxPos, C1 );
  KT_ASSERT( "goLineEnd(), another line", C2.pos() == MaxPos );
  C2.goDown();
  C2.goLineEnd( MaxPos, C1 );
  KT_ASSERT( "goLineEnd(), same line", C1 == C2 );
}

void CoordTest::testCoordCompare() 
{
  KBufferCoord C1 = Coord;
  KBufferCoord C2( C1.pos()+1, C1.line() );
  KT_ASSERT( "operator>()", C2>C1 );
  
  // isBelow, isAbove
  C2.setLine( C1.line() );
  C2.goDown();
  KT_ASSERT( "isBelow()", C2.isBelow(C1.line()) );
  KT_ASSERT( "!isAbove()", !C2.isAbove(C1.line()) );
  C2.goUp(2);
  KT_ASSERT( "isAbove()", C2.isAbove(C1.line()) );
  KT_ASSERT( "!isBelow()", !C2.isBelow(C1.line()) );
  C2.goDown();
  KT_ASSERT( "!isBelow() && !isAbove()", !C2.isBelow(C1.line()) && !C2.isAbove(C1.line()) );

  // isLaterInLineThan, isPriorInLineThan
  C2 = C1;
  C2.goRight();
  KT_ASSERT( "isLaterInLineThan()", C2.isLaterInLineThan(C1) );
  KT_ASSERT( "!isPriorInLineThan()", !C2.isPriorInLineThan(C1) );
  C2.goLeft(2);
  KT_ASSERT( "isPriorInLineThan()", C2.isPriorInLineThan(C1) );
  KT_ASSERT( "!isLaterInLineThan()", !C2.isLaterInLineThan(C1) );
  C2.goRight();
  KT_ASSERT( "!isLaterInLineThan() && !isPriorInLineThan()", 
             !C2.isLaterInLineThan(C1) && !C2.isPriorInLineThan(C1) );
             
  // isBehindLineStart
  C2.setPos( 1 );
  KT_ASSERT( "isBehindLineStart()", C2.isBehindLineStart() );
  C2.goLeft();
  KT_ASSERT( "!isBehindLineStart()", !C2.isBehindLineStart() );
  
  // isBeforeLineEnd
  C2.setPos( MaxPos-1 );
  KT_ASSERT( "isBeforeLineEnd()", C2.isBeforeLineEnd(MaxPos) );
  C2.goRight();
  KT_ASSERT( "!isBeforeLineEnd()", !C2.isBeforeLineEnd(MaxPos) );
}


void CoordTest::testCoordByIndex() 
{
  KBufferCoord C2;
  C2.setByIndexNWidth( Index, LineWidth );
  KT_ASSERT_EQUALS( "indexByLineWidth()" , C2.indexByLineWidth(LineWidth), Index ); 
}


int main( int, char** )
{
  CoordTest t;
  return TestRunner( &t ).run();
}

#include "testcoord.moc"
