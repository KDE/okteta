/***************************************************************************
                          insertpiecetablechange.cpp  -  description
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

#include "insertpiecetablechange.h"


// lib
#include "piecetable.h"
#include <ksection.h>
#include <kreplacementscope.h>
// KDE
#include <KLocale>


namespace KPieceTable
{

int InsertPieceTableChange::type() const { return InsertId; }

QString InsertPieceTableChange::description() const
{
    return i18n( "Insert" );
}

bool InsertPieceTableChange::merge( const AbstractPieceTableChange *other )
{
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

KHE::KSection InsertPieceTableChange::apply( PieceTable *pieceTable )
{
    pieceTable->insert( mInsertOffset, mInsertLength, mStorageOffset );

    return KHE::KSection( mInsertOffset, pieceTable->size()-1 );
}

KHE::KSection InsertPieceTableChange::revert( PieceTable *pieceTable )
{
    const int oldLast = pieceTable->size() - 1;
    pieceTable->remove( KHE::KSection::fromWidth(mInsertOffset,mInsertLength) );
    return KHE::KSection( mInsertOffset, oldLast );
}

KHE::ReplacementScope InsertPieceTableChange::replacement() const
{ return KHE::ReplacementScope(mInsertOffset,0,mInsertLength); }

InsertPieceTableChange::~InsertPieceTableChange() {}

}
