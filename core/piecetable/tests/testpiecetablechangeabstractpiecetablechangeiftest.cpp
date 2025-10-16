/*
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

static constexpr KPieceTable::Address TestOffset = 30;
static constexpr KPieceTable::Address StorageOffset = 20;
static constexpr KPieceTable::Address ReplacedStorageOffset = 53;

namespace KPieceTable {

std::unique_ptr<AbstractPieceTableChange> TestPieceTableChangeAbstractPieceTableChangeIfTest::createPieceTableChange()
{
    auto pieceTableChange =
        std::make_unique<TestPieceTableChange>(0, QString(),
                                               TestOffset, StorageOffset, Piece::ChangeStorage,
                                               ReplacedStorageOffset);

    return pieceTableChange;
}
void TestPieceTableChangeAbstractPieceTableChangeIfTest::changePieceTable(PieceTable* pieceTable)
{
    pieceTable->replaceOne(TestOffset, StorageOffset);
}

void TestPieceTableChangeAbstractPieceTableChangeIfTest::deletePieceTableChange(
    std::unique_ptr<AbstractPieceTableChange>&& pieceTableChange)
{
    Q_UNUSED(pieceTableChange)
}

}

QTEST_GUILESS_MAIN(KPieceTable::TestPieceTableChangeAbstractPieceTableChangeIfTest)

#include "moc_testpiecetablechangeabstractpiecetablechangeiftest.cpp"
