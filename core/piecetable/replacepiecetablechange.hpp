/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KPIECETABLE_REPLACEPIECETABLECHANGE_HPP
#define KPIECETABLE_REPLACEPIECETABLECHANGE_HPP

// lib
#include "abstractpiecetablechange.hpp"
#include "piecelist.hpp"

namespace KPieceTable {

/** class
 * @author Friedrich W. H. Kossebau
 */

class ReplacePieceTableChange : public AbstractPieceTableChange
{
public:
    ReplacePieceTableChange(const AddressRange& removeRange, Size insertLength, Address storageOffset,
                            const PieceList& removedPieces);
    ReplacePieceTableChange(const ReplacePieceTableChange&) = delete;
    ReplacePieceTableChange(ReplacePieceTableChange&&) = delete;

    ~ReplacePieceTableChange() override;

    ReplacePieceTableChange& operator=(const ReplacePieceTableChange&) = delete;
    ReplacePieceTableChange& operator=(ReplacePieceTableChange&&) = delete;

public: // AbstractPieceTableChange API
    [[nodiscard]]
    int type() const override;
    [[nodiscard]]
    QString description() const override;
    [[nodiscard]]
    Address storageOffset() const override;
    [[nodiscard]]
    bool merge(const AbstractPieceTableChange* other) override;
    AddressRange apply(PieceTable* pieceTable) const override;
    AddressRange revert(PieceTable* pieceTable) const override;
    [[nodiscard]]
    ArrayChangeMetrics metrics() const override;
    [[nodiscard]]
    Size dataSize() const override;

private:
    AddressRange mRemoveRange;
    Size mInsertLength;
    Address mStorageOffset;
    PieceList mRemovedPieces;
};

inline ReplacePieceTableChange::ReplacePieceTableChange(const AddressRange& removeRange,
                                                        Size insertLength, Address storageOffset,
                                                        const PieceList& removedPieces)
    : mRemoveRange(removeRange)
    , mInsertLength(insertLength)
    , mStorageOffset(storageOffset)
    , mRemovedPieces(removedPieces)
{}

}

#endif
