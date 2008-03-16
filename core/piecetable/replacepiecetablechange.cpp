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

#include "replacepiecetablechange.h"

// lib
#include "piecetable.h"
//
#include <ksection.h>
#include <arraychangemetrics.h>
// KDE
#include <KLocale>


namespace KPieceTable
{

int ReplacePieceTableChange::type() const { return ReplaceId; }

QString ReplacePieceTableChange::description() const
{
    return i18n( "Replace" );
}

bool ReplacePieceTableChange::merge( const AbstractPieceTableChange *other )
{
    bool result = false;
    if( other->type() == ReplaceId )
    {
        const ReplacePieceTableChange *otherReplaceChange = static_cast<const ReplacePieceTableChange *>( other );
        // other replaced after?
        if( mRemoveSection.start()+mInsertLength == otherReplaceChange->mRemoveSection.start() )
        {
            mRemoveSection.moveEndBy( otherReplaceChange->mRemoveSection.width() );
            mInsertLength += otherReplaceChange->mInsertLength;
            mRemovedPieces.append( otherReplaceChange->mRemovedPieces );
            result = true;
        }
        // other replaced before would be two swapped ranges in the change buffer, if this ever needed/wanted?
    }

    return result;
}

KHE::KSection ReplacePieceTableChange::apply( PieceTable *pieceTable ) const
{
    const int oldSize = pieceTable->size();

    pieceTable->replace( mRemoveSection, mInsertLength, mStorageOffset );

    const int newSize = pieceTable->size();
    const int lastChanged = ( newSize == oldSize ) ? mRemoveSection.end() :
                            ( newSize > oldSize ) ?  newSize - 1 :
                                                     oldSize - 1;
    return KHE::KSection( mRemoveSection.start(), lastChanged );
}

KHE::KSection ReplacePieceTableChange::revert( PieceTable *pieceTable ) const
{
    const int oldSize = pieceTable->size();

    const KHE::KSection insertedSection = KHE::KSection::fromWidth( mRemoveSection.start(), mInsertLength );
    pieceTable->replace( insertedSection, mRemovedPieces );

    const int newSize = pieceTable->size();
    const int lastChanged = ( newSize == oldSize ) ? insertedSection.end() :
                            ( newSize > oldSize ) ?  newSize - 1 :
                                                     oldSize - 1;
    return KHE::KSection( mRemoveSection.start(), lastChanged );
}

KHE::ArrayChangeMetrics ReplacePieceTableChange::metrics() const
{
    return KHE::ArrayChangeMetrics::asReplacement( mRemoveSection.start(), mRemoveSection.width(), mInsertLength );
}

int ReplacePieceTableChange::dataSize() const { return mInsertLength; }

ReplacePieceTableChange::~ReplacePieceTableChange() {}

}
