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

#include "replacepiecetablechange.h"

// lib
#include "piecetable.h"
//
#include <arraychangemetrics.h>
// KDE
#include <KLocale>


namespace KPieceTable
{

int ReplacePieceTableChange::type() const { return ReplaceId; }

QString ReplacePieceTableChange::description() const
{
    return i18nc( "name of the change", "Replace" );
}

Address ReplacePieceTableChange::storageOffset() const { return mStorageOffset; }


bool ReplacePieceTableChange::merge( const AbstractPieceTableChange* other )
{
// TODO: remove me again after synching solved
// return false;
    bool result = false;

    if( other->type() == ReplaceId )
    {
        const ReplacePieceTableChange* otherReplaceChange = static_cast<const ReplacePieceTableChange *>( other );
        // other replaced after?
        if( mRemoveRange.start()+mInsertLength == otherReplaceChange->mRemoveRange.start() )
        {
            mRemoveRange.moveEndBy( otherReplaceChange->mRemoveRange.width() );
            mInsertLength += otherReplaceChange->mInsertLength;
            mRemovedPieces.append( otherReplaceChange->mRemovedPieces );
            result = true;
        }
        // other replaced before would be two swapped ranges in the change buffer, if this ever needed/wanted?
    }

    return result;
}

AddressRange ReplacePieceTableChange::apply( PieceTable* pieceTable ) const
{
    const Size oldSize = pieceTable->size();

    pieceTable->replace( mRemoveRange, mInsertLength, mStorageOffset );

    const Size newSize = pieceTable->size();
    const Address lastChanged = ( newSize == oldSize ) ? mRemoveRange.end() :
                                ( newSize > oldSize ) ?  newSize - 1 :
                                                         oldSize - 1;
    return AddressRange( mRemoveRange.start(), lastChanged );
}

AddressRange ReplacePieceTableChange::revert( PieceTable* pieceTable ) const
{
    const Size oldSize = pieceTable->size();

    const AddressRange insertedSection = AddressRange::fromWidth( mRemoveRange.start(), mInsertLength );
    pieceTable->replace( insertedSection, mRemovedPieces );

    const Size newSize = pieceTable->size();
    const Address lastChanged = ( newSize == oldSize ) ? insertedSection.end() :
                                ( newSize > oldSize ) ?  newSize - 1 :
                                                         oldSize - 1;
    return AddressRange( mRemoveRange.start(), lastChanged );
}

ArrayChangeMetrics ReplacePieceTableChange::metrics() const
{
    return ArrayChangeMetrics::asReplacement( mRemoveRange.start(), mRemoveRange.width(), mInsertLength );
}

Size ReplacePieceTableChange::dataSize() const { return mInsertLength; }

ReplacePieceTableChange::~ReplacePieceTableChange() {}

}
