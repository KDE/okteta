/***************************************************************************
                          kcoordtest.cpp  -  description
                            -------------------
    begin                : Son Mai 7 2006
    copyright            : (C) 2006 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
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
#include <kcoord.h>
#include "kcoordtest.h"

using namespace KHEUI;

// local variables
static const int Line = 3;
static const int Pos = 15;
static const int LineWidth = 19;
static const int Index = Line*LineWidth + Pos;
static const int MaxPos = LineWidth-1;
static const int Steps = 5;


void KCoordTest::testConstructor()
{
  KCoord Coord;

  QCOMPARE( Coord.pos(), 0 );
  QCOMPARE( Coord.line(), 0 );
  QVERIFY( Coord.isValid() );
}


void KCoordTest::testSetConstructor()
{
  KCoord Coord( Pos, Line );

  QCOMPARE( Coord.pos(), Pos );
  QCOMPARE( Coord.line(), Line );
  QVERIFY( Coord.isValid() );
}


void KCoordTest::testFromIndexConstructor()
{
  KCoord Coord = KCoord::fromIndex( Index, LineWidth );

  QCOMPARE( Coord.indexByLineWidth(LineWidth), Index );
  QCOMPARE( Coord.pos(), Pos );
  QCOMPARE( Coord.line(), Line );
  QVERIFY( Coord.isValid() );
}


void KCoordTest::testCopyConstructor()
{
  KCoord Coord( Pos, Line );
  KCoord OtherCoord( Coord );

  QCOMPARE( OtherCoord.pos(), Pos );
  QCOMPARE( OtherCoord.line(), Line );
  QVERIFY( OtherCoord.isValid() );
}


void KCoordTest::testEquals()
{
  KCoord Coord( Pos, Line );

  KCoord OtherCoord( Coord );
  QVERIFY( OtherCoord == Coord );
  OtherCoord.goRight();
  QVERIFY( OtherCoord != Coord );
}


void KCoordTest::testAssign()
{
  KCoord Coord( Pos, Line );

  KCoord OtherCoord;
  OtherCoord = Coord;
  QCOMPARE( OtherCoord, Coord );
}


void KCoordTest::testSetPosLine()
{
  KCoord Coord;
  // setPos()
  Coord.setPos( Pos );
  QCOMPARE( Coord.pos(), Pos );
  // setLine()
  Coord.setLine( Line );
  QCOMPARE( Coord.line(), Line );
}


void KCoordTest::testSet()
{
  KCoord Coord;
  Coord.set( Pos, Line );
  QCOMPARE( Coord.pos(), Pos );
  QCOMPARE( Coord.line(), Line );
}


void KCoordTest::testIsAtStart()
{
  KCoord Coord(0,0);

  QVERIFY( Coord.isAtStart() );
}


void KCoordTest::testGotoStart()
{
  KCoord Coord;
  Coord.gotoStart();
  QVERIFY( Coord.isAtStart() );
}


void KCoordTest::testGo()
{
  KCoord Coord( Pos, Line );
  KCoord OtherCoord( Coord );
  // going clockwise by one
  OtherCoord.goRight(); OtherCoord.goDown(); OtherCoord.goLeft(); OtherCoord.goUp();
  QCOMPARE( OtherCoord, Coord );
  // going against clockwise by one
  OtherCoord.goUp(); OtherCoord.goLeft(); OtherCoord.goDown(); OtherCoord.goRight();
  QCOMPARE( OtherCoord, Coord );
}


void KCoordTest::testGoN()
{
  KCoord Coord( Pos, Line );
  KCoord OtherCoord( Coord );
  // going clockwise by N
  OtherCoord.goRight( Steps ); OtherCoord.goDown( Steps );
  OtherCoord.goLeft( Steps ); OtherCoord.goUp( Steps );
  QCOMPARE( OtherCoord, Coord );
  // going against clockwise by N
  OtherCoord.goUp( Steps ); OtherCoord.goLeft( Steps );
  OtherCoord.goDown( Steps ); OtherCoord.goRight( Steps );
  QCOMPARE( OtherCoord, Coord );
}


void KCoordTest::testGotoControlled()
{
  KCoord Coord( MaxPos, Line );
  KCoord OtherCoord( Coord );

  // gotoStartOfNextLine(), goCRight()
  Coord.gotoStartOfNextLine();
  OtherCoord.goCRight( MaxPos );
  QCOMPARE( Coord, OtherCoord );
  // gotoEndOfPreviousLine(), goCLeft()
  Coord.gotoEndOfPreviousLine( MaxPos );
  OtherCoord.goCLeft( MaxPos );
  QCOMPARE( Coord, OtherCoord );
}


void KCoordTest::testGoLineStart()
{
  KCoord Coord( Pos, Line );
  KCoord OtherCoord;

  // goLineStart(), same line
  OtherCoord.setLine( Coord.line() );
  OtherCoord.goLineStart( Coord );
  QCOMPARE( OtherCoord, Coord );
  // goLineStart(), another line
  OtherCoord.goUp();
  OtherCoord.goLineStart( Coord );
  QCOMPARE( OtherCoord.pos(), 0 );
}


void KCoordTest::testGoLineEnd()
{
  KCoord Coord( Pos, Line );
  KCoord OtherCoord;
  //goLineEnd(), same line
  OtherCoord.setLine( Coord.line() );
  OtherCoord.goLineEnd( MaxPos, Coord );
  QCOMPARE( OtherCoord, Coord );
  // goLineEnd(), another line
  OtherCoord.goDown();
  OtherCoord.goLineEnd( MaxPos, Coord );
  QCOMPARE( OtherCoord.pos(), MaxPos );
}

void KCoordTest::testCompareOperator()
{
  KCoord Coord( Pos, Line );
  KCoord OtherCoord( Coord );

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


void KCoordTest::testIsBelowIsAbove()
{
  KCoord Coord( Pos, Line );
  KCoord OtherCoord( Coord );

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

void KCoordTest::testIsLaterPriorInLineThan()
{
  KCoord Coord( Pos, Line );
  KCoord OtherCoord( Coord );

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

void KCoordTest::testIsBeforeLineStart()
{
  KCoord Coord( 1, Line );

  QVERIFY( Coord.isBehindLineStart() );
  QVERIFY( Coord.isValid() );
  Coord.goLeft();
  QVERIFY( !Coord.isBehindLineStart() );
  QVERIFY( Coord.isValid() );
}

void KCoordTest::testIsBeforeLineEnd()
{
  KCoord Coord( MaxPos-1, Line );

  QVERIFY( Coord.isBeforeLineEnd(MaxPos) );
  QVERIFY( Coord.isValid() );
  Coord.goRight();
  QVERIFY( !Coord.isBeforeLineEnd(MaxPos) );
  QVERIFY( Coord.isValid() );
}


void KCoordTest::testIndexNWidth()
{
  KCoord Coord;
  Coord.setByIndexNWidth( Index, LineWidth );
  QCOMPARE( Coord.indexByLineWidth(LineWidth), Index );
  QVERIFY( Coord.isValid() );
}


QTEST_MAIN( KCoordTest )

#include "kcoordtest.moc"
