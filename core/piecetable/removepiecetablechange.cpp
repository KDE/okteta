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

#include "removepiecetablechange.h"

// lib
#include "piecetable.h"
#include <ksection.h>
#include <kreplacementscope.h>
// KDE
#include <KLocale>


namespace KPieceTable
{

int RemovePieceTableChange::type() const { return RemoveId; }

QString RemovePieceTableChange::description() const
{
    return i18n( "Remove" );
}

bool RemovePieceTableChange::merge( const AbstractPieceTableChange *other )
{
    bool result = false;
    if( other->type() == RemoveId )
    {
        const RemovePieceTableChange *otherRemoveChange = static_cast<const RemovePieceTableChange *>( other );
        // other removed at the same start?
        if( mRemoveSection.start() == otherRemoveChange->mRemoveSection.start() )
        {
            mRemoveSection.moveEndBy( otherRemoveChange->mRemoveSection.width() );
            mRemovedPieces.append( otherRemoveChange->mRemovedPieces );
            result = true;
        }
        // other removed before?
        else if( otherRemoveChange->mRemoveSection.end()+1 == mRemoveSection.start() )
        {
            mRemoveSection.setStart( otherRemoveChange->mRemoveSection.start() );
            mRemovedPieces.prepend( otherRemoveChange->mRemovedPieces );
            result = true;
        }
    }

    return result;
}

KHE::KSection RemovePieceTableChange::apply( PieceTable *pieceTable )
{
    const int oldLast = pieceTable->size() - 1;

    pieceTable->remove( mRemoveSection );

    return KHE::KSection( mRemoveSection.start(), oldLast );
}

KHE::KSection RemovePieceTableChange::revert( PieceTable *pieceTable )
{
    pieceTable->insert( mRemoveSection.start(), mRemovedPieces );

    return KHE::KSection( mRemoveSection.start(), pieceTable->size()-1 );
}

KHE::ReplacementScope RemovePieceTableChange::replacement() const
{ return KHE::ReplacementScope(mRemoveSection.start(),mRemoveSection.width(),0); }

RemovePieceTableChange::~RemovePieceTableChange() {}

}
