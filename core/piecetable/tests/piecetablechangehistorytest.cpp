/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "piecetablechangehistorytest.hpp"

// test object
#include <piecetable/piecetablechangehistory.hpp>
// test
#include "testpiecetablechange.hpp"
#include <piecetable/piecetable.hpp>
//
#include <arraychangemetricslist.hpp>
#include <addressrangelist.hpp>
// Qt
#include <QTest>

namespace KPieceTable {

void PieceTableChangeHistoryTest::testSimpleConstructor()
{
    PieceTableChangeHistory pieceTableChangeHistory;

    QCOMPARE(pieceTableChangeHistory.count(), 0);
    QCOMPARE(pieceTableChangeHistory.appliedChangesCount(), 0);
}

void PieceTableChangeHistoryTest::testAppendChange()
{
    PieceTableChangeHistory pieceTableChangeHistory;

    const int type1Id = 1;
    const int type2Id = 2;
    const int type3Id = type2Id;
    const int type4Id = 4;
    const QString description1 = QStringLiteral("1");
    const QString description2 = QStringLiteral("2");
    const QString description3 = QStringLiteral("3");
    const QString description4 = QStringLiteral("4");
    auto* change1 = new TestPieceTableChange(type1Id, description1);
    auto* change2 = new TestPieceTableChange(type2Id, description2);
    auto* change3 = new TestPieceTableChange(type3Id, description3);
    auto* change4 = new TestPieceTableChange(type4Id, description4);

    // adding first
    bool result = pieceTableChangeHistory.appendChange(change1);

    QVERIFY(result);
    QCOMPARE(pieceTableChangeHistory.count(), 1);
    QCOMPARE(pieceTableChangeHistory.appliedChangesCount(), 1);
    QCOMPARE(pieceTableChangeHistory.headChangeDescription(), description1);
    QCOMPARE(pieceTableChangeHistory.changeDescription(0), description1);

    // adding first
    result = pieceTableChangeHistory.appendChange(change2);

    QVERIFY(result);
    QCOMPARE(pieceTableChangeHistory.count(), 2);
    QCOMPARE(pieceTableChangeHistory.appliedChangesCount(), 2);
    QCOMPARE(pieceTableChangeHistory.headChangeDescription(), description2);
    QCOMPARE(pieceTableChangeHistory.changeDescription(0), description1);
    QCOMPARE(pieceTableChangeHistory.changeDescription(1), description2);

    // adding third which should be merged
    result = pieceTableChangeHistory.appendChange(change3);

    QVERIFY(!result);
    QCOMPARE(pieceTableChangeHistory.count(), 2);
    QCOMPARE(pieceTableChangeHistory.appliedChangesCount(), 2);
    QCOMPARE(pieceTableChangeHistory.headChangeDescription(), QString(description2 + description3));
    QCOMPARE(pieceTableChangeHistory.changeDescription(0), description1);
    QCOMPARE(pieceTableChangeHistory.changeDescription(1), QString(description2 + description3));

    // adding third which should not be merged as we call finishChange before
    pieceTableChangeHistory.finishChange();
    result = pieceTableChangeHistory.appendChange(change4);

    QVERIFY(result);
    QCOMPARE(pieceTableChangeHistory.count(), 3);
    QCOMPARE(pieceTableChangeHistory.appliedChangesCount(), 3);
    QCOMPARE(pieceTableChangeHistory.headChangeDescription(), description4);
    QCOMPARE(pieceTableChangeHistory.changeDescription(0), description1);
    QCOMPARE(pieceTableChangeHistory.changeDescription(1), QString(description2 + description3));
    QCOMPARE(pieceTableChangeHistory.changeDescription(2), description4);
}

void PieceTableChangeHistoryTest::testRevertBeforeChange()
{
    PieceTableChangeHistory pieceTableChangeHistory;
    PieceTable pieceTable;
    pieceTable.init(0);

    const int type1Id = 1;
    const int type2Id = 2;
    const int type3Id = 3;
    const int type4Id = 4;
    const QString description1 = QStringLiteral("1");
    const QString description2 = QStringLiteral("2");
    const QString description3 = QStringLiteral("3");
    const QString description4 = QStringLiteral("4");
    auto* change1 = new TestPieceTableChange(type1Id, description1, 1);
    auto* change2 = new TestPieceTableChange(type2Id, description2, 2);
    auto* change3 = new TestPieceTableChange(type3Id, description3, 3);
    auto* change4 = new TestPieceTableChange(type4Id, description4, 4);
    ArrayChangeMetrics changeMetrics1 = change1->metrics();
    ArrayChangeMetrics changeMetrics2 = change2->metrics();
    ArrayChangeMetrics changeMetrics3 = change3->metrics();
    ArrayChangeMetrics changeMetrics4 = change4->metrics();
    ArrayChangeMetrics revertedChangeMetrics1 = change1->metrics(); revertedChangeMetrics1.revert();
    ArrayChangeMetrics revertedChangeMetrics2 = change2->metrics(); revertedChangeMetrics2.revert();
    ArrayChangeMetrics revertedChangeMetrics3 = change3->metrics(); revertedChangeMetrics3.revert();
    ArrayChangeMetrics revertedChangeMetrics4 = change4->metrics(); revertedChangeMetrics4.revert();

    bool result = pieceTableChangeHistory.appendChange(change1);
    result = pieceTableChangeHistory.appendChange(change2);
    result = pieceTableChangeHistory.appendChange(change3);
    result = pieceTableChangeHistory.appendChange(change4);

    // revert before first change
    AddressRangeList changedSectionList;
    ArrayChangeMetricsList changeList;
    result =
        pieceTableChangeHistory.revertBeforeChange(&pieceTable, 0, &changedSectionList, &changeList);

    QVERIFY(result);
    QCOMPARE(pieceTableChangeHistory.count(), 4);
    QCOMPARE(pieceTableChangeHistory.appliedChangesCount(), 0);
    QCOMPARE(pieceTableChangeHistory.headChangeDescription(), description4);
    QCOMPARE(changedSectionList.count(), 1);
    QCOMPARE(changedSectionList.first(), AddressRange(1, 4));
    QCOMPARE(changeList.count(), 4);
    QCOMPARE(changeList[0], revertedChangeMetrics4);
    QCOMPARE(changeList[1], revertedChangeMetrics3);
    QCOMPARE(changeList[2], revertedChangeMetrics2);
    QCOMPARE(changeList[3], revertedChangeMetrics1);

    // revert before last change
    changedSectionList.clear();
    changeList.clear();
    result =
        pieceTableChangeHistory.revertBeforeChange(&pieceTable, 3, &changedSectionList, &changeList);

    QVERIFY(result);
    QCOMPARE(pieceTableChangeHistory.count(), 4);
    QCOMPARE(pieceTableChangeHistory.appliedChangesCount(), 3);
    QCOMPARE(pieceTableChangeHistory.headChangeDescription(), description4);
    QCOMPARE(changedSectionList.count(), 1);
    QCOMPARE(changedSectionList.first(), AddressRange(1, 3));
    QCOMPARE(changeList.count(), 3);
    QCOMPARE(changeList[0], changeMetrics1);
    QCOMPARE(changeList[1], changeMetrics2);
    QCOMPARE(changeList[2], changeMetrics3);

    // revert after first change
    changedSectionList.clear();
    changeList.clear();
    result =
        pieceTableChangeHistory.revertBeforeChange(&pieceTable, 1, &changedSectionList, &changeList);

    QVERIFY(result);
    QCOMPARE(pieceTableChangeHistory.count(), 4);
    QCOMPARE(pieceTableChangeHistory.appliedChangesCount(), 1);
    QCOMPARE(pieceTableChangeHistory.headChangeDescription(), description4);
    QCOMPARE(changedSectionList.count(), 1);
    QCOMPARE(changedSectionList.first(), AddressRange(2, 3));
    QCOMPARE(changeList.count(), 2);
    QCOMPARE(changeList[0], revertedChangeMetrics3);
    QCOMPARE(changeList[1], revertedChangeMetrics2);

    // revert after last change
    changedSectionList.clear();
    changeList.clear();
    result =
        pieceTableChangeHistory.revertBeforeChange(&pieceTable, 4, &changedSectionList, &changeList);

    QVERIFY(result);
    QCOMPARE(pieceTableChangeHistory.count(), 4);
    QCOMPARE(pieceTableChangeHistory.appliedChangesCount(), 4);
    QCOMPARE(pieceTableChangeHistory.headChangeDescription(), description4);
    QCOMPARE(changedSectionList.count(), 1);
    QCOMPARE(changedSectionList.first(), AddressRange(2, 4));
    QCOMPARE(changeList.count(), 3);
    QCOMPARE(changeList[0], changeMetrics2);
    QCOMPARE(changeList[1], changeMetrics3);
    QCOMPARE(changeList[2], changeMetrics4);
}

}

QTEST_GUILESS_MAIN(KPieceTable::PieceTableChangeHistoryTest)
