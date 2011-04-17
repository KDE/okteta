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

#include "revertablepiecetable.h"

// lib
#include "insertpiecetablechange.h"
#include "removepiecetablechange.h"
#include "replacepiecetablechange.h"
#include "swaprangespiecetablechange.h"
//
#include <arraychangemetricslist.h>


namespace KPieceTable
{

RevertablePieceTable::RevertablePieceTable() {}

void RevertablePieceTable::init( Size size )
{
    mPieceTable.init( size );
    mChangeHistory.clear();
}

void RevertablePieceTable::getChangeData( ArrayChangeMetrics* metrics, Address* storageOffset, int versionIndex ) const
{
    mChangeHistory.getChangeData( metrics, storageOffset, versionIndex );
}

bool RevertablePieceTable::insert( Address dataOffset, Size length, Address* storageOffset )
{
    *storageOffset = mChangeHistory.appliedChangesDataSize();

    mPieceTable.insert( dataOffset, length, *storageOffset );

    InsertPieceTableChange* change =
        new InsertPieceTableChange( dataOffset, length, *storageOffset );

    return mChangeHistory.appendChange( change );
}

bool RevertablePieceTable::remove( const AddressRange& removeRange )
{
    const PieceList removedPieces = mPieceTable.remove( removeRange );

    RemovePieceTableChange* change =
        new RemovePieceTableChange( removeRange, removedPieces );

    return mChangeHistory.appendChange( change );
}

bool RevertablePieceTable::replace( const AddressRange& removeRange, Size insertLength, Size* storageSize )
{
    *storageSize = mChangeHistory.appliedChangesDataSize();

    const PieceList replacedPieces = mPieceTable.remove( removeRange );
    mPieceTable.insert( removeRange.start(), insertLength, *storageSize );

    ReplacePieceTableChange* change =
        new ReplacePieceTableChange( removeRange, insertLength, *storageSize, replacedPieces );

    return mChangeHistory.appendChange( change );
}

bool RevertablePieceTable::swap( Address firstStart, const AddressRange& secondRange )
{
    mPieceTable.swap( firstStart, secondRange );

    SwapRangesPieceTableChange* change =
        new SwapRangesPieceTableChange( firstStart, secondRange);

    return mChangeHistory.appendChange( change );
}

bool RevertablePieceTable::replaceOne( Address dataOffset, Size* storageSize )
{
    *storageSize = mChangeHistory.appliedChangesDataSize();

    const Piece replacedPiece = mPieceTable.replaceOne( dataOffset, *storageSize );
    const PieceList replacedPieces( replacedPiece );

    ReplacePieceTableChange* change =
        new ReplacePieceTableChange( AddressRange::fromWidth(dataOffset,1), 1, *storageSize, replacedPieces );

    return mChangeHistory.appendChange( change );
}

}
