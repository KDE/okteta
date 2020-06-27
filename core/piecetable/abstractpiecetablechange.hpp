/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2008-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KPIECETABLE_ABSTRACTPIECETABLECHANGE_HPP
#define KPIECETABLE_ABSTRACTPIECETABLECHANGE_HPP

// lib
#include "piece.hpp"
//
#include <arraychangemetrics.hpp>

class QString;

namespace KPieceTable {

class PieceTable;

using ArrayChangeMetrics = Okteta::ArrayChangeMetrics;

/** class
 * @author Friedrich W. H. Kossebau
 */

class AbstractPieceTableChange
{
public:
    enum TypeId
    {
        InsertId,
        RemoveId,
        ReplaceId,
        SwapRangesId,
        FillId,
        ReplaceOneId,
        GroupId
    };

public:
    virtual ~AbstractPieceTableChange();

public: // API to be implemented
    virtual int type() const = 0;

    virtual QString description() const = 0;

    /// returns the storageOffset. Default returns -1.
    virtual Address storageOffset() const;

    /// returns true if successful, false otherwise. Default returns false.
    virtual bool merge(const AbstractPieceTableChange* other);

    virtual AddressRange apply(PieceTable* pieceTable) const = 0;
    virtual AddressRange revert(PieceTable* pieceTable) const = 0;

    virtual ArrayChangeMetrics metrics() const = 0;
    /// returns the size of the added data. Default returns 0.
    virtual Size dataSize() const;
};

inline AbstractPieceTableChange::~AbstractPieceTableChange() = default;

}

#endif
