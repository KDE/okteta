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

protected:
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
