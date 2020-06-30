/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KPIECETABLE_SWAPRANGESPIECETABLECHANGE_HPP
#define KPIECETABLE_SWAPRANGESPIECETABLECHANGE_HPP

// lib
#include "abstractpiecetablechange.hpp"

namespace KPieceTable {

/** class
 * @author Friedrich W. H. Kossebau
 */

class SwapRangesPieceTableChange : public AbstractPieceTableChange
{
public:
    SwapRangesPieceTableChange(Address firstStart, const AddressRange& secondRange);
    ~SwapRangesPieceTableChange() override;

public: // AbstractPieceTableChange API
    int type() const override;
    QString description() const override;
    AddressRange apply(PieceTable* pieceTable) const override;
    AddressRange revert(PieceTable* pieceTable) const override;
    ArrayChangeMetrics metrics() const override;

private:
    Address mFirstStart;
    AddressRange mSecondRange;
};

inline SwapRangesPieceTableChange::SwapRangesPieceTableChange(Address firstStart, const AddressRange& secondRange)
    : mFirstStart(firstStart)
    , mSecondRange(secondRange)
{}

}

#endif
