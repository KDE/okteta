/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "removepiecetablechange.hpp"

// lib
#include "piecetable.hpp"
//
#include <arraychangemetrics.hpp>
// KF
#include <KLocalizedString>

namespace KPieceTable {

RemovePieceTableChange::~RemovePieceTableChange() = default;

int RemovePieceTableChange::type() const { return RemoveId; }

QString RemovePieceTableChange::description() const
{
    return i18nc("name of the change", "Remove");
}

bool RemovePieceTableChange::merge(const AbstractPieceTableChange* other)
{
// TODO: remove me again after synching solved
// return false;
    bool result = false;

    if (other->type() == RemoveId) {
        const auto* otherRemoveChange = static_cast<const RemovePieceTableChange*>(other);
        // other removed at the same start?
        if (mRemoveRange.start() == otherRemoveChange->mRemoveRange.start()) {
            mRemoveRange.moveEndBy(otherRemoveChange->mRemoveRange.width());
            mRemovedPieces.append(otherRemoveChange->mRemovedPieces);
            result = true;
        }
        // other removed before?
        else if (otherRemoveChange->mRemoveRange.nextBehindEnd() == mRemoveRange.start()) {
            mRemoveRange.setStart(otherRemoveChange->mRemoveRange.start());
            mRemovedPieces.prepend(otherRemoveChange->mRemovedPieces);
            result = true;
        }
    }

    return result;
}

AddressRange RemovePieceTableChange::apply(PieceTable* pieceTable) const
{
    const Address oldLast = pieceTable->size() - 1;

    pieceTable->remove(mRemoveRange);

    return AddressRange(mRemoveRange.start(), oldLast);
}

AddressRange RemovePieceTableChange::revert(PieceTable* pieceTable) const
{
    pieceTable->insert(mRemoveRange.start(), mRemovedPieces);

    return AddressRange(mRemoveRange.start(), pieceTable->size() - 1);
}

ArrayChangeMetrics RemovePieceTableChange::metrics() const
{
    return ArrayChangeMetrics::asReplacement(mRemoveRange.start(), mRemoveRange.width(), 0);
}

}
