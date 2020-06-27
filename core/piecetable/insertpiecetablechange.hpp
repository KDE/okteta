/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KPIECETABLE_INSERTPIECETABLECHANGE_HPP
#define KPIECETABLE_INSERTPIECETABLECHANGE_HPP

// lib
#include "abstractpiecetablechange.hpp"

namespace KPieceTable {

/** class
 * @author Friedrich W. H. Kossebau
 */

class InsertPieceTableChange : public AbstractPieceTableChange
{
public:
    InsertPieceTableChange(Address insertOffset, Size insertLength, Address storageOffset);

    ~InsertPieceTableChange() override;

public: // AbstractPieceTableChange API
    int type() const override;
    QString description() const override;
    Address storageOffset() const override;
    bool merge(const AbstractPieceTableChange* other) override;
    AddressRange apply(PieceTable* pieceTable) const override;
    AddressRange revert(PieceTable* pieceTable) const override;
    ArrayChangeMetrics metrics() const override;
    Size dataSize() const override;

private:
    Address mInsertOffset;
    Size mInsertLength;
    Address mStorageOffset;
};

inline InsertPieceTableChange::InsertPieceTableChange(Address insertOffset, Size insertLength, Address storageOffset)
    : mInsertOffset(insertOffset)
    , mInsertLength(insertLength)
    , mStorageOffset(storageOffset)
{}

}

#endif
