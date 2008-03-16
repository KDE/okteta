/*
    This file is part of the Okteta Core library, part of the KDE project.

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
#include <ksectionlist.h>

namespace KPieceTable
{

RevertablePieceTable::RevertablePieceTable() {}

void RevertablePieceTable::init( int size )
{
    mPieceTable.init( size );
    mChangeHistory.clear();
}

bool RevertablePieceTable::insert( int dataOffset, int length, int *storageOffset )
{
    *storageOffset = mChangeHistory.appliedChangesDataSize();

    mPieceTable.insert( dataOffset, length, *storageOffset );

    InsertPieceTableChange *change =
        new InsertPieceTableChange( dataOffset, length, *storageOffset );

    return mChangeHistory.appendChange( change );
}

bool RevertablePieceTable::remove( const KHE::KSection &removeSection )
{
    const PieceList removedPieces = mPieceTable.remove( removeSection );

    RemovePieceTableChange *change =
        new RemovePieceTableChange( removeSection, removedPieces );

    return mChangeHistory.appendChange( change );
}

bool RevertablePieceTable::replace( const KHE::KSection &removeSection, int insertLength, int *storageOffset )
{
    *storageOffset = mChangeHistory.appliedChangesDataSize();

    const PieceList replacedPieces = mPieceTable.remove( removeSection );
    mPieceTable.insert( removeSection.start(), insertLength, *storageOffset );

    ReplacePieceTableChange *change =
        new ReplacePieceTableChange( removeSection, insertLength, *storageOffset, replacedPieces );

    return mChangeHistory.appendChange( change );
}

bool RevertablePieceTable::swap( int firstStart, const KHE::KSection &secondSection )
{
    mPieceTable.swap( firstStart, secondSection );

    SwapRangesPieceTableChange *change =
        new SwapRangesPieceTableChange( firstStart, secondSection);

    return mChangeHistory.appendChange( change );
}

bool RevertablePieceTable::replaceOne( int dataOffset, int *storageOffset )
{
    *storageOffset = mChangeHistory.appliedChangesDataSize();

    const Piece replacedPiece = mPieceTable.replaceOne( dataOffset, *storageOffset );
    const PieceList replacedPieces( replacedPiece );

    ReplacePieceTableChange *change =
        new ReplacePieceTableChange( KHE::KSection::fromWidth(dataOffset,1), 1, *storageOffset, replacedPieces );

    return mChangeHistory.appendChange( change );
}

}
