/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "testpiecetablechange.hpp"

// lib
#include <piecetable/piecetable.hpp>
#include <arraychangemetrics.hpp>
// Qt
#include <QString>

namespace KPieceTable {

TestPieceTableChange::~TestPieceTableChange() = default;

int TestPieceTableChange::type() const { return mTypeId; }

QString TestPieceTableChange::description() const
{
    return mDescription;
}

bool TestPieceTableChange::merge(const AbstractPieceTableChange* other)
{
    bool result = false;
    if (other->type() == mTypeId) {
        const auto* otherTestChange = static_cast<const TestPieceTableChange*>(other);
        mDescription += otherTestChange->mDescription;
        result = true;
    }

    return result;
}

AddressRange TestPieceTableChange::apply(PieceTable* pieceTable) const
{
    pieceTable->replaceOne(mPosition, mStoragePosition, mStorageId);

    return AddressRange(mPosition, mPosition);
}

AddressRange TestPieceTableChange::revert(PieceTable* pieceTable) const
{
    const Piece replaced = pieceTable->replaceOne(mPosition, mReplacedStoragePosition, mReplacedStorageId);

    return AddressRange(mPosition, mPosition);
}

ArrayChangeMetrics TestPieceTableChange::metrics() const
{
    return ArrayChangeMetrics::asReplacement(mPosition, 1, 1);
}

int TestPieceTableChange::dataSize() const { return 1; }

}
