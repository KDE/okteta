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

#include "coordrangetest.h"

// test object
#include <coordrange.h>
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
static Coord Start( Pos1, Line1 );
static Coord End(   Pos2, Line2 );

static const Size BufferWidth = Pos2 + 5;


void CoordRangeTest::testConstructorByCoords()
{
    CoordRange coordRange( Start, End );
    QCOMPARE( coordRange.start(), Start );
    QCOMPARE( coordRange.end(),   End );
    QCOMPARE( coordRange.lines(), LineCount );
    QVERIFY( coordRange.isValid() );
}

void CoordRangeTest::testConstructorByPosLines()
{
    LinePositionRange positions( Pos1, Pos2 );
    LineRange lines( Line1, Line2 );

    CoordRange coordRange( positions, lines );
    QCOMPARE( coordRange.start(), Coord(Pos1,Line1) );
    QCOMPARE( coordRange.end(),   Coord(Pos2,Line2) );
    QCOMPARE( coordRange.lines(), LineCount );
    QVERIFY( coordRange.isValid() );
}

void CoordRangeTest::testSimpleConstructor()
{
    CoordRange coordRange;
    QVERIFY( coordRange.isEmpty() );
    QVERIFY( !coordRange.isValid() );
}

void CoordRangeTest::testSetGetStart()
{
    CoordRange coordRange;
    coordRange.setStart( Start );
    QCOMPARE( coordRange.start(), Start );
}

void CoordRangeTest::testSetGetEnd()
{
    CoordRange coordRange;
    coordRange.setEnd( End );
    QCOMPARE( coordRange.end(), End );
}

void CoordRangeTest::testIsEmpty()
{
    CoordRange coordRange( Start, End );
    QVERIFY( !coordRange.isEmpty() );
    coordRange.unset();
    QVERIFY( coordRange.isEmpty() );
}

void CoordRangeTest::testCompare()
{
    CoordRange coordRange( Start, End );
    // same
    CoordRange otherCoordRange( Start, End );
    QVERIFY( coordRange == otherCoordRange );
    // different start
    otherCoordRange.set( Coord(Pos1+1,Line1), End );
    QVERIFY( !(coordRange == otherCoordRange) );
    // different end
    otherCoordRange.set(Start, Coord(Pos2+1,Line2));
    QVERIFY( !(coordRange == otherCoordRange) );
}

void CoordRangeTest::testIncludes()
{
    CoordRange coordRange( Start, End );
    // at start
    Coord coord = coordRange.start();
    QVERIFY( coordRange.includes(coord) );
    // before start
    coord.goLeft();
    QVERIFY( !coordRange.includes(coord) );
    // at end
    coord = coordRange.end();
    QVERIFY( coordRange.includes(coord) );
    // behind end
    coord.goRight();
    QVERIFY( !coordRange.includes(coord) );
}

void CoordRangeTest::testIncludesLine()
{
    CoordRange coordRange( Start, End );
    // at start
    Line L = coordRange.start().line();
    QVERIFY( coordRange.includesLine(L) );
    // before start
    --L;
    QVERIFY( !coordRange.includesLine(L) );
    // at end
    L = coordRange.end().line();
    QVERIFY( coordRange.includesLine(L) );
    // behind end
    ++L;
    QVERIFY( !coordRange.includesLine(L) );
}

void CoordRangeTest::testWidth()
{
    CoordRange coordRange( Start, End );
    const Size width = BufferWidth*(LineCount-1) - Start.pos() + End.pos()+1;
    QCOMPARE( coordRange.width(BufferWidth), width );
}

/*

void CoordRangeTest::testAdaptToChange()
{
    // adaptToChange, same length, behind
    CoordRange coordRange( Start, End );
    CoordRange otherCoordRange( coordRange );
    coordRange.adaptToChange( End+1, Width, Width );
    QCOMPARE( coordRange, otherCoordRange );

    // adaptToChange, same length, before
    coordRange.set( Start, End );
    otherCoordRange.set( coordRange );
    coordRange.adaptToChange( Start-2, 1, 1 );
    QCOMPARE( coordRange, otherCoordRange );

    // adaptToChange, same length, -1,-1
    coordRange.set( Start-1, End-1 );
    otherCoordRange.set( Start-1, Start-1 );
    coordRange.adaptToChange( Start, Width, Width );
    QCOMPARE( coordRange, otherCoordRange );

    // adaptToChange, same length, -1,0
    coordRange.set( Start-1, End );
    otherCoordRange.set( coordRange );
    coordRange.adaptToChange( Start, Width, Width );
    QCOMPARE( coordRange, otherCoordRange );

    // adaptToChange, same length, -1,+1
    coordRange.set( Start-1, End+1 );
    otherCoordRange.set( coordRange );
    coordRange.adaptToChange( Start, Width, Width );
    QCOMPARE( coordRange, otherCoordRange );

    // adaptToChange, same length, 0,-1
    coordRange.set( Start, End-1 );
    otherCoordRange.set( coordRange );
    coordRange.adaptToChange( Start, Width, Width );
    QVERIFY( !coordRange.isValid() );

    // adaptToChange, same length, 0,0
    coordRange.set( Start, End );
    otherCoordRange.set( coordRange );
    coordRange.adaptToChange( Start, Width, Width );
    QCOMPARE( coordRange, otherCoordRange );

    // adaptToChange, same length, 0,+1
    coordRange.set( Start, End+1 );
    otherCoordRange.set( coordRange );
    coordRange.adaptToChange( Start, Width, Width );
    QCOMPARE( coordRange, otherCoordRange );

    // adaptToChange, same length, +1,-1
    coordRange.set( Start+1, End-1 );
    coordRange.adaptToChange( Start, Width, Width );
    QVERIFY( !coordRange.isValid() );

    // adaptToChange, same length, +1,0
    coordRange.set( Start+1, End );
    coordRange.adaptToChange( Start, Width, Width );
    QVERIFY( !coordRange.isValid() );

    // adaptToChange, same length, +1,+1
    coordRange.set( Start+1, End+1 );
    otherCoordRange.set( End+1, End+1 );
    coordRange.adaptToChange( Start, Width, Width );
    QCOMPARE( coordRange, otherCoordRange );


    // adaptToChange, less length, behind
    coordRange.set( Start, End );
    otherCoordRange.set( coordRange );
    coordRange.adaptToChange( End+1, Width, Width-2 );
    QCOMPARE( coordRange, otherCoordRange );

    // adaptToChange, less length, before
    coordRange.set( Start, End );
    otherCoordRange.set( Start-2, End-2 );
    coordRange.adaptToChange( Start-Width, Width, Width-2 );
    QCOMPARE( coordRange, otherCoordRange );

    // adaptToChange, less length, -1,-1
    coordRange.set( Start-1, End-1 );
    otherCoordRange.set( Start-1, Start-1 );
    coordRange.adaptToChange( Start, Width, Width-2 );
    QCOMPARE( coordRange, otherCoordRange );

    // adaptToChange, less length, -1,0
    coordRange.set( Start-1, End );
    otherCoordRange.set( Start-1, End-2 );
    coordRange.adaptToChange( Start, Width, Width-2 );
    QCOMPARE( coordRange, otherCoordRange );

    // adaptToChange, less length, -1,+1
    coordRange.set( Start-1, End+1 );
    otherCoordRange.set( Start-1, End-1 );
    coordRange.adaptToChange( Start, Width, Width-2 );
    QCOMPARE( coordRange, otherCoordRange );

    // adaptToChange, less length, 0,-1
    coordRange.set( Start, End-1 );
    coordRange.adaptToChange( Start, Width, Width-2 );
    QVERIFY( !coordRange.isValid() );

    // adaptToChange, less length, 0,0
    coordRange.set( Start, End );
    otherCoordRange.set( Start, End-2 );
    coordRange.adaptToChange( Start, Width, Width-2 );
    QCOMPARE( coordRange, otherCoordRange );

    // adaptToChange, less length, 0,+1
    coordRange.set( Start, End+1 );
    otherCoordRange.set( Start, End-1 );
    coordRange.adaptToChange( Start, Width, Width-2 );
    QCOMPARE( coordRange, otherCoordRange );

    // adaptToChange, less length, +1,-1
    coordRange.set( Start+1, End-1 );
    coordRange.adaptToChange( Start, Width, Width-2 );
    QVERIFY( !coordRange.isValid() );

    // adaptToChange, less length, +1,0
    coordRange.set( Start+1, End );
    coordRange.adaptToChange( Start, Width, Width-2 );
    QVERIFY( !coordRange.isValid() );

    // adaptToChange, less length, +1,+1
    coordRange.set( Start+1, End+1 );
    otherCoordRange.set( End-1, End-1 );
    coordRange.adaptToChange( Start, Width, Width-2 );
    QCOMPARE( coordRange, otherCoordRange );


    // adaptToChange, greater length, behind
    coordRange.set( Start, End );
    otherCoordRange.set( coordRange );
    coordRange.adaptToChange( End+1, Width, Width+2 );
    QCOMPARE( coordRange, otherCoordRange );

    // adaptToChange, greater length, before
    coordRange.set( Start, End );
    otherCoordRange.set( Start+2, End+2 );
    coordRange.adaptToChange( Start-Width, Width, Width+2 );
    QCOMPARE( coordRange, otherCoordRange );

    // adaptToChange, greater length, -1,-1
    coordRange.set( Start-1, End-1 );
    otherCoordRange.set( Start-1, Start-1 );
    coordRange.adaptToChange( Start, Width, Width+2 );
    QCOMPARE( coordRange, otherCoordRange );

    // adaptToChange, greater length, -1,0
    coordRange.set( Start-1, End );
    otherCoordRange.set( Start-1, End+2 );
    coordRange.adaptToChange( Start, Width, Width+2 );
    QCOMPARE( coordRange, otherCoordRange );

    // adaptToChange, greater length, -1,+1
    coordRange.set( Start-1, End+1 );
    otherCoordRange.set( Start-1, End+3 );
    coordRange.adaptToChange( Start, Width, Width+2 );
    QCOMPARE( coordRange, otherCoordRange );

    // adaptToChange, greater length, 0,-1
    coordRange.set( Start, End-1 );
    coordRange.adaptToChange( Start, Width, Width+2 );
    QVERIFY( !coordRange.isValid() );

    // adaptToChange, greater length, 0,0
    coordRange.set( Start, End );
    otherCoordRange.set( Start, End+2 );
    coordRange.adaptToChange( Start, Width, Width+2 );
    QCOMPARE( coordRange, otherCoordRange );

    // adaptToChange, greater length, 0,+1
    coordRange.set( Start, End+1 );
    otherCoordRange.set( Start, End+3 );
    coordRange.adaptToChange( Start, Width, Width+2 );
    QCOMPARE( coordRange, otherCoordRange );

    // adaptToChange, greater length, +1,-1
    coordRange.set( Start+1, End-1 );
    coordRange.adaptToChange( Start, Width, Width+2 );
    QVERIFY( !coordRange.isValid() );

    // adaptToChange, greater length, +1,0
    coordRange.set( Start+1, End );
    coordRange.adaptToChange( Start, Width, Width+2 );
    QVERIFY( !coordRange.isValid() );

    // adaptToChange, greater length, +1,+1
    coordRange.set( Start+1, End+1 );
    otherCoordRange.set( End+3, End+3 );
    coordRange.adaptToChange( Start, Width, Width+2 );
    QCOMPARE( coordRange, otherCoordRange );


    // adaptToChange, insert, behind
    coordRange.set( Start-1, Start-1 );
    otherCoordRange.set( Start-1, End );
    coordRange.adaptToChange( Start, 0, Width );
    QCOMPARE( coordRange, otherCoordRange );

    // adaptToChange, insert, middle
    coordRange.set( Start-1, Start );
    otherCoordRange.set( Start-1, End+1 );
    coordRange.adaptToChange( Start, 0, Width );
    QCOMPARE( coordRange, otherCoordRange );

    // adaptToChange, insert, before
    coordRange.set( Start, Start );
    otherCoordRange.set( End+1, End+1 );
    coordRange.adaptToChange( Start, 0, Width );
    QCOMPARE( coordRange, otherCoordRange );
}

void CoordRangeTest::testStartForInclude()
{
    // testStartForInclude, same length, start at start
    const CoordRange coordRange( Start, End );
    CoordRange otherCoordRange( coordRange );
    QCOMPARE( coordRange.startForInclude(otherCoordRange), otherCoordRange.start() );

    // testStartForInclude, same length, start before start
    otherCoordRange.moveBy( -1 );
    QCOMPARE( coordRange.startForInclude(otherCoordRange), otherCoordRange.start() );

    // testStartForInclude, same length, end behind end
    otherCoordRange.moveBy( 2 );
    QCOMPARE( coordRange.startForInclude(otherCoordRange), otherCoordRange.start() );

    // testStartForInclude, smaller length, start at start
    otherCoordRange.set( Start, End-1 );
    int Diff = coordRange.width() - otherCoordRange.width();
    QCOMPARE( coordRange.startForInclude(otherCoordRange), otherCoordRange.start() );

    // testStartForInclude, smaller length, start before start
    otherCoordRange.moveBy( -1 );
    QCOMPARE( coordRange.startForInclude(otherCoordRange), otherCoordRange.start() );

    // testStartForInclude, smaller length, end behind end
    otherCoordRange.moveBy( 2 );
    QCOMPARE( coordRange.startForInclude(otherCoordRange), otherCoordRange.start()-Diff );

*    // undefined in startForInclude
    // testStartForInclude, greater length, start at start
    otherCoordRange.set( Start, End+1 );
    Diff = coordRange.width() - otherCoordRange.width();
    QCOMPARE( coordRange.startForInclude(otherCoordRange), otherCoordRange.start() );

    // testStartForInclude, smaller length, start before start
    otherCoordRange.moveBy( -1 );
    QCOMPARE( coordRange.startForInclude(otherCoordRange), otherCoordRange.start() );

    // testStartForInclude, smaller length, end behind end
    otherCoordRange.moveBy( 2 );
    QCOMPARE( coordRange.startForInclude(otherCoordRange), otherCoordRange.start()-Diff );
*
}


void CoordRangeTest::testIsJoinable()
{
    // testStartForInclude, same length, start at start
    const CoordRange coordRange( Start, End );
    CoordRange otherCoordRange( coordRange );
    QVERIFY( coordRange.isJoinable(otherCoordRange) );

    // adaptToChange, insert, before
    otherCoordRange.set( End+1, End+Width );
    QVERIFY( coordRange.isJoinable(otherCoordRange) );

    // Overlapping, right shifted
    otherCoordRange.set( Start+1, End+1 );
    QVERIFY( coordRange.isJoinable(otherCoordRange) );

    // Overlapping, left shifted
    otherCoordRange.set( Start-1, End-1 );
    QVERIFY( coordRange.isJoinable(otherCoordRange) );

    // Overlapping, 1 includes 2
    otherCoordRange.set( Start+1, End-1 );
    QVERIFY( coordRange.isJoinable(otherCoordRange) );

    // Overlapping, 2 includes 1
    otherCoordRange.set( Start-1, End+1 );
    QVERIFY( coordRange.isJoinable(otherCoordRange) );

    // Overlapping, identic
    otherCoordRange.set( coordRange );
    QVERIFY( coordRange.isJoinable(otherCoordRange) );

    // Coupled, first, then second
    otherCoordRange.set( End+1, End+Width );
    QVERIFY( coordRange.isJoinable(otherCoordRange) );

    // Coupled, second, then first
    QVERIFY( otherCoordRange.isJoinable(coordRange) );

    // NonOverlapping, first, then second
    otherCoordRange.set( End+2, End+Width+1 );
    QVERIFY( !coordRange.isJoinable(otherCoordRange) );

    // NonOverlapping, second, then first
    QVERIFY( !otherCoordRange.isJoinable(coordRange) );
}
*/

}

QTEST_MAIN( Okteta::CoordRangeTest )
