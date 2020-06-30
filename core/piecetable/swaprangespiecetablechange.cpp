/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "swaprangespiecetablechange.hpp"

// lib
#include "piecetable.hpp"
//
#include <arraychangemetrics.hpp>
// KF
#include <KLocalizedString>

namespace KPieceTable {

SwapRangesPieceTableChange::~SwapRangesPieceTableChange() = default;

int SwapRangesPieceTableChange::type() const { return SwapRangesId; }

QString SwapRangesPieceTableChange::description() const
{
    return i18nc("name of the change", "Swap Ranges");
}

AddressRange SwapRangesPieceTableChange::apply(PieceTable* pieceTable) const
{
    pieceTable->swap(mFirstStart, mSecondRange);

    return AddressRange(mFirstStart, mSecondRange.end());
}

AddressRange SwapRangesPieceTableChange::revert(PieceTable* pieceTable) const
{
    pieceTable->swap(mFirstStart, AddressRange(mFirstStart + mSecondRange.width(), mSecondRange.end()));

    return AddressRange(mFirstStart, mSecondRange.end());
}

ArrayChangeMetrics SwapRangesPieceTableChange::metrics() const
{
    return ArrayChangeMetrics::asSwapping(mFirstStart, mSecondRange.start(), mSecondRange.width());
}

}
