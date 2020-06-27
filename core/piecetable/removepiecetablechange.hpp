/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KPIECETABLE_REMOVEPIECETABLECHANGE_HPP
#define KPIECETABLE_REMOVEPIECETABLECHANGE_HPP

// lib
#include "abstractpiecetablechange.hpp"
#include "piecelist.hpp"

namespace KPieceTable {

/** class
 * @author Friedrich W. H. Kossebau
 */

class RemovePieceTableChange : public AbstractPieceTableChange
{
public:
    RemovePieceTableChange(const AddressRange& removeRange, const PieceList& removedPieces);
    ~RemovePieceTableChange() override;

public: // AbstractPieceTableChange API
    int type() const override;
    QString description() const override;
    bool merge(const AbstractPieceTableChange* other) override;
    AddressRange apply(PieceTable* pieceTable) const override;
    AddressRange revert(PieceTable* pieceTable) const override;
    ArrayChangeMetrics metrics() const override;

private:
    AddressRange mRemoveRange;
    PieceList mRemovedPieces;
};

inline RemovePieceTableChange::RemovePieceTableChange(const AddressRange& removeRange, const PieceList& removedPieces)
    : mRemoveRange(removeRange)
    , mRemovedPieces(removedPieces)
{}
}

#endif
