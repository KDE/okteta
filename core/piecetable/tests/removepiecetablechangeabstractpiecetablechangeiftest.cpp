/*
    This file is part of the Okteta Core library, made within the KDE community.

    Copyright 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library. If not, see <http://www.gnu.org/licenses/>.
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
