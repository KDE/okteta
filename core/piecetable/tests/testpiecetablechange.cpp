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

#include "testpiecetablechange.h"

// lib
#include <piecetable/piecetable.h>
#include <arraychangemetrics.h>
// Qt
#include <QtCore/QString>


namespace KPieceTable
{

int TestPieceTableChange::type() const { return mTypeId; }

QString TestPieceTableChange::description() const
{
    return mDescription;
}

bool TestPieceTableChange::merge( const AbstractPieceTableChange* other )
{
    bool result = false;
    if( other->type() == mTypeId )
    {
        const TestPieceTableChange* otherTestChange = static_cast<const TestPieceTableChange*>( other );
        mDescription += otherTestChange->mDescription;
        result = true;
    }

    return result;
}

AddressRange TestPieceTableChange::apply( PieceTable* pieceTable ) const
{
    pieceTable->replaceOne( mPosition, mStoragePosition, mStorageId );

    return AddressRange( mPosition, mPosition );
}

AddressRange TestPieceTableChange::revert( PieceTable* pieceTable ) const
{
    const Piece replaced = pieceTable->replaceOne( mPosition, mReplacedStoragePosition, mReplacedStorageId );

    return AddressRange( mPosition, mPosition );
}

ArrayChangeMetrics TestPieceTableChange::metrics() const
{
    return ArrayChangeMetrics::asReplacement( mPosition, 1, 1 );
}

int TestPieceTableChange::dataSize() const { return 1; }

TestPieceTableChange::~TestPieceTableChange() {}

}
