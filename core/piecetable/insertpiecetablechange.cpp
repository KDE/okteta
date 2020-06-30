/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "insertpiecetablechange.hpp"

// lib
#include "piecetable.hpp"
//
#include <arraychangemetrics.hpp>
// KF
#include <KLocalizedString>

namespace KPieceTable {

InsertPieceTableChange::~InsertPieceTableChange() = default;

int InsertPieceTableChange::type() const { return InsertId; }

QString InsertPieceTableChange::description() const
{
    return i18nc("name of the change", "Insert");
}

Address InsertPieceTableChange::storageOffset() const { return mStorageOffset; }

bool InsertPieceTableChange::merge(const AbstractPieceTableChange* other)
{
// TODO: remove me again after synching solved
// return false;
    bool result = false;

    if (other->type() == InsertId) {
        const auto* otherInsertChange = static_cast<const InsertPieceTableChange*>(other);
        if (mInsertOffset + mInsertLength == otherInsertChange->mInsertOffset) {
            mInsertLength += otherInsertChange->mInsertLength;
            result = true;
        }
    }

    return result;
}

AddressRange InsertPieceTableChange::apply(PieceTable* pieceTable) const
{
    pieceTable->insert(mInsertOffset, mInsertLength, mStorageOffset);

    return AddressRange(mInsertOffset, pieceTable->size() - 1);
}

AddressRange InsertPieceTableChange::revert(PieceTable* pieceTable) const
{
    const Address oldLast = pieceTable->size() - 1;
    pieceTable->remove(AddressRange::fromWidth(mInsertOffset, mInsertLength));
    return AddressRange(mInsertOffset, oldLast);
}

ArrayChangeMetrics InsertPieceTableChange::metrics() const
{
    return ArrayChangeMetrics::asReplacement(mInsertOffset, 0, mInsertLength);
}

Size InsertPieceTableChange::dataSize() const { return mInsertLength; }

}
