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

#include "insertpiecetablechange.h"


// lib
#include "piecetable.h"
//
#include <section.h>
#include <arraychangemetrics.h>
// KDE
#include <KLocale>


namespace KPieceTable
{

int InsertPieceTableChange::type() const { return InsertId; }

QString InsertPieceTableChange::description() const
{
    return i18nc( "name of the change", "Insert" );
}

int InsertPieceTableChange::storageOffset() const { return mStorageOffset; }

bool InsertPieceTableChange::merge( const AbstractPieceTableChange *other )
{
// TODO: remove me again after synching solved
// return false;
    bool result = false;
    if( other->type() == InsertId )
    {
        const InsertPieceTableChange *otherInsertChange = static_cast<const InsertPieceTableChange *>( other );
        if( mInsertOffset+mInsertLength == otherInsertChange->mInsertOffset )
        {
            mInsertLength += otherInsertChange->mInsertLength;
            result = true;
        }
    }

    return result;
}

KDE::Section InsertPieceTableChange::apply( PieceTable *pieceTable ) const
{
    pieceTable->insert( mInsertOffset, mInsertLength, mStorageOffset );

    return KDE::Section( mInsertOffset, pieceTable->size()-1 );
}

KDE::Section InsertPieceTableChange::revert( PieceTable *pieceTable ) const
{
    const int oldLast = pieceTable->size() - 1;
    pieceTable->remove( KDE::Section::fromWidth(mInsertOffset,mInsertLength) );
    return KDE::Section( mInsertOffset, oldLast );
}

KDE::ArrayChangeMetrics InsertPieceTableChange::metrics() const
{
    return KDE::ArrayChangeMetrics::asReplacement( mInsertOffset, 0, mInsertLength );
}

int InsertPieceTableChange::dataSize() const { return mInsertLength; }

InsertPieceTableChange::~InsertPieceTableChange() {}

}
