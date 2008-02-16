/***************************************************************************
                          piecetablechangehistorytest.cpp  -  description
                             -------------------
    begin                : Fri Jan 18 2008
    copyright            : 2008 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License version 2 as published by the Free Software Foundation.       *
 *                                                                         *
 ***************************************************************************/


#include "piecetablechangehistorytest.h"

// test object
#include <piecetable/piecetablechangehistory.h>
// test
#include "testpiecetablechange.h"
#include <piecetable/piecetable.h>
//
#include <ksectionlist.h>
#include <kreplacementscope.h>
// Qt
#include <QtTest/QtTest>

#include <KDebug>

using namespace KHE;

// local variables
static const int Start = 15;
static const int End = 27;
static const int HalfSize = 50;
static const int Size = 2 * HalfSize;

static const int Width = End - Start + 1;

static const int ChangeStart = 8;
static const int ChangeEnd = ChangeStart + Width - 1;

namespace KPieceTable
{

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
    const QString description1( "1" );
    const QString description2( "2" );
    const QString description3( "3" );
    const QString description4( "4" );
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
    QCOMPARE( pieceTableChangeHistory.headChangeDescription(), description2+description3 );
    QCOMPARE( pieceTableChangeHistory.changeDescription(0), description1 );
    QCOMPARE( pieceTableChangeHistory.changeDescription(1), description2+description3 );

    // adding third which should not be merged as we call finishChange before
    pieceTableChangeHistory.finishChange();
    result = pieceTableChangeHistory.appendChange( change4 );

    QVERIFY( result );
    QCOMPARE( pieceTableChangeHistory.count(), 3 );
    QCOMPARE( pieceTableChangeHistory.appliedChangesCount(), 3 );
    QCOMPARE( pieceTableChangeHistory.headChangeDescription(), description4 );
    QCOMPARE( pieceTableChangeHistory.changeDescription(0), description1 );
    QCOMPARE( pieceTableChangeHistory.changeDescription(1), description2+description3 );
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
    const QString description1( "1" );
    const QString description2( "2" );
    const QString description3( "3" );
    const QString description4( "4" );
    TestPieceTableChange *change1 = new TestPieceTableChange( type1Id, description1, 1 );
    TestPieceTableChange *change2 = new TestPieceTableChange( type2Id, description2, 2 );
    TestPieceTableChange *change3 = new TestPieceTableChange( type3Id, description3, 3 );
    TestPieceTableChange *change4 = new TestPieceTableChange( type4Id, description4, 4 );

    bool result = pieceTableChangeHistory.appendChange( change1 );
    result = pieceTableChangeHistory.appendChange( change2 );
    result = pieceTableChangeHistory.appendChange( change3 );
    result = pieceTableChangeHistory.appendChange( change4 );

    // revert before first change
    KSectionList changedSectionList;
    QList<ReplacementScope> replacementList;
    result =
        pieceTableChangeHistory.revertBeforeChange( &pieceTable, 0, &changedSectionList, &replacementList );

    QVERIFY( result );
    QCOMPARE( pieceTableChangeHistory.count(), 4 );
    QCOMPARE( pieceTableChangeHistory.appliedChangesCount(), 0 );
    QCOMPARE( pieceTableChangeHistory.headChangeDescription(), description4 );
    QCOMPARE( changedSectionList.count(), 1 );
    QCOMPARE( changedSectionList.first(), KSection(1,4) );

    // revert before last change
    changedSectionList.clear();
    replacementList.clear();
    result =
        pieceTableChangeHistory.revertBeforeChange( &pieceTable, 3, &changedSectionList, &replacementList );

    QVERIFY( result );
    QCOMPARE( pieceTableChangeHistory.count(), 4 );
    QCOMPARE( pieceTableChangeHistory.appliedChangesCount(), 3 );
    QCOMPARE( pieceTableChangeHistory.headChangeDescription(), description4 );
    QCOMPARE( changedSectionList.count(), 1 );
    QCOMPARE( changedSectionList.first(), KSection(1,3) );

    // revert after first change
    changedSectionList.clear();
    replacementList.clear();
    result =
        pieceTableChangeHistory.revertBeforeChange( &pieceTable, 1, &changedSectionList, &replacementList );

    QVERIFY( result );
    QCOMPARE( pieceTableChangeHistory.count(), 4 );
    QCOMPARE( pieceTableChangeHistory.appliedChangesCount(), 1 );
    QCOMPARE( pieceTableChangeHistory.headChangeDescription(), description4 );
    QCOMPARE( changedSectionList.count(), 1 );
    QCOMPARE( changedSectionList.first(), KSection(2,3) );

    // revert after last change
    changedSectionList.clear();
    replacementList.clear();
    result =
        pieceTableChangeHistory.revertBeforeChange( &pieceTable, 4, &changedSectionList, &replacementList );

    QVERIFY( result );
    QCOMPARE( pieceTableChangeHistory.count(), 4 );
    QCOMPARE( pieceTableChangeHistory.appliedChangesCount(), 4 );
    QCOMPARE( pieceTableChangeHistory.headChangeDescription(), description4 );
    QCOMPARE( changedSectionList.count(), 1 );
    QCOMPARE( changedSectionList.first(), KSection(2,4) );
}

}

QTEST_MAIN( KPieceTable::PieceTableChangeHistoryTest )
