/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "abstractpiecetablechangeiftest.hpp"

// test object
#include <piecetable/abstractpiecetablechange.hpp>
// lib
#include "testpiecetablechange.hpp"
#include <piecetable/piecetable.hpp>
// Qt
#include <QTest>

namespace KPieceTable {

static constexpr Size BaseSize = 100;
static constexpr Address ChangeStorageOffset = 23;

//---------------------------------------------------------------- Tests -----

void AbstractPieceTableChangeIfTest::init()
{
    mPieceTableChange = createPieceTableChange();
}

void AbstractPieceTableChangeIfTest::cleanup()
{
    deletePieceTableChange(mPieceTableChange);
}

//---------------------------------------------------------------- Tests -----

void AbstractPieceTableChangeIfTest::testMerge()
{
    TestPieceTableChange testChange;

    int typeId = mPieceTableChange->type();
    QString description = mPieceTableChange->description();
    bool result = mPieceTableChange->merge(&testChange);

    QVERIFY(!result);
    QCOMPARE(mPieceTableChange->type(), typeId);
    QCOMPARE(mPieceTableChange->description(), description);
}

void AbstractPieceTableChangeIfTest::testRevertApply()
{
    PieceTable pieceTable;
    pieceTable.init(0);
    pieceTable.insert(0, BaseSize, ChangeStorageOffset);

    // do change which creates this
    changePieceTable(&pieceTable);
    mPieceTableChange->revert(&pieceTable);

    QCOMPARE(pieceTable.size(), BaseSize);
    for (int i = 0; i < BaseSize; ++i) {
        int storageId;
        Address storageOffset;
        bool result = pieceTable.getStorageData(&storageId, &storageOffset, i);

        QVERIFY(result);
        QCOMPARE(storageOffset, i + ChangeStorageOffset);
        QCOMPARE(storageId, (int)Piece::ChangeStorage);
    }

    // now replay changes
    mPieceTableChange->apply(&pieceTable);
    mPieceTableChange->revert(&pieceTable);

    QCOMPARE(pieceTable.size(), BaseSize);
    for (Address i = 0; i < BaseSize; ++i) {
        int storageId;
        Address storageOffset;
        bool result = pieceTable.getStorageData(&storageId, &storageOffset, i);

        QVERIFY(result);
        QCOMPARE(storageOffset, i + ChangeStorageOffset);
        QCOMPARE(storageId, (int)Piece::ChangeStorage);
    }
}

}
