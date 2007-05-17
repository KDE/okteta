/***************************************************************************
                          kcoordrangelisttest.cpp -  description
                             -------------------
    begin                : Thu Mai 25 2006
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


#include "kcoordrangelisttest.h"

// test object
#include <kcoordrangelist.h>
// Qt
#include <QtTest/QtTest>

using namespace KHEUI;

// local variables
static const int Pos1 = 15;
static const int Pos2 = 25;
static const int Line1 = 10;
static const int LineSize = 10;
static const int Line2 = Line1 + LineSize - 1;
static const int LineDistance = 5;
static const int Line3 = Line2 + LineDistance;
static const int Line4 = Line3 + LineSize - 1;

static KCoord Start1( Pos1, Line1 );
static KCoord End1(   Pos2, Line2 );
static KCoord Start2( Pos1, Line3 );
static KCoord End2(   Pos2, Line4 );

static const int BufferWidth = Pos2 + 5;


void KCoordRangeListTest::testSimpleConstructor()
{
  KCoordRangeList CoordRangeList;
  QVERIFY( CoordRangeList.isEmpty() );
}

void KCoordRangeListTest::testAddSingleSection()
{
  KCoordRangeList CoordRangeList;

  KCoordRange CoordRange( Start1, End1 );
  CoordRangeList.addCoordRange( CoordRange );
  QCOMPARE( CoordRangeList.size(), 1 );
  QCOMPARE( CoordRangeList.first(), CoordRange );
}

void KCoordRangeListTest::testAddJoinableSections()
{
  KCoordRangeList CoordRangeList;

  KCoordRange CoordRange1( Start1, End1 );
  KCoordRange CoordRange2( Start2, End2 );
  // in front
  KCoord OtherStart( 0, 0 );
  KCoord OtherEnd = Start1;
  OtherEnd.goLeft(0);
  KCoordRange OtherCoordRange( OtherStart, OtherEnd );
  CoordRangeList.addCoordRange( CoordRange1 );
  CoordRangeList.addCoordRange( CoordRange2 );
  CoordRangeList.addCoordRange( OtherCoordRange );

  QCOMPARE( CoordRangeList.size(), 2 );
  QCOMPARE( CoordRangeList.first(), KCoordRange(OtherCoordRange.start(),CoordRange1.end()) );
  QCOMPARE( CoordRangeList.last(), CoordRange2 );

  // in the middle, with first
  CoordRangeList.clear();
  OtherStart = End1;
  OtherStart.goRight( 0 );
  OtherEnd = Start2;
  OtherEnd.goLeft( 1 );
  OtherCoordRange.set( OtherStart, OtherEnd );
  CoordRangeList.addCoordRange( CoordRange1 );
  CoordRangeList.addCoordRange( CoordRange2 );
  CoordRangeList.addCoordRange( OtherCoordRange );

  QCOMPARE( CoordRangeList.size(), 2 );
  QCOMPARE( CoordRangeList.first(), KCoordRange(CoordRange1.start(),OtherCoordRange.end()) );
  QCOMPARE( CoordRangeList.last(), CoordRange2 );

  // in the middle, with both
  CoordRangeList.clear();
  OtherEnd.goRight( 1 );
  OtherCoordRange.setEnd( OtherEnd );
  CoordRangeList.addCoordRange( CoordRange1 );
  CoordRangeList.addCoordRange( CoordRange2 );
  CoordRangeList.addCoordRange( OtherCoordRange );

  QCOMPARE( CoordRangeList.size(), 1 );
  QCOMPARE( CoordRangeList.first(), KCoordRange(CoordRange1.start(),CoordRange2.end()) );

  // in the middle, with last
  CoordRangeList.clear();
  OtherStart.goRight( 1 );
  OtherCoordRange.setStart( OtherStart );
  CoordRangeList.addCoordRange( CoordRange1 );
  CoordRangeList.addCoordRange( CoordRange2 );
  CoordRangeList.addCoordRange( OtherCoordRange );

  QCOMPARE( CoordRangeList.size(), 2 );
  QCOMPARE( CoordRangeList.first(), CoordRange1 );
  QCOMPARE( CoordRangeList.last(), KCoordRange(OtherCoordRange.start(),CoordRange2.end()) );

  // behind
  CoordRangeList.clear();
  OtherStart = End2;
  OtherStart.goRight( 0 );
  OtherEnd = OtherStart;
  OtherEnd.goDown( LineSize );
  OtherCoordRange.set( OtherStart, OtherEnd );
  CoordRangeList.addCoordRange( CoordRange1 );
  CoordRangeList.addCoordRange( CoordRange2 );
  CoordRangeList.addCoordRange( OtherCoordRange );

  QCOMPARE( CoordRangeList.size(), 2 );
  QCOMPARE( CoordRangeList.first(), CoordRange1 );
  QCOMPARE( CoordRangeList.last(), KCoordRange(CoordRange2.start(),OtherCoordRange.end()) );
}

void KCoordRangeListTest::testAddNonJoinableSections()
{
  KCoordRangeList CoordRangeList;

  KCoordRange CoordRange1( Start1, End1 );
  KCoordRange CoordRange2( Start2, End2 );
  // in front
  KCoord OtherStart( 0, 0 );
  KCoord OtherEnd = Start1;
  OtherEnd.goLeft(2);
  KCoordRange OtherCoordRange( OtherStart, OtherEnd );
  CoordRangeList.addCoordRange( CoordRange1 );
  CoordRangeList.addCoordRange( CoordRange2 );
  CoordRangeList.addCoordRange( OtherCoordRange );

  QCOMPARE( CoordRangeList.size(), 3 );
  QCOMPARE( CoordRangeList.first(), OtherCoordRange );
  QCOMPARE( CoordRangeList.last(), CoordRange2 );

  // in the middle
  CoordRangeList.clear();
  OtherStart = End1;
  OtherStart.goRight( 2 );
  OtherEnd = Start2;
  OtherEnd.goLeft( 2 );
  OtherCoordRange.set( OtherStart, OtherEnd );
  CoordRangeList.addCoordRange( CoordRange1 );
  CoordRangeList.addCoordRange( CoordRange2 );
  CoordRangeList.addCoordRange( OtherCoordRange );

  QCOMPARE( CoordRangeList.size(), 3 );
  QCOMPARE( CoordRangeList.first(), CoordRange1 );
  QCOMPARE( CoordRangeList.last(), CoordRange2 );

  // behind
  CoordRangeList.clear();
  OtherStart = End2;
  OtherStart.goRight( 2 );
  OtherEnd = OtherStart;
  OtherEnd.goDown( LineSize );
  OtherCoordRange.set( OtherStart, OtherEnd );
  CoordRangeList.addCoordRange( CoordRange1 );
  CoordRangeList.addCoordRange( CoordRange2 );
  CoordRangeList.addCoordRange( OtherCoordRange );

  QCOMPARE( CoordRangeList.size(), 3 );
  QCOMPARE( CoordRangeList.first(), CoordRange1 );
  QCOMPARE( CoordRangeList.last(), OtherCoordRange );
}


QTEST_MAIN( KCoordRangeListTest )

#include "kcoordrangelisttest.moc"
