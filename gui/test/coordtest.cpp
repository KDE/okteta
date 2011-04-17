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

#include "coordtest.h"

// test object
#include <coord.h>
// Qt
#include <QtTest/QtTest>


namespace Okteta
{

// local variables
static const Line LineIndex =          3;
static const LinePosition Pos =        15;
static const LineSize LineWidth =      19;
static const Address Index =           LineIndex*LineWidth + Pos;
static const LinePositionSize MaxPos = LineWidth-1;
static const LinePositionSize Steps =  5;


void CoordTest::testConstructor()
{
    const Coord coord;

    QCOMPARE( coord.pos(), 0 );
    QCOMPARE( coord.line(), 0 );
    QVERIFY( coord.isValid() );
}


void CoordTest::testSetConstructor()
{
    const Coord coord( Pos, LineIndex );

    QCOMPARE( coord.pos(), Pos );
    QCOMPARE( coord.line(), LineIndex );
    QVERIFY( coord.isValid() );
}


void CoordTest::testFromIndexConstructor()
{
    const Coord coord = Coord::fromIndex( Index, LineWidth );

    QCOMPARE( coord.indexByLineWidth(LineWidth), Index );
    QCOMPARE( coord.pos(), Pos );
    QCOMPARE( coord.line(), LineIndex );
    QVERIFY( coord.isValid() );
}


void CoordTest::testCopyConstructor()
{
    const Coord coord( Pos, LineIndex );
    const Coord otherCoord( coord );

    QCOMPARE( otherCoord.pos(), Pos );
    QCOMPARE( otherCoord.line(), LineIndex );
    QVERIFY( otherCoord.isValid() );
}


void CoordTest::testEquals()
{
    const Coord coord( Pos, LineIndex );

    Coord otherCoord( coord );
    QVERIFY( otherCoord == coord );
    otherCoord.goRight();
    QVERIFY( otherCoord != coord );
}


void CoordTest::testAssign()
{
    const Coord coord( Pos, LineIndex );

    Coord otherCoord;
    otherCoord = coord;
    QCOMPARE( otherCoord, coord );
}


void CoordTest::testSetPosLine()
{
    Coord coord;
    // setPos()
    coord.setPos( Pos );
    QCOMPARE( coord.pos(), Pos );
    // setLine()
    coord.setLine( LineIndex );
    QCOMPARE( coord.line(), LineIndex );
}


void CoordTest::testSet()
{
    Coord coord;
    coord.set( Pos, LineIndex );
    QCOMPARE( coord.pos(), Pos );
    QCOMPARE( coord.line(), LineIndex );
}


void CoordTest::testIsAtStart()
{
    Coord coord(0,0);

    QVERIFY( coord.isAtStart() );
    coord.set( Pos, LineIndex );
    QVERIFY( !coord.isAtStart() );
    coord.set( 0, 0 );
    QVERIFY( coord.isAtStart() );
}


void CoordTest::testGotoStart()
{
    Coord coord;
    coord.gotoStart();
    QVERIFY( coord.isAtStart() );
}


void CoordTest::testGo()
{
    const Coord coord( Pos, LineIndex );
    Coord otherCoord( coord );

    // going clockwise by one
    otherCoord.goRight(); otherCoord.goDown(); otherCoord.goLeft(); otherCoord.goUp();
    QCOMPARE( otherCoord, coord );
    // going against clockwise by one
    otherCoord.goUp(); otherCoord.goLeft(); otherCoord.goDown(); otherCoord.goRight();
    QCOMPARE( otherCoord, coord );
}


void CoordTest::testGoN()
{
    const Coord coord( Pos, LineIndex );
    Coord otherCoord( coord );

    // going clockwise by N
    otherCoord.goRight( Steps ); otherCoord.goDown( Steps );
    otherCoord.goLeft( Steps ); otherCoord.goUp( Steps );
    QCOMPARE( otherCoord, coord );
    // going against clockwise by N
    otherCoord.goUp( Steps ); otherCoord.goLeft( Steps );
    otherCoord.goDown( Steps ); otherCoord.goRight( Steps );
    QCOMPARE( otherCoord, coord );
}


void CoordTest::testGotoControlled()
{
    Coord coord( MaxPos, LineIndex );
    Coord otherCoord( coord );

    // gotoStartOfNextLine(), goCRight()
    coord.gotoStartOfNextLine();
    otherCoord.goCRight( MaxPos );
    QCOMPARE( coord, otherCoord );
    // gotoEndOfPreviousLine(), goCLeft()
    coord.gotoEndOfPreviousLine( MaxPos );
    otherCoord.goCLeft( MaxPos );
    QCOMPARE( coord, otherCoord );
}


void CoordTest::testGoLineStart()
{
    const Coord coord( Pos, LineIndex );
    Coord otherCoord;

    // goLineStart(), same line
    otherCoord.setLine( coord.line() );
    otherCoord.goLineStart( coord );
    QCOMPARE( otherCoord, coord );
    // goLineStart(), another line
    otherCoord.goUp();
    otherCoord.goLineStart( coord );
    QCOMPARE( otherCoord.pos(), 0 );
}


void CoordTest::testGoLineEnd()
{
    const Coord coord( Pos, LineIndex );
    Coord otherCoord;

    //goLineEnd(), same line
    otherCoord.setLine( coord.line() );
    otherCoord.goLineEnd( MaxPos, coord );
    QCOMPARE( otherCoord, coord );
    // goLineEnd(), another line
    otherCoord.goDown();
    otherCoord.goLineEnd( MaxPos, coord );
    QCOMPARE( otherCoord.pos(), MaxPos );
}

void CoordTest::testCompareOperator()
{
    const Coord coord( Pos, LineIndex );
    Coord otherCoord( coord );

    otherCoord.goRight();
    QVERIFY( otherCoord>coord );
    QVERIFY( !(otherCoord<coord) );
    otherCoord.goLeft(2);
    QVERIFY( otherCoord<coord );
    QVERIFY( !(otherCoord>coord) );
    otherCoord.goRight();
    QVERIFY( !(otherCoord>coord) );
    QVERIFY( !(otherCoord<coord) );
}


void CoordTest::testIsBelowIsAbove()
{
    const Coord coord( Pos, LineIndex );
    Coord otherCoord( coord );

    otherCoord.goDown();
    QVERIFY(  otherCoord.isBelow(coord.line()) );
    QVERIFY( !otherCoord.isAbove(coord.line()) );
    otherCoord.goUp( 2 );
    QVERIFY(  otherCoord.isAbove(coord.line()) );
    QVERIFY( !otherCoord.isBelow(coord.line()) );
    otherCoord.goDown();
    QVERIFY( !otherCoord.isAbove(coord.line()) );
    QVERIFY( !otherCoord.isBelow(coord.line()) );
}

void CoordTest::testIsLaterPriorInLineThan()
{
    const Coord coord( Pos, LineIndex );
    Coord otherCoord( coord );

    otherCoord.goRight();
    QVERIFY(  otherCoord.isLaterInLineThan(coord) );
    QVERIFY( !otherCoord.isPriorInLineThan(coord) );
    otherCoord.goLeft(2);
    QVERIFY(  otherCoord.isPriorInLineThan(coord) );
    QVERIFY( !otherCoord.isLaterInLineThan(coord) );
    otherCoord.goRight();
    QVERIFY( !otherCoord.isPriorInLineThan(coord) );
    QVERIFY( !otherCoord.isLaterInLineThan(coord) );
}

void CoordTest::testIsBeforeLineStart()
{
    Coord coord( 1, LineIndex );

    QVERIFY( coord.isBehindLineStart() );
    QVERIFY( coord.isValid() );
    coord.goLeft();
    QVERIFY( !coord.isBehindLineStart() );
    QVERIFY( coord.isValid() );
}

void CoordTest::testIsBeforeLineEnd()
{
    Coord coord( MaxPos-1, LineIndex );

    QVERIFY( coord.isBeforeLineEnd(MaxPos) );
    QVERIFY( coord.isValid() );
    coord.goRight();
    QVERIFY( !coord.isBeforeLineEnd(MaxPos) );
    QVERIFY( coord.isValid() );
}


void CoordTest::testIndexNWidth()
{
    Coord coord;
    coord.setByIndexNWidth( Index, LineWidth );
    QCOMPARE( coord.indexByLineWidth(LineWidth), Index );
    QVERIFY( coord.isValid() );
}

}

QTEST_MAIN( Okteta::CoordTest )
