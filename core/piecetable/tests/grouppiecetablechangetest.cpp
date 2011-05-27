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

#include "grouppiecetablechangetest.h"

// test object
#include <piecetable/grouppiecetablechange.h>
// test
#include "testpiecetablechange.h"
#include <piecetable/piecetable.h>
//
#include <arraychangemetricslist.h>
// Qt
#include <QtTest/QtTest>


namespace KPieceTable
{

// local variables
static const char Description[] = "description";


void GroupPieceTableChangeTest::testConstructor()
{
    GroupPieceTableChange groupPieceTableChange( 0, QLatin1String(Description) );

//     QCOMPARE( groupPieceTableChange.count(), 0 );
    QCOMPARE( groupPieceTableChange.appliedChangesCount(), 0 );
    QCOMPARE( groupPieceTableChange.parent(), (GroupPieceTableChange*)0 );
    QCOMPARE( groupPieceTableChange.dataSize(), 0 );
    QCOMPARE( groupPieceTableChange.description(), QLatin1String(Description) );
}

void GroupPieceTableChangeTest::testAppendChange()
{
    GroupPieceTableChange groupPieceTableChange( 0, QLatin1String(Description) );

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
    bool result = groupPieceTableChange.appendChange( change1 );

    QVERIFY( result );
    QCOMPARE( groupPieceTableChange.count(), 1 );
    QCOMPARE( groupPieceTableChange.appliedChangesCount(), 1 );
    QCOMPARE( groupPieceTableChange.headChangeDescription(), description1 );
    QCOMPARE( groupPieceTableChange.changeDescription(0), description1 );

    // adding first
    result = groupPieceTableChange.appendChange( change2 );

    QVERIFY( result );
    QCOMPARE( groupPieceTableChange.count(), 2 );
    QCOMPARE( groupPieceTableChange.appliedChangesCount(), 2 );
    QCOMPARE( groupPieceTableChange.headChangeDescription(), description2 );
    QCOMPARE( groupPieceTableChange.changeDescription(0), description1 );
    QCOMPARE( groupPieceTableChange.changeDescription(1), description2 );

    // adding third which should be merged
    result = groupPieceTableChange.appendChange( change3 );

    QVERIFY( !result );
    QCOMPARE( groupPieceTableChange.count(), 2 );
    QCOMPARE( groupPieceTableChange.appliedChangesCount(), 2 );
    QCOMPARE( groupPieceTableChange.headChangeDescription(), QString(description2+description3) );
    QCOMPARE( groupPieceTableChange.changeDescription(0), description1 );
    QCOMPARE( groupPieceTableChange.changeDescription(1), QString(description2+description3) );

    // adding third which should not be merged as we call finishChange before
    groupPieceTableChange.finishChange();
    result = groupPieceTableChange.appendChange( change4 );

    QVERIFY( result );
    QCOMPARE( groupPieceTableChange.count(), 3 );
    QCOMPARE( groupPieceTableChange.appliedChangesCount(), 3 );
    QCOMPARE( groupPieceTableChange.headChangeDescription(), description4 );
    QCOMPARE( groupPieceTableChange.changeDescription(0), description1 );
    QCOMPARE( groupPieceTableChange.changeDescription(1), QString(description2+description3) );
    QCOMPARE( groupPieceTableChange.changeDescription(2), description4 );
}

#if 0
void GroupPieceTableChangeTest::testRevertBeforeChange()
{
    GroupPieceTableChange groupPieceTableChange;
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
    KDE::ArrayChangeMetrics changeMetrics1 = change1->metrics();
    KDE::ArrayChangeMetrics changeMetrics2 = change2->metrics();
    KDE::ArrayChangeMetrics changeMetrics3 = change3->metrics();
    KDE::ArrayChangeMetrics changeMetrics4 = change4->metrics();
    KDE::ArrayChangeMetrics revertedChangeMetrics1 = change1->metrics(); revertedChangeMetrics1.revert();
    KDE::ArrayChangeMetrics revertedChangeMetrics2 = change2->metrics(); revertedChangeMetrics2.revert();
    KDE::ArrayChangeMetrics revertedChangeMetrics3 = change3->metrics(); revertedChangeMetrics3.revert();
    KDE::ArrayChangeMetrics revertedChangeMetrics4 = change4->metrics(); revertedChangeMetrics4.revert();

    bool result = groupPieceTableChange.appendChange( change1 );
    result = groupPieceTableChange.appendChange( change2 );
    result = groupPieceTableChange.appendChange( change3 );
    result = groupPieceTableChange.appendChange( change4 );

    // revert before first change
    SectionList changedSectionList;
    ArrayChangeMetricsList changeList;
    result =
        groupPieceTableChange.revertBeforeChange( &pieceTable, 0, &changedSectionList, &changeList );

    QVERIFY( result );
    QCOMPARE( groupPieceTableChange.count(), 4 );
    QCOMPARE( groupPieceTableChange.appliedChangesCount(), 0 );
    QCOMPARE( groupPieceTableChange.headChangeDescription(), description4 );
    QCOMPARE( changedSectionList.count(), 1 );
    QCOMPARE( changedSectionList.first(), Section(1,4) );
    QCOMPARE( changeList.count(), 4 );
    QCOMPARE( changeList[0], revertedChangeMetrics4 );
    QCOMPARE( changeList[1], revertedChangeMetrics3 );
    QCOMPARE( changeList[2], revertedChangeMetrics2 );
    QCOMPARE( changeList[3], revertedChangeMetrics1 );

    // revert before last change
    changedSectionList.clear();
    changeList.clear();
    result =
        groupPieceTableChange.revertBeforeChange( &pieceTable, 3, &changedSectionList, &changeList );

    QVERIFY( result );
    QCOMPARE( groupPieceTableChange.count(), 4 );
    QCOMPARE( groupPieceTableChange.appliedChangesCount(), 3 );
    QCOMPARE( groupPieceTableChange.headChangeDescription(), description4 );
    QCOMPARE( changedSectionList.count(), 1 );
    QCOMPARE( changedSectionList.first(), Section(1,3) );
    QCOMPARE( changeList.count(), 3 );
    QCOMPARE( changeList[0], changeMetrics1 );
    QCOMPARE( changeList[1], changeMetrics2 );
    QCOMPARE( changeList[2], changeMetrics3 );

    // revert after first change
    changedSectionList.clear();
    changeList.clear();
    result =
        groupPieceTableChange.revertBeforeChange( &pieceTable, 1, &changedSectionList, &changeList );

    QVERIFY( result );
    QCOMPARE( groupPieceTableChange.count(), 4 );
    QCOMPARE( groupPieceTableChange.appliedChangesCount(), 1 );
    QCOMPARE( groupPieceTableChange.headChangeDescription(), description4 );
    QCOMPARE( changedSectionList.count(), 1 );
    QCOMPARE( changedSectionList.first(), Section(2,3) );
    QCOMPARE( changeList.count(), 2 );
    QCOMPARE( changeList[0], revertedChangeMetrics3 );
    QCOMPARE( changeList[1], revertedChangeMetrics2 );

    // revert after last change
    changedSectionList.clear();
    changeList.clear();
    result =
        groupPieceTableChange.revertBeforeChange( &pieceTable, 4, &changedSectionList, &changeList );

    QVERIFY( result );
    QCOMPARE( groupPieceTableChange.count(), 4 );
    QCOMPARE( groupPieceTableChange.appliedChangesCount(), 4 );
    QCOMPARE( groupPieceTableChange.headChangeDescription(), description4 );
    QCOMPARE( changedSectionList.count(), 1 );
    QCOMPARE( changedSectionList.first(), Section(2,4) );
    QCOMPARE( changeList.count(), 3 );
    QCOMPARE( changeList[0], changeMetrics2 );
    QCOMPARE( changeList[1], changeMetrics3 );
    QCOMPARE( changeList[2], changeMetrics4 );
}
#endif
}

QTEST_MAIN( KPieceTable::GroupPieceTableChangeTest )
