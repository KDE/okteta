/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "revertablepiecetable.hpp"

// lib
#include "insertpiecetablechange.hpp"
#include "removepiecetablechange.hpp"
#include "replacepiecetablechange.hpp"
#include "swaprangespiecetablechange.hpp"

namespace KPieceTable {

RevertablePieceTable::RevertablePieceTable() = default;

void RevertablePieceTable::init(Size size)
{
    mPieceTable.init(size);
    mChangeHistory.clear();
}

void RevertablePieceTable::getChangeData(ArrayChangeMetrics* metrics, Address* storageOffset, int versionIndex) const
{
    mChangeHistory.getChangeData(metrics, storageOffset, versionIndex);
}

bool RevertablePieceTable::insert(Address dataOffset, Size length, Address* storageOffset)
{
    *storageOffset = mChangeHistory.appliedChangesDataSize();

    mPieceTable.insert(dataOffset, length, *storageOffset);

    auto change = std::make_unique<InsertPieceTableChange>(dataOffset, length, *storageOffset);

    return mChangeHistory.appendChange(std::move(change));
}

bool RevertablePieceTable::remove(const AddressRange& removeRange)
{
    const PieceList removedPieces = mPieceTable.remove(removeRange);

    auto change = std::make_unique<RemovePieceTableChange>(removeRange, removedPieces);

    return mChangeHistory.appendChange(std::move(change));
}

bool RevertablePieceTable::replace(const AddressRange& removeRange, Size insertLength, Size* storageSize)
{
    *storageSize = mChangeHistory.appliedChangesDataSize();

    const PieceList replacedPieces = mPieceTable.remove(removeRange);
    mPieceTable.insert(removeRange.start(), insertLength, *storageSize);

    auto change = std::make_unique<ReplacePieceTableChange>(removeRange, insertLength, *storageSize, replacedPieces);

    return mChangeHistory.appendChange(std::move(change));
}

bool RevertablePieceTable::swap(Address firstStart, const AddressRange& secondRange)
{
    mPieceTable.swap(firstStart, secondRange);

    auto change = std::make_unique<SwapRangesPieceTableChange>(firstStart, secondRange);

    return mChangeHistory.appendChange(std::move(change));
}

bool RevertablePieceTable::replaceOne(Address dataOffset, Size* storageSize)
{
    *storageSize = mChangeHistory.appliedChangesDataSize();

    const Piece replacedPiece = mPieceTable.replaceOne(dataOffset, *storageSize);
    const PieceList replacedPieces(replacedPiece);

    auto change =
        std::make_unique<ReplacePieceTableChange>(AddressRange::fromWidth(dataOffset, 1), 1, *storageSize, replacedPieces);

    return mChangeHistory.appendChange(std::move(change));
}

}
