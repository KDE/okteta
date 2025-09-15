/*
    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "insertpiecetablechangeabstractpiecetablechangeiftest.hpp"

// test object
#include <piecetable/insertpiecetablechange.hpp>
// lib
#include <piecetable/piecetable.hpp>
// Qt
#include <QTest>

namespace KPieceTable {

static constexpr Address InsertOffset = 30;
static constexpr Size InsertLength = 80;
static constexpr Address StorageOffset = 67;

std::unique_ptr<AbstractPieceTableChange> InsertPieceTableChangeAbstractPieceTableChangeIfTest::createPieceTableChange()
{
    auto pieceTableChange = std::make_unique<InsertPieceTableChange>(InsertOffset, InsertLength, StorageOffset);

    return pieceTableChange;
}

void InsertPieceTableChangeAbstractPieceTableChangeIfTest::changePieceTable(PieceTable* pieceTable)
{
    pieceTable->insert(InsertOffset, InsertLength, StorageOffset);
}

void InsertPieceTableChangeAbstractPieceTableChangeIfTest::deletePieceTableChange(std::unique_ptr<AbstractPieceTableChange>&& pieceTableChange)
{
    Q_UNUSED(pieceTableChange)
}

}

QTEST_GUILESS_MAIN(KPieceTable::InsertPieceTableChangeAbstractPieceTableChangeIfTest)

#include "moc_insertpiecetablechangeabstractpiecetablechangeiftest.cpp"
