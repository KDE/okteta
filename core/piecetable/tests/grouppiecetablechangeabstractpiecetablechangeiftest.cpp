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

AbstractPieceTableChange* GroupPieceTableChangeAbstractPieceTableChangeIfTest::createPieceTableChange()
{
    const Piece replacedPiece(AddressRange::fromWidth(ReplacedOldStorageOffset, ReplacedWidth), Piece::ChangeStorage);
    const Piece removedPiece(AddressRange::fromWidth(RemovedOldStorageOffset, RemovedWidth), Piece::ChangeStorage);

    ReplacePieceTableChange* replaceChange =
        new ReplacePieceTableChange(AddressRange(ReplacedStart, ReplacedEnd),
                                    ReplaceLength, ReplaceInsertStorageOffset,
                                    PieceList(replacedPiece));
    InsertPieceTableChange* insertChange =
        new InsertPieceTableChange(InsertOffset, InsertLength, InsertStorageOffset);
    RemovePieceTableChange* removeChange =
        new RemovePieceTableChange(AddressRange(RemovedStart, RemovedEnd), PieceList(removedPiece));

    GroupPieceTableChange* pieceTableChange =
        new GroupPieceTableChange(0, QString());

    pieceTableChange->appendChange(replaceChange);
    pieceTableChange->appendChange(insertChange);
    pieceTableChange->appendChange(removeChange);

    return pieceTableChange;
}
void GroupPieceTableChangeAbstractPieceTableChangeIfTest::changePieceTable(PieceTable* pieceTable)
{
    pieceTable->replace(AddressRange(ReplacedStart, ReplacedEnd), ReplaceLength, ReplaceInsertStorageOffset);
    pieceTable->insert(InsertOffset, InsertLength, InsertStorageOffset);
    pieceTable->remove(AddressRange(RemovedStart, RemovedEnd));
}

void GroupPieceTableChangeAbstractPieceTableChangeIfTest::deletePieceTableChange(
    AbstractPieceTableChange* pieceTableChange)
{
    delete pieceTableChange;
}

}

QTEST_GUILESS_MAIN(KPieceTable::GroupPieceTableChangeAbstractPieceTableChangeIfTest)
