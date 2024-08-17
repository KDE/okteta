/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "grouppiecetablechangeabstractpiecetablechangeiftest.hpp"

// test object
#include <piecetable/grouppiecetablechange.hpp>
#include <piecetable/replacepiecetablechange.hpp>
#include <piecetable/insertpiecetablechange.hpp>
#include <piecetable/removepiecetablechange.hpp>
// lib
#include <piecetable/piecetable.hpp>
// Qt
#include <QTest>

namespace KPieceTable {

static constexpr Address ReplacedOldStorageOffset = 80;
static constexpr Address ReplacedStart = 32;
static constexpr Address ReplacedEnd = 78;
static constexpr Size ReplacedWidth = ReplacedEnd - ReplacedStart + 1;
static constexpr Size ReplaceLength = 30;
static constexpr Address ReplaceInsertStorageOffset = 67;

static constexpr Address InsertOffset = 50;
static constexpr Size InsertLength = 80;
static constexpr Address InsertStorageOffset = 67;

static constexpr Address RemovedStart = 40;
static constexpr Address RemovedEnd = 78;
static constexpr Size RemovedWidth = RemovedEnd - RemovedStart + 1;
static constexpr Address RemovedOldStorageOffset = 23;

std::unique_ptr<AbstractPieceTableChange> GroupPieceTableChangeAbstractPieceTableChangeIfTest::createPieceTableChange()
{
    const Piece replacedPiece(AddressRange::fromWidth(ReplacedOldStorageOffset, ReplacedWidth), Piece::ChangeStorage);
    const Piece removedPiece(AddressRange::fromWidth(RemovedOldStorageOffset, RemovedWidth), Piece::ChangeStorage);

    auto replaceChange =
        std::make_unique<ReplacePieceTableChange>(AddressRange(ReplacedStart, ReplacedEnd),
                                                  ReplaceLength, ReplaceInsertStorageOffset,
                                                  PieceList(replacedPiece));
    auto insertChange =
        std::make_unique<InsertPieceTableChange>(InsertOffset, InsertLength, InsertStorageOffset);
    auto removeChange =
        std::make_unique<RemovePieceTableChange>(AddressRange(RemovedStart, RemovedEnd), PieceList(removedPiece));

    auto pieceTableChange =
        std::make_unique<GroupPieceTableChange>(nullptr, QString());

    pieceTableChange->appendChange(std::move(replaceChange));
    pieceTableChange->appendChange(std::move(insertChange));
    pieceTableChange->appendChange(std::move(removeChange));

    return pieceTableChange;
}

void GroupPieceTableChangeAbstractPieceTableChangeIfTest::changePieceTable(PieceTable* pieceTable)
{
    pieceTable->replace(AddressRange(ReplacedStart, ReplacedEnd), ReplaceLength, ReplaceInsertStorageOffset);
    pieceTable->insert(InsertOffset, InsertLength, InsertStorageOffset);
    pieceTable->remove(AddressRange(RemovedStart, RemovedEnd));
}

void GroupPieceTableChangeAbstractPieceTableChangeIfTest::deletePieceTableChange(
    std::unique_ptr<AbstractPieceTableChange>&& pieceTableChange)
{
    Q_UNUSED(pieceTableChange)
}

}

QTEST_GUILESS_MAIN(KPieceTable::GroupPieceTableChangeAbstractPieceTableChangeIfTest)
