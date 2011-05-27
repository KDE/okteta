/*
    This file is part of the Okteta Core library, made within the KDE community.

    Copyright 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "piecetablechangehistorytest.h"

// test object
#include <piecetable/piecetablechangehistory.h>
// test
#include "testpiecetablechange.h"
#include <piecetable/piecetable.h>
//
#include <arraychangemetricslist.h>
#include <addressrangelist.h>
// Qt
#include <QtTest/QtTest>


namespace KPieceTable
{

// local variables
static const Address Start = 15;
static const Address End = 27;

static const Size Width = End - Start + 1;

static const Address ChangeStart = 8;
static const Address ChangeEnd = ChangeStart + Width - 1;


void PieceTableChangeHistoryTest::testSimpleConstructor()
{
    PieceTableChangeHistory pieceTableChangeHistory;

    QCOMPARE( pieceTableChangeHistory.count(), 0 );
    QCOMPARE( pieceTableChangeHistory.appliedChangesCount(), 0 );
}

void PieceTableChangeHistoryTest::testAppendChange()
{
    PieceTableChangeHistory pieceTableChangeHistory;

    const int type1Id = 1;
    const int type2Id = 2;
    const int type3Id = type2Id;
    const int type4Id = 4;
    const QString description1 = QLatin1String( "1" );
    const QString description2 = QLatin1String( "2" );
    const QString description3 = QLatin1String( "3" );
    const QString description4 = QLatin1String( "4" );
    TestPieceTableChange *change1 = new TestPieceTableChange( type1Id, description1 );
    TestPieceTableChange *change2 = new TestPieceTableChange( type2Id, description2 );
    TestPieceTableChange *change3 = new TestPieceTableChange( type3Id, description3 );
    TestPieceTableChange *change4 = new TestPieceTableChange( type4Id, description4 );

    // adding first
    bool result = pieceTableChangeHistory.appendChange( change1 );

    QVERIFY( result );
    QCOMPARE( pieceTableChangeHistory.count(), 1 );
    QCOMPARE( pieceTableChangeHistory.appliedChangesCount(), 1 );
    QCOMPARE( pieceTableChangeHistory.headChangeDescription(), description1 );
    QCOMPARE( pieceTableChangeHistory.changeDescription(0), description1 );

    // adding first
    result = pieceTableChangeHistory.appendChange( change2 );

    QVERIFY( result );
    QCOMPARE( pieceTableChangeHistory.count(), 2 );
    QCOMPARE( pieceTableChangeHistory.appliedChangesCount(), 2 );
    QCOMPARE( pieceTableChangeHistory.headChangeDescription(), description2 );
    QCOMPARE( pieceTableChangeHistory.changeDescription(0), description1 );
    QCOMPARE( pieceTableChangeHistory.changeDescription(1), description2 );

    // adding third which should be merged
    result = pieceTableChangeHistory.appendChange( change3 );

    QVERIFY( !result );
    QCOMPARE( pieceTableChangeHistory.count(), 2 );
    QCOMPARE( pieceTableChangeHistory.appliedChangesCount(), 2 );
    QCOMPARE( pieceTableChangeHistory.headChangeDescription(), QString(description2+description3) );
    QCOMPARE( pieceTableChangeHistory.changeDescription(0), description1 );
    QCOMPARE( pieceTableChangeHistory.changeDescription(1), QString(description2+description3) );

    // adding third which should not be merged as we call finishChange before
    pieceTableChangeHistory.finishChange();
    result = pieceTableChangeHistory.appendChange( change4 );

    QVERIFY( result );
    QCOMPARE( pieceTableChangeHistory.count(), 3 );
    QCOMPARE( pieceTableChangeHistory.appliedChangesCount(), 3 );
    QCOMPARE( pieceTableChangeHistory.headChangeDescription(), description4 );
    QCOMPARE( pieceTableChangeHistory.changeDescription(0), description1 );
    QCOMPARE( pieceTableChangeHistory.changeDescription(1), QString(description2+description3) );
    QCOMPARE( pieceTableChangeHistory.changeDescription(2), description4 );
}


void PieceTableChangeHistoryTest::testRevertBeforeChange()
{
    PieceTableChangeHistory pieceTableChangeHistory;
    PieceTable pieceTable;
    pieceTable.init( 0 );

    const int type1Id = 1;
    const int type2Id = 2;
    const int type3Id = 3;
    const int type4Id = 4;
    const QString description1 = QLatin1String( "1" );
    const QString description2 = QLatin1String( "2" );
    const QString description3 = QLatin1String( "3" );
    const QString description4 = QLatin1String( "4" );
    TestPieceTableChange *change1 = new TestPieceTableChange( type1Id, description1, 1 );
    TestPieceTableChange *change2 = new TestPieceTableChange( type2Id, description2, 2 );
    TestPieceTableChange *change3 = new TestPieceTableChange( type3Id, description3, 3 );
    TestPieceTableChange *change4 = new TestPieceTableChange( type4Id, description4, 4 );
    ArrayChangeMetrics changeMetrics1 = change1->metrics();
    ArrayChangeMetrics changeMetrics2 = change2->metrics();
    ArrayChangeMetrics changeMetrics3 = change3->metrics();
    ArrayChangeMetrics changeMetrics4 = change4->metrics();
    ArrayChangeMetrics revertedChangeMetrics1 = change1->metrics(); revertedChangeMetrics1.revert();
    ArrayChangeMetrics revertedChangeMetrics2 = change2->metrics(); revertedChangeMetrics2.revert();
    ArrayChangeMetrics revertedChangeMetrics3 = change3->metrics(); revertedChangeMetrics3.revert();
    ArrayChangeMetrics revertedChangeMetrics4 = change4->metrics(); revertedChangeMetrics4.revert();

    bool result = pieceTableChangeHistory.appendChange( change1 );
    result = pieceTableChangeHistory.appendChange( change2 );
    result = pieceTableChangeHistory.appendChange( change3 );
    result = pieceTableChangeHistory.appendChange( change4 );

    // revert before first change
    AddressRangeList changedSectionList;
    ArrayChangeMetricsList changeList;
    result =
        pieceTableChangeHistory.revertBeforeChange( &pieceTable, 0, &changedSectionList, &changeList );

    QVERIFY( result );
    QCOMPARE( pieceTableChangeHistory.count(), 4 );
    QCOMPARE( pieceTableChangeHistory.appliedChangesCount(), 0 );
    QCOMPARE( pieceTableChangeHistory.headChangeDescription(), description4 );
    QCOMPARE( changedSectionList.count(), 1 );
    QCOMPARE( changedSectionList.first(), AddressRange(1,4) );
    QCOMPARE( changeList.count(), 4 );
    QCOMPARE( changeList[0], revertedChangeMetrics4 );
    QCOMPARE( changeList[1], revertedChangeMetrics3 );
    QCOMPARE( changeList[2], revertedChangeMetrics2 );
    QCOMPARE( changeList[3], revertedChangeMetrics1 );

    // revert before last change
    changedSectionList.clear();
    changeList.clear();
    result =
        pieceTableChangeHistory.revertBeforeChange( &pieceTable, 3, &changedSectionList, &changeList );

    QVERIFY( result );
    QCOMPARE( pieceTableChangeHistory.count(), 4 );
    QCOMPARE( pieceTableChangeHistory.appliedChangesCount(), 3 );
    QCOMPARE( pieceTableChangeHistory.headChangeDescription(), description4 );
    QCOMPARE( changedSectionList.count(), 1 );
    QCOMPARE( changedSectionList.first(), AddressRange(1,3) );
    QCOMPARE( changeList.count(), 3 );
    QCOMPARE( changeList[0], changeMetrics1 );
    QCOMPARE( changeList[1], changeMetrics2 );
    QCOMPARE( changeList[2], changeMetrics3 );

    // revert after first change
    changedSectionList.clear();
    changeList.clear();
    result =
        pieceTableChangeHistory.revertBeforeChange( &pieceTable, 1, &changedSectionList, &changeList );

    QVERIFY( result );
    QCOMPARE( pieceTableChangeHistory.count(), 4 );
    QCOMPARE( pieceTableChangeHistory.appliedChangesCount(), 1 );
    QCOMPARE( pieceTableChangeHistory.headChangeDescription(), description4 );
    QCOMPARE( changedSectionList.count(), 1 );
    QCOMPARE( changedSectionList.first(), AddressRange(2,3) );
    QCOMPARE( changeList.count(), 2 );
    QCOMPARE( changeList[0], revertedChangeMetrics3 );
    QCOMPARE( changeList[1], revertedChangeMetrics2 );

    // revert after last change
    changedSectionList.clear();
    changeList.clear();
    result =
        pieceTableChangeHistory.revertBeforeChange( &pieceTable, 4, &changedSectionList, &changeList );

    QVERIFY( result );
    QCOMPARE( pieceTableChangeHistory.count(), 4 );
    QCOMPARE( pieceTableChangeHistory.appliedChangesCount(), 4 );
    QCOMPARE( pieceTableChangeHistory.headChangeDescription(), description4 );
    QCOMPARE( changedSectionList.count(), 1 );
    QCOMPARE( changedSectionList.first(), AddressRange(2,4) );
    QCOMPARE( changeList.count(), 3 );
    QCOMPARE( changeList[0], changeMetrics2 );
    QCOMPARE( changeList[1], changeMetrics3 );
    QCOMPARE( changeList[2], changeMetrics4 );
}

}

QTEST_MAIN( KPieceTable::PieceTableChangeHistoryTest )
