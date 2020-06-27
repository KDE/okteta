/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "replacepiecetablechangeabstractpiecetablechangeiftest.hpp"

// test object
#include <piecetable/replacepiecetablechange.hpp>
// lib
#include <piecetable/piecetable.hpp>
// Qt
#include <QTest>

namespace KPieceTable {

static constexpr Address Start = 32;
static constexpr Address End = 78;
static constexpr Size Width = End - Start + 1;
static constexpr Size InsertLength = 80;
static constexpr Address InsertStorageOffset = 67;

static constexpr Address ChangeStorageOffset = 23;

AbstractPieceTableChange* ReplacePieceTableChangeAbstractPieceTableChangeIfTest::createPieceTableChange()
{
    const Piece replacedPiece(AddressRange::fromWidth(Start + ChangeStorageOffset, Width), Piece::ChangeStorage);

    ReplacePieceTableChange* pieceTableChange =
        new ReplacePieceTableChange(AddressRange(Start, End), InsertLength, InsertStorageOffset,
                                    PieceList(replacedPiece));

    return pieceTableChange;
}
void ReplacePieceTableChangeAbstractPieceTableChangeIfTest::changePieceTable(PieceTable* pieceTable)
{
    pieceTable->replace(AddressRange(Start, End), InsertLength, InsertStorageOffset);
}

void ReplacePieceTableChangeAbstractPieceTableChangeIfTest::deletePieceTableChange(
    AbstractPieceTableChange* pieceTableChange)
{
    delete pieceTableChange;
}

}

QTEST_GUILESS_MAIN(KPieceTable::ReplacePieceTableChangeAbstractPieceTableChangeIfTest)
