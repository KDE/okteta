/*
    This file is part of the Okteta Core library, made within the KDE community.

    Copyright 2008-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef KPIECETABLE_ABSTRACTPIECETABLECHANGE_H
#define KPIECETABLE_ABSTRACTPIECETABLECHANGE_H

// lib
#include "piece.h"
//
#include <arraychangemetrics.h>

class QString;


namespace KPieceTable
{

class PieceTable;

typedef Okteta::ArrayChangeMetrics ArrayChangeMetrics;

/** class
  *@author Friedrich W. H. Kossebau
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
    virtual bool merge( const AbstractPieceTableChange* other );

    virtual AddressRange apply( PieceTable* pieceTable ) const = 0;
    virtual AddressRange revert( PieceTable* pieceTable ) const = 0;

    virtual ArrayChangeMetrics metrics() const = 0;
    /// returns the size of the added data. Default returns 0.
    virtual Size dataSize() const;
};

inline AbstractPieceTableChange::~AbstractPieceTableChange() {}

}

#endif
