/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "testpiecetablechangetest.hpp"

// test object
#include "testpiecetablechange.hpp"
//
#include <piecetable/piecetable.hpp>
// Qt
#include <QTest>

namespace KPieceTable {

// local variables

void TestPieceTableChangeTest::testSimpleConstructor()
{
    const int typeId = 0;
    const QString description = QStringLiteral("0");
    TestPieceTableChange change(typeId, description);

    QCOMPARE(change.type(), typeId);
    QCOMPARE(change.description(), description);
}

void TestPieceTableChangeTest::testMerge()
{
    const int type0Id = 0;
    const QString description0 = QStringLiteral("0");
    const int type1Id = 1;
    const QString description1 = QStringLiteral("1");

    TestPieceTableChange change(type0Id, description0);

    // merge with different
    TestPieceTableChange otherDifferentChange(type1Id, description1);
    bool result = change.merge(&otherDifferentChange);

    QVERIFY(!result);
    QCOMPARE(change.type(), type0Id);
    QCOMPARE(change.description(), description0);

    // merge with same
    TestPieceTableChange otherSameChange(type0Id, description1);
    result = change.merge(&otherSameChange);

    QVERIFY(result);
    QCOMPARE(change.type(), type0Id);
    QCOMPARE(change.description(), QString(description0 + description1));
}

}

QTEST_GUILESS_MAIN(KPieceTable::TestPieceTableChangeTest)
