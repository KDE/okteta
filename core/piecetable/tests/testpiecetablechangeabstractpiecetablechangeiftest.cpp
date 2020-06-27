/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "testpiecetablechangeabstractpiecetablechangeiftest.hpp"

// test object
#include "testpiecetablechange.hpp"
// lib
#include <piecetable/piecetable.hpp>
// Qt
#include <QTest>

static constexpr int TestOffset = 30;
static constexpr int StorageOffset = 20;
static constexpr int ReplacedStorageOffset = 53;

namespace KPieceTable {

KPieceTable::AbstractPieceTableChange* TestPieceTableChangeAbstractPieceTableChangeIfTest::createPieceTableChange()
{
    TestPieceTableChange* pieceTableChange =
        new TestPieceTableChange(0, QString(),
                                 TestOffset, StorageOffset, Piece::ChangeStorage,
                                 ReplacedStorageOffset);

    return pieceTableChange;
}
void TestPieceTableChangeAbstractPieceTableChangeIfTest::changePieceTable(PieceTable* pieceTable)
{
    pieceTable->replaceOne(TestOffset, StorageOffset);
}

void TestPieceTableChangeAbstractPieceTableChangeIfTest::deletePieceTableChange(
    AbstractPieceTableChange* pieceTableChange)
{
    delete pieceTableChange;
}

}

QTEST_GUILESS_MAIN(KPieceTable::TestPieceTableChangeAbstractPieceTableChangeIfTest)
