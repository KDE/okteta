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
// test specific
#include <kbuffercoord.h>
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


//---------------------------------------------------------------------------- Tests -----

void CoordTest::testConstructor()
{
  KBufferCoord Coord;

  KT_ASSERT_EQUALS( "pos()", Coord.pos(), 0 );
  KT_ASSERT_EQUALS( "line()", Coord.line(), 0 );
}


void CoordTest::testSetConstructor()
{
  KBufferCoord Coord( Pos, Line );

  KT_ASSERT_EQUALS( "pos()", Coord.pos(), Pos );
  KT_ASSERT_EQUALS( "line()", Coord.line(), Line );
}


void CoordTest::testCopyConstructor()
{
  KBufferCoord Coord( Pos, Line );
  KBufferCoord OtherCoord( Coord );

  KT_ASSERT_EQUALS( "pos()", OtherCoord.pos(), Pos );
  KT_ASSERT_EQUALS( "line()", OtherCoord.line(), Line );
}


void CoordTest::testEquals()
{
  KBufferCoord Coord( Pos, Line );

  KBufferCoord OtherCoord( Coord );
  KT_ASSERT_EQUALS( "Test ==", OtherCoord == Coord, true );
  OtherCoord.goRight();
  KT_ASSERT_EQUALS( "Test !=", OtherCoord != Coord, true );
}


void CoordTest::testAssign()
{
  KBufferCoord Coord( Pos, Line );

  KBufferCoord OtherCoord;
  OtherCoord = Coord;
  KT_ASSERT( "operator=", OtherCoord == Coord );
}


void CoordTest::testSetPosLine()
{
  KBufferCoord Coord;
  Coord.setPos( Pos );
  KT_ASSERT_EQUALS( "setPos()", Coord.pos(), Pos );
  Coord.setLine( Line );
  KT_ASSERT_EQUALS( "setLine()", Coord.line(), Line );
}


void CoordTest::testSet()
{
  KBufferCoord Coord;
  Coord.set( Pos, Line );
  KT_ASSERT( "set()", Coord.pos()==Pos && Coord.line()==Line );
}


void CoordTest::testIsAtStart()
{
  KBufferCoord Coord(0,0);

  KT_ASSERT( "", Coord.isAtStart() );
}


void CoordTest::testGotoStart()
{
  KBufferCoord Coord;
  Coord.gotoStart();
  KT_ASSERT( "gotoStart()", Coord.isAtStart() );
}


void CoordTest::testGo()
{
  KBufferCoord Coord( Pos, Line );
  KBufferCoord OtherCoord( Coord );
  OtherCoord.goRight(); OtherCoord.goDown(); OtherCoord.goLeft(); OtherCoord.goUp();
  KT_ASSERT( "going clockwise by one", OtherCoord == Coord );
  OtherCoord.goUp(); OtherCoord.goLeft(); OtherCoord.goDown(); OtherCoord.goRight();
  KT_ASSERT( "going against clockwise by one", OtherCoord == Coord );
}


void CoordTest::testGoN()
{
  KBufferCoord Coord( Pos, Line );
  KBufferCoord OtherCoord( Coord );
  OtherCoord.goRight( Steps ); OtherCoord.goDown( Steps );
  OtherCoord.goLeft( Steps ); OtherCoord.goUp( Steps );
  KT_ASSERT( "going clockwise by N" , OtherCoord == Coord );
  OtherCoord.goUp( Steps ); OtherCoord.goLeft( Steps );
  OtherCoord.goDown( Steps ); OtherCoord.goRight( Steps );
  KT_ASSERT( "going against clockwise by N", OtherCoord == Coord );
}


void CoordTest::testGotoControlled()
{
  KBufferCoord Coord( MaxPos, Line );
  KBufferCoord OtherCoord( Coord );

  Coord.gotoStartOfNextLine();
  OtherCoord.goCRight( MaxPos );
  KT_ASSERT( "gotoStartOfNextLine(), goCRight()", Coord == OtherCoord );
  Coord.gotoEndOfPreviousLine( MaxPos );
  OtherCoord.goCLeft( MaxPos );
  KT_ASSERT( "gotoEndOfPreviousLine(), goCLeft()", Coord == OtherCoord );
}


void CoordTest::testGoLineStart()
{
  KBufferCoord Coord( Pos, Line );
  KBufferCoord OtherCoord;

  OtherCoord.setLine( Coord.line() );
  OtherCoord.goLineStart( Coord );
  KT_ASSERT( "goLineStart(), same line", Coord == OtherCoord );
  OtherCoord.goUp();
  OtherCoord.goLineStart( Coord );
  KT_ASSERT_EQUALS( "goLineStart(), another line", OtherCoord.pos(), 0 );
}


void CoordTest::testGoLineEnd()
{
  KBufferCoord Coord( Pos, Line );
  KBufferCoord OtherCoord;

  OtherCoord.setLine( Coord.line() );
  OtherCoord.goLineEnd( MaxPos, Coord );
  KT_ASSERT( "goLineEnd(), same line", Coord == OtherCoord );
  OtherCoord.goDown();
  OtherCoord.goLineEnd( MaxPos, Coord );
  KT_ASSERT_EQUALS( "goLineEnd(), another line", OtherCoord.pos(), MaxPos );
}

void CoordTest::testCompareOperator()
{
  KBufferCoord Coord( Pos, Line );
  KBufferCoord OtherCoord( Coord );

  OtherCoord.goRight();
  KT_ASSERT( "operator>()", OtherCoord>Coord );
  KT_ASSERT( "!operator<()", !(OtherCoord<Coord) );
  OtherCoord.goLeft(2);
  KT_ASSERT( "operator<()", OtherCoord<Coord );
  KT_ASSERT( "!operator>()", !(OtherCoord>Coord) );
  OtherCoord.goRight();
  KT_ASSERT( "!operator>()", !(OtherCoord>Coord) );
  KT_ASSERT( "!operator<()", !(OtherCoord<Coord) );
}


void CoordTest::testIsBelowIsAbove()
{
  KBufferCoord Coord( Pos, Line );
  KBufferCoord OtherCoord( Coord );

  OtherCoord.goDown();
  KT_ASSERT( "isBelow()", OtherCoord.isBelow(Coord.line()) );
  KT_ASSERT( "!isAbove()", !OtherCoord.isAbove(Coord.line()) );
  OtherCoord.goUp( 2 );
  KT_ASSERT( "isAbove()", OtherCoord.isAbove(Coord.line()) );
  KT_ASSERT( "!isBelow()", !OtherCoord.isBelow(Coord.line()) );
  OtherCoord.goDown();
  KT_ASSERT( "!isAbove()", !OtherCoord.isAbove(Coord.line()) );
  KT_ASSERT( "!isBelow()", !OtherCoord.isBelow(Coord.line()) );
}

void CoordTest::testIsLaterPriorInLineThan()
{
  KBufferCoord Coord( Pos, Line );
  KBufferCoord OtherCoord( Coord );

  OtherCoord.goRight();
  KT_ASSERT( "isLaterInLineThan()", OtherCoord.isLaterInLineThan(Coord) );
  KT_ASSERT( "!isPriorInLineThan()", !OtherCoord.isPriorInLineThan(Coord) );
  OtherCoord.goLeft(2);
  KT_ASSERT( "isPriorInLineThan()", OtherCoord.isPriorInLineThan(Coord) );
  KT_ASSERT( "!isLaterInLineThan()", !OtherCoord.isLaterInLineThan(Coord) );
  OtherCoord.goRight();
  KT_ASSERT( "!isPriorInLineThan()", !OtherCoord.isPriorInLineThan(Coord) );
  KT_ASSERT( "!isLaterInLineThan()", !OtherCoord.isLaterInLineThan(Coord) );
}

void CoordTest::testIsBeforeLineStart()
{
  KBufferCoord Coord( 1, Line );

  KT_ASSERT_EQUALS( "isBehindLineStart()", Coord.isBehindLineStart(), true );
  Coord.goLeft();
  KT_ASSERT_EQUALS( "!isBehindLineStart()", Coord.isBehindLineStart(), false );
}

void CoordTest::testIsBeforeLineEnd()
{
  KBufferCoord Coord( MaxPos-1, Line );

  KT_ASSERT_EQUALS( "isBeforeLineEnd()", Coord.isBeforeLineEnd(MaxPos), true );
  Coord.goRight();
  KT_ASSERT_EQUALS( "!isBeforeLineEnd()", Coord.isBeforeLineEnd(MaxPos), false );
}


void CoordTest::testIndexNWidth()
{
  KBufferCoord Coord;
  Coord.setByIndexNWidth( Index, LineWidth );
  KT_ASSERT_EQUALS( "indexByLineWidth()", Coord.indexByLineWidth(LineWidth), Index );
}


int main( int, char** )
{
  CoordTest t;
  return TestRunner( &t ).run();
}

#include "testcoord.moc"
