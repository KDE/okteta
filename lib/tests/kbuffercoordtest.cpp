/***************************************************************************
                          kbuffercoordtest.cpp  -  description
                            -------------------
    begin                : Son Mai 7 2006
    copyright            : (C) 2006 by Friedrich W. H. Kossebau
    email                : Friedrich.W.H@Kossebau.de
***************************************************************************/

/***************************************************************************
*                                                                         *
*   This library is free software; you can redistribute it and/or         *
*   modify it under the terms of the GNU Library General Public           *
*   License version 2 as published by the Free Software Foundation.       *
*                                                                         *
***************************************************************************/


// qt specific
#include <QtTest>
// test specific
#include <kbuffercoord.h>
#include "kbuffercoordtest.h"

using namespace KHE;

// local variables
static const int Line = 3;
static const int Pos = 15;
static const int Index = 43;
static const int LineWidth = 19;
static const int MaxPos = LineWidth-1;
static const int Steps = 5;


void KBufferCoordTest::testConstructor()
{
  KBufferCoord Coord;

  QCOMPARE( Coord.pos(), 0 );
  QCOMPARE( Coord.line(), 0 );
}


void KBufferCoordTest::testSetConstructor()
{
  KBufferCoord Coord( Pos, Line );

  QCOMPARE( Coord.pos(), Pos );
  QCOMPARE( Coord.line(), Line );
}


void KBufferCoordTest::testCopyConstructor()
{
  KBufferCoord Coord( Pos, Line );
  KBufferCoord OtherCoord( Coord );

  QCOMPARE( OtherCoord.pos(), Pos );
  QCOMPARE( OtherCoord.line(), Line );
}


void KBufferCoordTest::testEquals()
{
  KBufferCoord Coord( Pos, Line );

  KBufferCoord OtherCoord( Coord );
  QVERIFY( OtherCoord == Coord );
  OtherCoord.goRight();
  QVERIFY( OtherCoord != Coord );
}


void KBufferCoordTest::testAssign()
{
  KBufferCoord Coord( Pos, Line );

  KBufferCoord OtherCoord;
  OtherCoord = Coord;
  QCOMPARE( OtherCoord, Coord );
}


void KBufferCoordTest::testSetPosLine()
{
  KBufferCoord Coord;
  // setPos()
  Coord.setPos( Pos );
  QCOMPARE( Coord.pos(), Pos );
  // setLine()
  Coord.setLine( Line );
  QCOMPARE( Coord.line(), Line );
}


void KBufferCoordTest::testSet()
{
  KBufferCoord Coord;
  Coord.set( Pos, Line );
  QCOMPARE( Coord.pos(), Pos );
  QCOMPARE( Coord.line(), Line );
}


void KBufferCoordTest::testIsAtStart()
{
  KBufferCoord Coord(0,0);

  QVERIFY( Coord.isAtStart() );
}


void KBufferCoordTest::testGotoStart()
{
  KBufferCoord Coord;
  Coord.gotoStart();
  QVERIFY( Coord.isAtStart() );
}


void KBufferCoordTest::testGo()
{
  KBufferCoord Coord( Pos, Line );
  KBufferCoord OtherCoord( Coord );
  // going clockwise by one
  OtherCoord.goRight(); OtherCoord.goDown(); OtherCoord.goLeft(); OtherCoord.goUp();
  QCOMPARE( OtherCoord, Coord );
  // going against clockwise by one
  OtherCoord.goUp(); OtherCoord.goLeft(); OtherCoord.goDown(); OtherCoord.goRight();
  QCOMPARE( OtherCoord, Coord );
}


void KBufferCoordTest::testGoN()
{
  KBufferCoord Coord( Pos, Line );
  KBufferCoord OtherCoord( Coord );
  // going clockwise by N
  OtherCoord.goRight( Steps ); OtherCoord.goDown( Steps );
  OtherCoord.goLeft( Steps ); OtherCoord.goUp( Steps );
  QCOMPARE( OtherCoord, Coord );
  // going against clockwise by N
  OtherCoord.goUp( Steps ); OtherCoord.goLeft( Steps );
  OtherCoord.goDown( Steps ); OtherCoord.goRight( Steps );
  QCOMPARE( OtherCoord, Coord );
}


void KBufferCoordTest::testGotoControlled()
{
  KBufferCoord Coord( MaxPos, Line );
  KBufferCoord OtherCoord( Coord );

  // gotoStartOfNextLine(), goCRight()
  Coord.gotoStartOfNextLine();
  OtherCoord.goCRight( MaxPos );
  QCOMPARE( Coord, OtherCoord );
  // gotoEndOfPreviousLine(), goCLeft()
  Coord.gotoEndOfPreviousLine( MaxPos );
  OtherCoord.goCLeft( MaxPos );
  QCOMPARE( Coord, OtherCoord );
}


void KBufferCoordTest::testGoLineStart()
{
  KBufferCoord Coord( Pos, Line );
  KBufferCoord OtherCoord;

  // goLineStart(), same line
  OtherCoord.setLine( Coord.line() );
  OtherCoord.goLineStart( Coord );
  QCOMPARE( OtherCoord, Coord );
  // goLineStart(), another line
  OtherCoord.goUp();
  OtherCoord.goLineStart( Coord );
  QCOMPARE( OtherCoord.pos(), 0 );
}


void KBufferCoordTest::testGoLineEnd()
{
  KBufferCoord Coord( Pos, Line );
  KBufferCoord OtherCoord;
  //goLineEnd(), same line
  OtherCoord.setLine( Coord.line() );
  OtherCoord.goLineEnd( MaxPos, Coord );
  QCOMPARE( OtherCoord, Coord );
  // goLineEnd(), another line
  OtherCoord.goDown();
  OtherCoord.goLineEnd( MaxPos, Coord );
  QCOMPARE( OtherCoord.pos(), MaxPos );
}

void KBufferCoordTest::testCompareOperator()
{
  KBufferCoord Coord( Pos, Line );
  KBufferCoord OtherCoord( Coord );

  OtherCoord.goRight();
  QVERIFY( OtherCoord>Coord );
  QVERIFY( !(OtherCoord<Coord) );
  OtherCoord.goLeft(2);
  QVERIFY( OtherCoord<Coord );
  QVERIFY( !(OtherCoord>Coord) );
  OtherCoord.goRight();
  QVERIFY( !(OtherCoord>Coord) );
  QVERIFY( !(OtherCoord<Coord) );
}


void KBufferCoordTest::testIsBelowIsAbove()
{
  KBufferCoord Coord( Pos, Line );
  KBufferCoord OtherCoord( Coord );

  OtherCoord.goDown();
  QVERIFY(  OtherCoord.isBelow(Coord.line()) );
  QVERIFY( !OtherCoord.isAbove(Coord.line()) );
  OtherCoord.goUp( 2 );
  QVERIFY(  OtherCoord.isAbove(Coord.line()) );
  QVERIFY( !OtherCoord.isBelow(Coord.line()) );
  OtherCoord.goDown();
  QVERIFY( !OtherCoord.isAbove(Coord.line()) );
  QVERIFY( !OtherCoord.isBelow(Coord.line()) );
}

void KBufferCoordTest::testIsLaterPriorInLineThan()
{
  KBufferCoord Coord( Pos, Line );
  KBufferCoord OtherCoord( Coord );

  OtherCoord.goRight();
  QVERIFY(  OtherCoord.isLaterInLineThan(Coord) );
  QVERIFY( !OtherCoord.isPriorInLineThan(Coord) );
  OtherCoord.goLeft(2);
  QVERIFY(  OtherCoord.isPriorInLineThan(Coord) );
  QVERIFY( !OtherCoord.isLaterInLineThan(Coord) );
  OtherCoord.goRight();
  QVERIFY( !OtherCoord.isPriorInLineThan(Coord) );
  QVERIFY( !OtherCoord.isLaterInLineThan(Coord) );
}

void KBufferCoordTest::testIsBeforeLineStart()
{
  KBufferCoord Coord( 1, Line );

  QVERIFY( Coord.isBehindLineStart() );
  Coord.goLeft();
  QVERIFY( !Coord.isBehindLineStart() );
}

void KBufferCoordTest::testIsBeforeLineEnd()
{
  KBufferCoord Coord( MaxPos-1, Line );

  QVERIFY( Coord.isBeforeLineEnd(MaxPos) );
  Coord.goRight();
  QVERIFY( !Coord.isBeforeLineEnd(MaxPos) );
}


void KBufferCoordTest::testIndexNWidth()
{
  KBufferCoord Coord;
  Coord.setByIndexNWidth( Index, LineWidth );
  QCOMPARE( Coord.indexByLineWidth(LineWidth), Index );
}


QTEST_MAIN( KBufferCoordTest )

#include "kbuffercoordtest.moc"
