/***************************************************************************
                          kcoordrangetest.cpp -  description
                             -------------------
    begin                : Thu May 25 2006
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
#include <kcoordrange.h>
#include "kcoordrangetest.h"

using namespace KHE;
using namespace KHEUI;

// local variables
static const int Pos1 = 15;
static const int Pos2 = 25;
static const int Line1 = 10;
static const int LineSize = 10;
static const int Line2 = Line1 + LineSize - 1;;
static KBufferCoord Start( Pos1, Line1 );
static KBufferCoord End(   Pos2, Line2 );

static const int BufferWidth = Pos2 + 5;


void KCoordRangeTest::testConstructorByCoords()
{
  KCoordRange CoordRange( Start, End );
  QCOMPARE( CoordRange.start(), Start );
  QCOMPARE( CoordRange.end(),   End );
  QCOMPARE( CoordRange.lines(), LineSize );
  QVERIFY( CoordRange.isValid() );
}

void KCoordRangeTest::testConstructorByPosLines()
{
  KSection Pos( Pos1, Pos2 );
  KSection Lines( Line1, Line2 );

  KCoordRange CoordRange( Pos, Lines );
  QCOMPARE( CoordRange.start(), KBufferCoord(Pos1,Line1) );
  QCOMPARE( CoordRange.end(),   KBufferCoord(Pos2,Line2) );
  QCOMPARE( CoordRange.lines(), LineSize );
  QVERIFY( CoordRange.isValid() );
}

void KCoordRangeTest::testSimpleConstructor()
{
  KCoordRange CoordRange;
  QVERIFY( CoordRange.isEmpty() );
  QVERIFY( !CoordRange.isValid() );
}

void KCoordRangeTest::testSetGetStart()
{
  KCoordRange CoordRange;
  CoordRange.setStart( Start );
  QCOMPARE( CoordRange.start(), Start );
}

void KCoordRangeTest::testSetGetEnd()
{
  KCoordRange CoordRange;
  CoordRange.setEnd( End );
  QCOMPARE( CoordRange.end(), End );
}

void KCoordRangeTest::testIsEmpty()
{
  KCoordRange CoordRange( Start, End );
  QVERIFY( !CoordRange.isEmpty() );
  CoordRange.unset();
  QVERIFY( CoordRange.isEmpty() );
}

void KCoordRangeTest::testCompare()
{
  KCoordRange CoordRange( Start, End );
  // same
  KCoordRange OtherCoordRange( Start, End );
  QVERIFY( CoordRange == OtherCoordRange );
  // different start
  OtherCoordRange.set( KBufferCoord(Pos1+1,Line1), End );
  QVERIFY( !(CoordRange == OtherCoordRange) );
  // different end
  OtherCoordRange.set(Start, KBufferCoord(Pos2+1,Line2));
  QVERIFY( !(CoordRange == OtherCoordRange) );
}

void KCoordRangeTest::testIncludes()
{
  KCoordRange CoordRange( Start, End );
  // at start
  KBufferCoord C = CoordRange.start();
  QVERIFY( CoordRange.includes(C) );
  // before start
  C.goLeft();
  QVERIFY( !CoordRange.includes(C) );
  // at end
  C = CoordRange.end();
  QVERIFY( CoordRange.includes(C) );
  // behind end
  C.goRight();
  QVERIFY( !CoordRange.includes(C) );
}

void KCoordRangeTest::testIncludesLine()
{
  KCoordRange CoordRange( Start, End );
  // at start
  int L = CoordRange.start().line();
  QVERIFY( CoordRange.includesLine(L) );
  // before start
  --L;
  QVERIFY( !CoordRange.includesLine(L) );
  // at end
  L = CoordRange.end().line();
  QVERIFY( CoordRange.includesLine(L) );
  // behind end
  ++L;
  QVERIFY( !CoordRange.includesLine(L) );
}

void KCoordRangeTest::testWidth()
{
  KCoordRange CoordRange( Start, End );
  int Width = BufferWidth*(LineSize-1) - Start.pos() + End.pos()+1;
  QCOMPARE( CoordRange.width(BufferWidth), Width );
}

/*

void KCoordRangeTest::testAdaptToChange()
{
  // adaptToChange, same length, behind
  KCoordRange CoordRange( Start, End );
  KCoordRange OtherCoordRange( CoordRange );
  CoordRange.adaptToChange( End+1, Width, Width );
  QCOMPARE( CoordRange, OtherCoordRange );

  // adaptToChange, same length, before
  CoordRange.set( Start, End );
  OtherCoordRange.set( CoordRange );
  CoordRange.adaptToChange( Start-2, 1, 1 );
  QCOMPARE( CoordRange, OtherCoordRange );

  // adaptToChange, same length, -1,-1
  CoordRange.set( Start-1, End-1 );
  OtherCoordRange.set( Start-1, Start-1 );
  CoordRange.adaptToChange( Start, Width, Width );
  QCOMPARE( CoordRange, OtherCoordRange );

  // adaptToChange, same length, -1,0
  CoordRange.set( Start-1, End );
  OtherCoordRange.set( CoordRange );
  CoordRange.adaptToChange( Start, Width, Width );
  QCOMPARE( CoordRange, OtherCoordRange );

  // adaptToChange, same length, -1,+1
  CoordRange.set( Start-1, End+1 );
  OtherCoordRange.set( CoordRange );
  CoordRange.adaptToChange( Start, Width, Width );
  QCOMPARE( CoordRange, OtherCoordRange );

  // adaptToChange, same length, 0,-1
  CoordRange.set( Start, End-1 );
  OtherCoordRange.set( CoordRange );
  CoordRange.adaptToChange( Start, Width, Width );
  QVERIFY( !CoordRange.isValid() );

  // adaptToChange, same length, 0,0
  CoordRange.set( Start, End );
  OtherCoordRange.set( CoordRange );
  CoordRange.adaptToChange( Start, Width, Width );
  QCOMPARE( CoordRange, OtherCoordRange );

  // adaptToChange, same length, 0,+1
  CoordRange.set( Start, End+1 );
  OtherCoordRange.set( CoordRange );
  CoordRange.adaptToChange( Start, Width, Width );
  QCOMPARE( CoordRange, OtherCoordRange );

  // adaptToChange, same length, +1,-1
  CoordRange.set( Start+1, End-1 );
  CoordRange.adaptToChange( Start, Width, Width );
  QVERIFY( !CoordRange.isValid() );

  // adaptToChange, same length, +1,0
  CoordRange.set( Start+1, End );
  CoordRange.adaptToChange( Start, Width, Width );
  QVERIFY( !CoordRange.isValid() );

  // adaptToChange, same length, +1,+1
  CoordRange.set( Start+1, End+1 );
  OtherCoordRange.set( End+1, End+1 );
  CoordRange.adaptToChange( Start, Width, Width );
  QCOMPARE( CoordRange, OtherCoordRange );


  // adaptToChange, less length, behind
  CoordRange.set( Start, End );
  OtherCoordRange.set( CoordRange );
  CoordRange.adaptToChange( End+1, Width, Width-2 );
  QCOMPARE( CoordRange, OtherCoordRange );

  // adaptToChange, less length, before
  CoordRange.set( Start, End );
  OtherCoordRange.set( Start-2, End-2 );
  CoordRange.adaptToChange( Start-Width, Width, Width-2 );
  QCOMPARE( CoordRange, OtherCoordRange );

  // adaptToChange, less length, -1,-1
  CoordRange.set( Start-1, End-1 );
  OtherCoordRange.set( Start-1, Start-1 );
  CoordRange.adaptToChange( Start, Width, Width-2 );
  QCOMPARE( CoordRange, OtherCoordRange );

  // adaptToChange, less length, -1,0
  CoordRange.set( Start-1, End );
  OtherCoordRange.set( Start-1, End-2 );
  CoordRange.adaptToChange( Start, Width, Width-2 );
  QCOMPARE( CoordRange, OtherCoordRange );

  // adaptToChange, less length, -1,+1
  CoordRange.set( Start-1, End+1 );
  OtherCoordRange.set( Start-1, End-1 );
  CoordRange.adaptToChange( Start, Width, Width-2 );
  QCOMPARE( CoordRange, OtherCoordRange );

  // adaptToChange, less length, 0,-1
  CoordRange.set( Start, End-1 );
  CoordRange.adaptToChange( Start, Width, Width-2 );
  QVERIFY( !CoordRange.isValid() );

  // adaptToChange, less length, 0,0
  CoordRange.set( Start, End );
  OtherCoordRange.set( Start, End-2 );
  CoordRange.adaptToChange( Start, Width, Width-2 );
  QCOMPARE( CoordRange, OtherCoordRange );

  // adaptToChange, less length, 0,+1
  CoordRange.set( Start, End+1 );
  OtherCoordRange.set( Start, End-1 );
  CoordRange.adaptToChange( Start, Width, Width-2 );
  QCOMPARE( CoordRange, OtherCoordRange );

  // adaptToChange, less length, +1,-1
  CoordRange.set( Start+1, End-1 );
  CoordRange.adaptToChange( Start, Width, Width-2 );
  QVERIFY( !CoordRange.isValid() );

  // adaptToChange, less length, +1,0
  CoordRange.set( Start+1, End );
  CoordRange.adaptToChange( Start, Width, Width-2 );
  QVERIFY( !CoordRange.isValid() );

  // adaptToChange, less length, +1,+1
  CoordRange.set( Start+1, End+1 );
  OtherCoordRange.set( End-1, End-1 );
  CoordRange.adaptToChange( Start, Width, Width-2 );
  QCOMPARE( CoordRange, OtherCoordRange );


  // adaptToChange, greater length, behind
  CoordRange.set( Start, End );
  OtherCoordRange.set( CoordRange );
  CoordRange.adaptToChange( End+1, Width, Width+2 );
  QCOMPARE( CoordRange, OtherCoordRange );

  // adaptToChange, greater length, before
  CoordRange.set( Start, End );
  OtherCoordRange.set( Start+2, End+2 );
  CoordRange.adaptToChange( Start-Width, Width, Width+2 );
  QCOMPARE( CoordRange, OtherCoordRange );

  // adaptToChange, greater length, -1,-1
  CoordRange.set( Start-1, End-1 );
  OtherCoordRange.set( Start-1, Start-1 );
  CoordRange.adaptToChange( Start, Width, Width+2 );
  QCOMPARE( CoordRange, OtherCoordRange );

  // adaptToChange, greater length, -1,0
  CoordRange.set( Start-1, End );
  OtherCoordRange.set( Start-1, End+2 );
  CoordRange.adaptToChange( Start, Width, Width+2 );
  QCOMPARE( CoordRange, OtherCoordRange );

  // adaptToChange, greater length, -1,+1
  CoordRange.set( Start-1, End+1 );
  OtherCoordRange.set( Start-1, End+3 );
  CoordRange.adaptToChange( Start, Width, Width+2 );
  QCOMPARE( CoordRange, OtherCoordRange );

  // adaptToChange, greater length, 0,-1
  CoordRange.set( Start, End-1 );
  CoordRange.adaptToChange( Start, Width, Width+2 );
  QVERIFY( !CoordRange.isValid() );

  // adaptToChange, greater length, 0,0
  CoordRange.set( Start, End );
  OtherCoordRange.set( Start, End+2 );
  CoordRange.adaptToChange( Start, Width, Width+2 );
  QCOMPARE( CoordRange, OtherCoordRange );

  // adaptToChange, greater length, 0,+1
  CoordRange.set( Start, End+1 );
  OtherCoordRange.set( Start, End+3 );
  CoordRange.adaptToChange( Start, Width, Width+2 );
  QCOMPARE( CoordRange, OtherCoordRange );

  // adaptToChange, greater length, +1,-1
  CoordRange.set( Start+1, End-1 );
  CoordRange.adaptToChange( Start, Width, Width+2 );
  QVERIFY( !CoordRange.isValid() );

  // adaptToChange, greater length, +1,0
  CoordRange.set( Start+1, End );
  CoordRange.adaptToChange( Start, Width, Width+2 );
  QVERIFY( !CoordRange.isValid() );

  // adaptToChange, greater length, +1,+1
  CoordRange.set( Start+1, End+1 );
  OtherCoordRange.set( End+3, End+3 );
  CoordRange.adaptToChange( Start, Width, Width+2 );
  QCOMPARE( CoordRange, OtherCoordRange );


  // adaptToChange, insert, behind
  CoordRange.set( Start-1, Start-1 );
  OtherCoordRange.set( Start-1, End );
  CoordRange.adaptToChange( Start, 0, Width );
  QCOMPARE( CoordRange, OtherCoordRange );

  // adaptToChange, insert, middle
  CoordRange.set( Start-1, Start );
  OtherCoordRange.set( Start-1, End+1 );
  CoordRange.adaptToChange( Start, 0, Width );
  QCOMPARE( CoordRange, OtherCoordRange );

  // adaptToChange, insert, before
  CoordRange.set( Start, Start );
  OtherCoordRange.set( End+1, End+1 );
  CoordRange.adaptToChange( Start, 0, Width );
  QCOMPARE( CoordRange, OtherCoordRange );
}

void KCoordRangeTest::testStartForInclude()
{
  // testStartForInclude, same length, start at start
  const KCoordRange CoordRange( Start, End );
  KCoordRange OtherCoordRange( CoordRange );
  QCOMPARE( CoordRange.startForInclude(OtherCoordRange), OtherCoordRange.start() );

  // testStartForInclude, same length, start before start
  OtherCoordRange.moveBy( -1 );
  QCOMPARE( CoordRange.startForInclude(OtherCoordRange), OtherCoordRange.start() );

  // testStartForInclude, same length, end behind end
  OtherCoordRange.moveBy( 2 );
  QCOMPARE( CoordRange.startForInclude(OtherCoordRange), OtherCoordRange.start() );

  // testStartForInclude, smaller length, start at start
  OtherCoordRange.set( Start, End-1 );
  int Diff = CoordRange.width() - OtherCoordRange.width();
  QCOMPARE( CoordRange.startForInclude(OtherCoordRange), OtherCoordRange.start() );

  // testStartForInclude, smaller length, start before start
  OtherCoordRange.moveBy( -1 );
  QCOMPARE( CoordRange.startForInclude(OtherCoordRange), OtherCoordRange.start() );

  // testStartForInclude, smaller length, end behind end
  OtherCoordRange.moveBy( 2 );
  QCOMPARE( CoordRange.startForInclude(OtherCoordRange), OtherCoordRange.start()-Diff );

/* // undefined in startForInclude
  // testStartForInclude, greater length, start at start
  OtherCoordRange.set( Start, End+1 );
  Diff = CoordRange.width() - OtherCoordRange.width();
  QCOMPARE( CoordRange.startForInclude(OtherCoordRange), OtherCoordRange.start() );

  // testStartForInclude, smaller length, start before start
  OtherCoordRange.moveBy( -1 );
  QCOMPARE( CoordRange.startForInclude(OtherCoordRange), OtherCoordRange.start() );

  // testStartForInclude, smaller length, end behind end
  OtherCoordRange.moveBy( 2 );
  QCOMPARE( CoordRange.startForInclude(OtherCoordRange), OtherCoordRange.start()-Diff );
*
}


void KCoordRangeTest::testIsJoinable()
{
  // testStartForInclude, same length, start at start
  const KCoordRange CoordRange( Start, End );
  KCoordRange OtherCoordRange( CoordRange );
  QVERIFY( CoordRange.isJoinable(OtherCoordRange) );

  // adaptToChange, insert, before
  OtherCoordRange.set( End+1, End+Width );
  QVERIFY( CoordRange.isJoinable(OtherCoordRange) );

  // Overlapping, right shifted
  OtherCoordRange.set( Start+1, End+1 );
  QVERIFY( CoordRange.isJoinable(OtherCoordRange) );

  // Overlapping, left shifted
  OtherCoordRange.set( Start-1, End-1 );
  QVERIFY( CoordRange.isJoinable(OtherCoordRange) );

  // Overlapping, 1 includes 2
  OtherCoordRange.set( Start+1, End-1 );
  QVERIFY( CoordRange.isJoinable(OtherCoordRange) );

  // Overlapping, 2 includes 1
  OtherCoordRange.set( Start-1, End+1 );
  QVERIFY( CoordRange.isJoinable(OtherCoordRange) );

  // Overlapping, identic
  OtherCoordRange.set( CoordRange );
  QVERIFY( CoordRange.isJoinable(OtherCoordRange) );

  // Coupled, first, then second
  OtherCoordRange.set( End+1, End+Width );
  QVERIFY( CoordRange.isJoinable(OtherCoordRange) );

  // Coupled, second, then first
  QVERIFY( OtherCoordRange.isJoinable(CoordRange) );

  // NonOverlapping, first, then second
  OtherCoordRange.set( End+2, End+Width+1 );
  QVERIFY( !CoordRange.isJoinable(OtherCoordRange) );

  // NonOverlapping, second, then first
  QVERIFY( !OtherCoordRange.isJoinable(CoordRange) );
}
*/


QTEST_MAIN( KCoordRangeTest )

#include "kcoordrangetest.moc"
