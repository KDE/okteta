/*
    This file is part of the Okteta Gui library, made within the KDE community.

    Copyright 2006 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "coordrangelisttest.h"

// test object
#include <coordrangelist.h>
// Qt
#include <QtTest/QtTest>


namespace Okteta
{

// local variables
static const LinePosition Pos1 = 15;
static const LinePosition Pos2 = 25;
static const Line Line1 = 10;
static const LineSize LineCount = 10;
static const Line Line2 = Line1 + LineCount - 1;
static const LineSize LineDistance = 5;
static const Line Line3 = Line2 + LineDistance;
static const Line Line4 = Line3 + LineCount - 1;

static Coord Start1( Pos1, Line1 );
static Coord End1(   Pos2, Line2 );
static Coord Start2( Pos1, Line3 );
static Coord End2(   Pos2, Line4 );

static const Size BufferWidth = Pos2 + 5;


void CoordRangeListTest::testSimpleConstructor()
{
  CoordRangeList CoordRangeList;
  QVERIFY( CoordRangeList.isEmpty() );
}

void CoordRangeListTest::testAddSingleSection()
{
  CoordRangeList CoordRangeList;

  CoordRange CoordRange( Start1, End1 );
  CoordRangeList.addCoordRange( CoordRange );
  QCOMPARE( CoordRangeList.size(), 1 );
  QCOMPARE( CoordRangeList.first(), CoordRange );
}

void CoordRangeListTest::testAddJoinableSections()
{
  CoordRangeList CoordRangeList;

  CoordRange CoordRange1( Start1, End1 );
  CoordRange CoordRange2( Start2, End2 );
  // in front
  Coord OtherStart( 0, 0 );
  Coord OtherEnd = Start1;
  OtherEnd.goLeft(0);
  CoordRange OtherCoordRange( OtherStart, OtherEnd );
  CoordRangeList.addCoordRange( CoordRange1 );
  CoordRangeList.addCoordRange( CoordRange2 );
  CoordRangeList.addCoordRange( OtherCoordRange );

  QCOMPARE( CoordRangeList.size(), 2 );
  QCOMPARE( CoordRangeList.first(), CoordRange(OtherCoordRange.start(),CoordRange1.end()) );
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
  QCOMPARE( CoordRangeList.first(), CoordRange(CoordRange1.start(),OtherCoordRange.end()) );
  QCOMPARE( CoordRangeList.last(), CoordRange2 );

  // in the middle, with both
  CoordRangeList.clear();
  OtherEnd.goRight( 1 );
  OtherCoordRange.setEnd( OtherEnd );
  CoordRangeList.addCoordRange( CoordRange1 );
  CoordRangeList.addCoordRange( CoordRange2 );
  CoordRangeList.addCoordRange( OtherCoordRange );

  QCOMPARE( CoordRangeList.size(), 1 );
  QCOMPARE( CoordRangeList.first(), CoordRange(CoordRange1.start(),CoordRange2.end()) );

  // in the middle, with last
  CoordRangeList.clear();
  OtherStart.goRight( 1 );
  OtherCoordRange.setStart( OtherStart );
  CoordRangeList.addCoordRange( CoordRange1 );
  CoordRangeList.addCoordRange( CoordRange2 );
  CoordRangeList.addCoordRange( OtherCoordRange );

  QCOMPARE( CoordRangeList.size(), 2 );
  QCOMPARE( CoordRangeList.first(), CoordRange1 );
  QCOMPARE( CoordRangeList.last(), CoordRange(OtherCoordRange.start(),CoordRange2.end()) );

  // behind
  CoordRangeList.clear();
  OtherStart = End2;
  OtherStart.goRight( 0 );
  OtherEnd = OtherStart;
  OtherEnd.goDown( LineCount );
  OtherCoordRange.set( OtherStart, OtherEnd );
  CoordRangeList.addCoordRange( CoordRange1 );
  CoordRangeList.addCoordRange( CoordRange2 );
  CoordRangeList.addCoordRange( OtherCoordRange );

  QCOMPARE( CoordRangeList.size(), 2 );
  QCOMPARE( CoordRangeList.first(), CoordRange1 );
  QCOMPARE( CoordRangeList.last(), CoordRange(CoordRange2.start(),OtherCoordRange.end()) );
}

void CoordRangeListTest::testAddNonJoinableSections()
{
  CoordRangeList CoordRangeList;

  CoordRange CoordRange1( Start1, End1 );
  CoordRange CoordRange2( Start2, End2 );
  // in front
  Coord OtherStart( 0, 0 );
  Coord OtherEnd = Start1;
  OtherEnd.goLeft(2);
  CoordRange OtherCoordRange( OtherStart, OtherEnd );
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
  OtherEnd.goDown( LineCount );
  OtherCoordRange.set( OtherStart, OtherEnd );
  CoordRangeList.addCoordRange( CoordRange1 );
  CoordRangeList.addCoordRange( CoordRange2 );
  CoordRangeList.addCoordRange( OtherCoordRange );

  QCOMPARE( CoordRangeList.size(), 3 );
  QCOMPARE( CoordRangeList.first(), CoordRange1 );
  QCOMPARE( CoordRangeList.last(), OtherCoordRange );
}

}

QTEST_MAIN( Okteta::CoordRangeListTest )
