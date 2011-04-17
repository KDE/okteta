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

#include "removepiecetablechange.h"

// lib
#include "piecetable.h"
//
#include <arraychangemetrics.h>
// KDE
#include <KLocale>


namespace KPieceTable
{

int RemovePieceTableChange::type() const { return RemoveId; }

QString RemovePieceTableChange::description() const
{
    return i18nc( "name of the change", "Remove" );
}

bool RemovePieceTableChange::merge( const AbstractPieceTableChange* other )
{
// TODO: remove me again after synching solved
// return false;
    bool result = false;

    if( other->type() == RemoveId )
    {
        const RemovePieceTableChange* otherRemoveChange = static_cast<const RemovePieceTableChange*>( other );
        // other removed at the same start?
        if( mRemoveRange.start() == otherRemoveChange->mRemoveRange.start() )
        {
            mRemoveRange.moveEndBy( otherRemoveChange->mRemoveRange.width() );
            mRemovedPieces.append( otherRemoveChange->mRemovedPieces );
            result = true;
        }
        // other removed before?
        else if( otherRemoveChange->mRemoveRange.nextBehindEnd() == mRemoveRange.start() )
        {
            mRemoveRange.setStart( otherRemoveChange->mRemoveRange.start() );
            mRemovedPieces.prepend( otherRemoveChange->mRemovedPieces );
            result = true;
        }
    }

    return result;
}

AddressRange RemovePieceTableChange::apply( PieceTable* pieceTable ) const
{
    const Address oldLast = pieceTable->size() - 1;

    pieceTable->remove( mRemoveRange );

    return AddressRange( mRemoveRange.start(), oldLast );
}

AddressRange RemovePieceTableChange::revert( PieceTable* pieceTable ) const
{
    pieceTable->insert( mRemoveRange.start(), mRemovedPieces );

    return AddressRange( mRemoveRange.start(), pieceTable->size()-1 );
}

ArrayChangeMetrics RemovePieceTableChange::metrics() const
{
    return ArrayChangeMetrics::asReplacement( mRemoveRange.start(), mRemoveRange.width(), 0 );
}

RemovePieceTableChange::~RemovePieceTableChange() {}

}
