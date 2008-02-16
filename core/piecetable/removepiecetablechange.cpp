/***************************************************************************
                          removepiecetablechange.cpp  -  description
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
