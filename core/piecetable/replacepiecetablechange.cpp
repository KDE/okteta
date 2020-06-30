/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "replacepiecetablechange.hpp"

// lib
#include "piecetable.hpp"
//
#include <arraychangemetrics.hpp>
// KF
#include <KLocalizedString>

namespace KPieceTable {

ReplacePieceTableChange::~ReplacePieceTableChange() = default;

int ReplacePieceTableChange::type() const { return ReplaceId; }

QString ReplacePieceTableChange::description() const
{
    return i18nc("name of the change", "Replace");
}

Address ReplacePieceTableChange::storageOffset() const { return mStorageOffset; }

bool ReplacePieceTableChange::merge(const AbstractPieceTableChange* other)
{
// TODO: remove me again after synching solved
// return false;
    bool result = false;

    if (other->type() == ReplaceId) {
        const auto* otherReplaceChange = static_cast<const ReplacePieceTableChange*>(other);
        // other replaced after?
        if (mRemoveRange.start() + mInsertLength == otherReplaceChange->mRemoveRange.start()) {
            mRemoveRange.moveEndBy(otherReplaceChange->mRemoveRange.width());
            mInsertLength += otherReplaceChange->mInsertLength;
            mRemovedPieces.append(otherReplaceChange->mRemovedPieces);
            result = true;
        }
        // other replaced before would be two swapped ranges in the change buffer, if this ever needed/wanted?
    }

    return result;
}

AddressRange ReplacePieceTableChange::apply(PieceTable* pieceTable) const
{
    const Size oldSize = pieceTable->size();

    pieceTable->replace(mRemoveRange, mInsertLength, mStorageOffset);

    const Size newSize = pieceTable->size();
    const Address lastChanged = (newSize == oldSize) ? mRemoveRange.end() :
                                (newSize > oldSize) ?  newSize - 1 :
                                                       oldSize - 1;
    return AddressRange(mRemoveRange.start(), lastChanged);
}

AddressRange ReplacePieceTableChange::revert(PieceTable* pieceTable) const
{
    const Size oldSize = pieceTable->size();

    const AddressRange insertedSection = AddressRange::fromWidth(mRemoveRange.start(), mInsertLength);
    pieceTable->replace(insertedSection, mRemovedPieces);

    const Size newSize = pieceTable->size();
    const Address lastChanged = (newSize == oldSize) ? insertedSection.end() :
                                (newSize > oldSize) ?  newSize - 1 :
                                                       oldSize - 1;
    return AddressRange(mRemoveRange.start(), lastChanged);
}

ArrayChangeMetrics ReplacePieceTableChange::metrics() const
{
    return ArrayChangeMetrics::asReplacement(mRemoveRange.start(), mRemoveRange.width(), mInsertLength);
}

Size ReplacePieceTableChange::dataSize() const { return mInsertLength; }

}
