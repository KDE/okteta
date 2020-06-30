/*
    This file is part of the Okteta Gui library, made within the KDE community.

    SPDX-FileCopyrightText: 2006 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "coordrangelisttest.hpp"

// test object
#include <coordrangelist.hpp>
// Qt
#include <QTest>

namespace Okteta {

// local variables
static constexpr LinePosition Pos1 = 15;
static constexpr LinePosition Pos2 = 25;
static constexpr Line Line1 = 10;
static constexpr LineSize LineCount = 10;
static constexpr Line Line2 = Line1 + LineCount - 1;
static constexpr LineSize LineDistance = 5;
static constexpr Line Line3 = Line2 + LineDistance;
static constexpr Line Line4 = Line3 + LineCount - 1;

static constexpr Coord Start1(Pos1, Line1);
static constexpr Coord End1(Pos2, Line2);
static constexpr Coord Start2(Pos1, Line3);
static constexpr Coord End2(Pos2, Line4);

void CoordRangeListTest::testSimpleConstructor()
{
    CoordRangeList CoordRangeList;
    QVERIFY(CoordRangeList.isEmpty());
}

void CoordRangeListTest::testAddSingleSection()
{
    CoordRangeList CoordRangeList;

    CoordRange CoordRange(Start1, End1);
    CoordRangeList.addCoordRange(CoordRange);
    QCOMPARE(CoordRangeList.size(), 1);
    QCOMPARE(CoordRangeList.first(), CoordRange);
}

void CoordRangeListTest::testAddJoinableSections()
{
    CoordRangeList CoordRangeList;

    CoordRange CoordRange1(Start1, End1);
    CoordRange CoordRange2(Start2, End2);
    // in front
    Coord OtherStart(0, 0);
    Coord OtherEnd = Start1;
    OtherEnd.goLeft(0);
    CoordRange OtherCoordRange(OtherStart, OtherEnd);
    CoordRangeList.addCoordRange(CoordRange1);
    CoordRangeList.addCoordRange(CoordRange2);
    CoordRangeList.addCoordRange(OtherCoordRange);

    QCOMPARE(CoordRangeList.size(), 2);
    QCOMPARE(CoordRangeList.first(), CoordRange(OtherCoordRange.start(), CoordRange1.end()));
    QCOMPARE(CoordRangeList.last(), CoordRange2);

    // in the middle, with first
    CoordRangeList.clear();
    OtherStart = End1;
    OtherStart.goRight(0);
    OtherEnd = Start2;
    OtherEnd.goLeft(1);
    OtherCoordRange.set(OtherStart, OtherEnd);
    CoordRangeList.addCoordRange(CoordRange1);
    CoordRangeList.addCoordRange(CoordRange2);
    CoordRangeList.addCoordRange(OtherCoordRange);

    QCOMPARE(CoordRangeList.size(), 2);
    QCOMPARE(CoordRangeList.first(), CoordRange(CoordRange1.start(), OtherCoordRange.end()));
    QCOMPARE(CoordRangeList.last(), CoordRange2);

    // in the middle, with both
    CoordRangeList.clear();
    OtherEnd.goRight(1);
    OtherCoordRange.setEnd(OtherEnd);
    CoordRangeList.addCoordRange(CoordRange1);
    CoordRangeList.addCoordRange(CoordRange2);
    CoordRangeList.addCoordRange(OtherCoordRange);

    QCOMPARE(CoordRangeList.size(), 1);
    QCOMPARE(CoordRangeList.first(), CoordRange(CoordRange1.start(), CoordRange2.end()));

    // in the middle, with last
    CoordRangeList.clear();
    OtherStart.goRight(1);
    OtherCoordRange.setStart(OtherStart);
    CoordRangeList.addCoordRange(CoordRange1);
    CoordRangeList.addCoordRange(CoordRange2);
    CoordRangeList.addCoordRange(OtherCoordRange);

    QCOMPARE(CoordRangeList.size(), 2);
    QCOMPARE(CoordRangeList.first(), CoordRange1);
    QCOMPARE(CoordRangeList.last(), CoordRange(OtherCoordRange.start(), CoordRange2.end()));

    // behind
    CoordRangeList.clear();
    OtherStart = End2;
    OtherStart.goRight(0);
    OtherEnd = OtherStart;
    OtherEnd.goDown(LineCount);
    OtherCoordRange.set(OtherStart, OtherEnd);
    CoordRangeList.addCoordRange(CoordRange1);
    CoordRangeList.addCoordRange(CoordRange2);
    CoordRangeList.addCoordRange(OtherCoordRange);

    QCOMPARE(CoordRangeList.size(), 2);
    QCOMPARE(CoordRangeList.first(), CoordRange1);
    QCOMPARE(CoordRangeList.last(), CoordRange(CoordRange2.start(), OtherCoordRange.end()));
}

void CoordRangeListTest::testAddNonJoinableSections()
{
    CoordRangeList CoordRangeList;

    CoordRange CoordRange1(Start1, End1);
    CoordRange CoordRange2(Start2, End2);
    // in front
    Coord OtherStart(0, 0);
    Coord OtherEnd = Start1;
    OtherEnd.goLeft(2);
    CoordRange OtherCoordRange(OtherStart, OtherEnd);
    CoordRangeList.addCoordRange(CoordRange1);
    CoordRangeList.addCoordRange(CoordRange2);
    CoordRangeList.addCoordRange(OtherCoordRange);

    QCOMPARE(CoordRangeList.size(), 3);
    QCOMPARE(CoordRangeList.first(), OtherCoordRange);
    QCOMPARE(CoordRangeList.last(), CoordRange2);

    // in the middle
    CoordRangeList.clear();
    OtherStart = End1;
    OtherStart.goRight(2);
    OtherEnd = Start2;
    OtherEnd.goLeft(2);
    OtherCoordRange.set(OtherStart, OtherEnd);
    CoordRangeList.addCoordRange(CoordRange1);
    CoordRangeList.addCoordRange(CoordRange2);
    CoordRangeList.addCoordRange(OtherCoordRange);

    QCOMPARE(CoordRangeList.size(), 3);
    QCOMPARE(CoordRangeList.first(), CoordRange1);
    QCOMPARE(CoordRangeList.last(), CoordRange2);

    // behind
    CoordRangeList.clear();
    OtherStart = End2;
    OtherStart.goRight(2);
    OtherEnd = OtherStart;
    OtherEnd.goDown(LineCount);
    OtherCoordRange.set(OtherStart, OtherEnd);
    CoordRangeList.addCoordRange(CoordRange1);
    CoordRangeList.addCoordRange(CoordRange2);
    CoordRangeList.addCoordRange(OtherCoordRange);

    QCOMPARE(CoordRangeList.size(), 3);
    QCOMPARE(CoordRangeList.first(), CoordRange1);
    QCOMPARE(CoordRangeList.last(), OtherCoordRange);
}

}

QTEST_MAIN(Okteta::CoordRangeListTest)
