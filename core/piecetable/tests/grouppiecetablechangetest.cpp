/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "grouppiecetablechangetest.hpp"

// test object
#include <piecetable/grouppiecetablechange.hpp>
// test
#include "testpiecetablechange.hpp"
#include <piecetable/piecetable.hpp>
//
#include <arraychangemetricslist.hpp>
// Qt
#include <QTest>

namespace KPieceTable {

// local variables
// TODO QStringLiteral
static constexpr char Description[] = "description";

void GroupPieceTableChangeTest::testConstructor()
{
    GroupPieceTableChange groupPieceTableChange(nullptr, QLatin1String(Description));

//     QCOMPARE( groupPieceTableChange.count(), 0 );
    QCOMPARE(groupPieceTableChange.appliedChangesCount(), 0);
    QCOMPARE(groupPieceTableChange.parent(), (GroupPieceTableChange*)nullptr);
    QCOMPARE(groupPieceTableChange.dataSize(), 0);
    QCOMPARE(groupPieceTableChange.description(), QLatin1String(Description));
}

void GroupPieceTableChangeTest::testAppendChange()
{
    GroupPieceTableChange groupPieceTableChange(nullptr, QLatin1String(Description));

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
    bool result = groupPieceTableChange.appendChange(change1);

    QVERIFY(result);
    QCOMPARE(groupPieceTableChange.count(), 1);
    QCOMPARE(groupPieceTableChange.appliedChangesCount(), 1);
    QCOMPARE(groupPieceTableChange.headChangeDescription(), description1);
    QCOMPARE(groupPieceTableChange.changeDescription(0), description1);

    // adding first
    result = groupPieceTableChange.appendChange(change2);

    QVERIFY(result);
    QCOMPARE(groupPieceTableChange.count(), 2);
    QCOMPARE(groupPieceTableChange.appliedChangesCount(), 2);
    QCOMPARE(groupPieceTableChange.headChangeDescription(), description2);
    QCOMPARE(groupPieceTableChange.changeDescription(0), description1);
    QCOMPARE(groupPieceTableChange.changeDescription(1), description2);

    // adding third which should be merged
    result = groupPieceTableChange.appendChange(change3);

    QVERIFY(!result);
    QCOMPARE(groupPieceTableChange.count(), 2);
    QCOMPARE(groupPieceTableChange.appliedChangesCount(), 2);
    QCOMPARE(groupPieceTableChange.headChangeDescription(), QString(description2 + description3));
    QCOMPARE(groupPieceTableChange.changeDescription(0), description1);
    QCOMPARE(groupPieceTableChange.changeDescription(1), QString(description2 + description3));

    // adding third which should not be merged as we call finishChange before
    groupPieceTableChange.finishChange();
    result = groupPieceTableChange.appendChange(change4);

    QVERIFY(result);
    QCOMPARE(groupPieceTableChange.count(), 3);
    QCOMPARE(groupPieceTableChange.appliedChangesCount(), 3);
    QCOMPARE(groupPieceTableChange.headChangeDescription(), description4);
    QCOMPARE(groupPieceTableChange.changeDescription(0), description1);
    QCOMPARE(groupPieceTableChange.changeDescription(1), QString(description2 + description3));
    QCOMPARE(groupPieceTableChange.changeDescription(2), description4);
}

#if 0
void GroupPieceTableChangeTest::testRevertBeforeChange()
{
    GroupPieceTableChange groupPieceTableChange;
    PieceTable pieceTable;
    pieceTable.init(0);

    const int type1Id = 1;
    const int type2Id = 2;
    const int type3Id = 3;
    const int type4Id = 4;
    const QString description1("1");
    const QString description2("2");
    const QString description3("3");
    const QString description4("4");
    TestPieceTableChange* change1 = new TestPieceTableChange(type1Id, description1, 1);
    TestPieceTableChange* change2 = new TestPieceTableChange(type2Id, description2, 2);
    TestPieceTableChange* change3 = new TestPieceTableChange(type3Id, description3, 3);
    TestPieceTableChange* change4 = new TestPieceTableChange(type4Id, description4, 4);
    Okteta::ArrayChangeMetrics changeMetrics1 = change1->metrics();
    Okteta::ArrayChangeMetrics changeMetrics2 = change2->metrics();
    Okteta::ArrayChangeMetrics changeMetrics3 = change3->metrics();
    Okteta::ArrayChangeMetrics changeMetrics4 = change4->metrics();
    Okteta::ArrayChangeMetrics revertedChangeMetrics1 = change1->metrics(); revertedChangeMetrics1.revert();
    Okteta::ArrayChangeMetrics revertedChangeMetrics2 = change2->metrics(); revertedChangeMetrics2.revert();
    Okteta::ArrayChangeMetrics revertedChangeMetrics3 = change3->metrics(); revertedChangeMetrics3.revert();
    Okteta::ArrayChangeMetrics revertedChangeMetrics4 = change4->metrics(); revertedChangeMetrics4.revert();

    bool result = groupPieceTableChange.appendChange(change1);
    result = groupPieceTableChange.appendChange(change2);
    result = groupPieceTableChange.appendChange(change3);
    result = groupPieceTableChange.appendChange(change4);

    // revert before first change
    SectionList changedSectionList;
    ArrayChangeMetricsList changeList;
    result =
        groupPieceTableChange.revertBeforeChange(&pieceTable, 0, &changedSectionList, &changeList);

    QVERIFY(result);
    QCOMPARE(groupPieceTableChange.count(), 4);
    QCOMPARE(groupPieceTableChange.appliedChangesCount(), 0);
    QCOMPARE(groupPieceTableChange.headChangeDescription(), description4);
    QCOMPARE(changedSectionList.count(), 1);
    QCOMPARE(changedSectionList.first(), Section(1, 4));
    QCOMPARE(changeList.count(), 4);
    QCOMPARE(changeList[0], revertedChangeMetrics4);
    QCOMPARE(changeList[1], revertedChangeMetrics3);
    QCOMPARE(changeList[2], revertedChangeMetrics2);
    QCOMPARE(changeList[3], revertedChangeMetrics1);

    // revert before last change
    changedSectionList.clear();
    changeList.clear();
    result =
        groupPieceTableChange.revertBeforeChange(&pieceTable, 3, &changedSectionList, &changeList);

    QVERIFY(result);
    QCOMPARE(groupPieceTableChange.count(), 4);
    QCOMPARE(groupPieceTableChange.appliedChangesCount(), 3);
    QCOMPARE(groupPieceTableChange.headChangeDescription(), description4);
    QCOMPARE(changedSectionList.count(), 1);
    QCOMPARE(changedSectionList.first(), Section(1, 3));
    QCOMPARE(changeList.count(), 3);
    QCOMPARE(changeList[0], changeMetrics1);
    QCOMPARE(changeList[1], changeMetrics2);
    QCOMPARE(changeList[2], changeMetrics3);

    // revert after first change
    changedSectionList.clear();
    changeList.clear();
    result =
        groupPieceTableChange.revertBeforeChange(&pieceTable, 1, &changedSectionList, &changeList);

    QVERIFY(result);
    QCOMPARE(groupPieceTableChange.count(), 4);
    QCOMPARE(groupPieceTableChange.appliedChangesCount(), 1);
    QCOMPARE(groupPieceTableChange.headChangeDescription(), description4);
    QCOMPARE(changedSectionList.count(), 1);
    QCOMPARE(changedSectionList.first(), Section(2, 3));
    QCOMPARE(changeList.count(), 2);
    QCOMPARE(changeList[0], revertedChangeMetrics3);
    QCOMPARE(changeList[1], revertedChangeMetrics2);

    // revert after last change
    changedSectionList.clear();
    changeList.clear();
    result =
        groupPieceTableChange.revertBeforeChange(&pieceTable, 4, &changedSectionList, &changeList);

    QVERIFY(result);
    QCOMPARE(groupPieceTableChange.count(), 4);
    QCOMPARE(groupPieceTableChange.appliedChangesCount(), 4);
    QCOMPARE(groupPieceTableChange.headChangeDescription(), description4);
    QCOMPARE(changedSectionList.count(), 1);
    QCOMPARE(changedSectionList.first(), Section(2, 4));
    QCOMPARE(changeList.count(), 3);
    QCOMPARE(changeList[0], changeMetrics2);
    QCOMPARE(changeList[1], changeMetrics3);
    QCOMPARE(changeList[2], changeMetrics4);
}
#endif
}

QTEST_GUILESS_MAIN(KPieceTable::GroupPieceTableChangeTest)
