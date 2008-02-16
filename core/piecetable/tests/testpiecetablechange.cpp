/***************************************************************************
                          testpiecetablechange.cpp  -  description
                             -------------------
    begin                : Mon Feb 11 2008
    copyright            : 2008 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License version 2 as published by the Free Software Foundation.       *
 *                                                                         *
 ***************************************************************************/

#include "testpiecetablechange.h"

// lib
#include <piecetable/piecetable.h>
#include <ksection.h>
// Qt
#include <QtCore/QString>

namespace KPieceTable 
{

int TestPieceTableChange::type() const { return mTypeId; }

QString TestPieceTableChange::description() const
{
    return mDescription;
}

bool TestPieceTableChange::merge( const AbstractPieceTableChange *other )
{
    bool result = false;
    if( other->type() == mTypeId )
    {
        const TestPieceTableChange *otherTestChange = static_cast<const TestPieceTableChange *>( other );
        mDescription += otherTestChange->mDescription;
        result = true;
    }

    return result;
}

KHE::KSection TestPieceTableChange::apply( PieceTable *pieceTable )
{
    pieceTable->replaceOne( mPosition, mStoragePosition, mStorageId );

    return KHE::KSection( mPosition, mPosition );
}

KHE::KSection TestPieceTableChange::revert( PieceTable *pieceTable )
{
    const Piece replaced = pieceTable->replaceOne( mPosition, mReplacedStoragePosition, mReplacedStorageId );

    return KHE::KSection( mPosition, mPosition );
}

TestPieceTableChange::~TestPieceTableChange() {}

}
