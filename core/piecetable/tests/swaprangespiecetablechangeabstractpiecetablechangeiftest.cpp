/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "swaprangespiecetablechangeabstractpiecetablechangeiftest.hpp"

// test object
#include <piecetable/swaprangespiecetablechange.hpp>
// lib
#include <piecetable/piecetable.hpp>
// Qt
#include <QTest>

namespace KPieceTable {

static constexpr Address FirstRangeStart = 30;
static constexpr Address SecondRangeStart = 50;
static constexpr Address SecondRangeEnd = 66;

std::unique_ptr<AbstractPieceTableChange> SwapRangesPieceTableChangeAbstractPieceTableChangeIfTest::createPieceTableChange()
{
    auto pieceTableChange =
        std::make_unique<SwapRangesPieceTableChange>(FirstRangeStart, AddressRange(SecondRangeStart, SecondRangeEnd));

    return pieceTableChange;
}

void SwapRangesPieceTableChangeAbstractPieceTableChangeIfTest::changePieceTable(PieceTable* pieceTable)
{
    pieceTable->swap(FirstRangeStart, AddressRange(SecondRangeStart, SecondRangeEnd));
}

void SwapRangesPieceTableChangeAbstractPieceTableChangeIfTest::deletePieceTableChange(std::unique_ptr<AbstractPieceTableChange>&& pieceTableChange)
{
    Q_UNUSED(pieceTableChange)
}

}

QTEST_GUILESS_MAIN(KPieceTable::SwapRangesPieceTableChangeAbstractPieceTableChangeIfTest)

#include "moc_swaprangespiecetablechangeabstractpiecetablechangeiftest.cpp"
