/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "removepiecetablechangeabstractpiecetablechangeiftest.hpp"

// test object
#include <piecetable/removepiecetablechange.hpp>
// lib
#include <piecetable/piecetable.hpp>
// Qt
#include <QTest>

namespace KPieceTable {

static constexpr Address Start = 32;
static constexpr Address End = 78;
static constexpr Size Width = End - Start + 1;

static constexpr Address ChangeStorageOffset = 23;

AbstractPieceTableChange* RemovePieceTableChangeAbstractPieceTableChangeIfTest::createPieceTableChange()
{
    const Piece removedPiece(AddressRange::fromWidth(Start + ChangeStorageOffset, Width), Piece::ChangeStorage);
    RemovePieceTableChange* pieceTableChange =
        new RemovePieceTableChange(AddressRange(Start, End), PieceList(removedPiece));

    return pieceTableChange;
}
void RemovePieceTableChangeAbstractPieceTableChangeIfTest::changePieceTable(PieceTable* pieceTable)
{
    pieceTable->remove(AddressRange(Start, End));
}

void RemovePieceTableChangeAbstractPieceTableChangeIfTest::deletePieceTableChange(
    AbstractPieceTableChange* pieceTableChange)
{
    delete pieceTableChange;
}

}

QTEST_GUILESS_MAIN(KPieceTable::RemovePieceTableChangeAbstractPieceTableChangeIfTest)
