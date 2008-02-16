/***************************************************************************
                          revertablepiecetable.cpp  -  description
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

bool RevertablePieceTable::insert( int dataOffset, int length, int storageOffset )
{
    mPieceTable.insert( dataOffset, length, storageOffset );

    InsertPieceTableChange *change =
        new InsertPieceTableChange( dataOffset, length, storageOffset );

    return mChangeHistory.appendChange( change );
}

bool RevertablePieceTable::remove( const KHE::KSection &removeSection )
{
    const PieceList removedPieces = mPieceTable.remove( removeSection );

    RemovePieceTableChange *change =
        new RemovePieceTableChange( removeSection, removedPieces );

    return mChangeHistory.appendChange( change );
}

bool RevertablePieceTable::remove( int start, int length ) { return remove( KHE::KSection::fromWidth(start,length) );}

bool RevertablePieceTable::replace( const KHE::KSection &removeSection, int insertLength, int storageOffset )
{
    const PieceList replacedPieces = mPieceTable.remove( removeSection );
    mPieceTable.insert( removeSection.start(), insertLength, storageOffset );

    ReplacePieceTableChange *change =
        new ReplacePieceTableChange( removeSection, insertLength, storageOffset, replacedPieces );

    return mChangeHistory.appendChange( change );
}
bool RevertablePieceTable::replace( int removeStart, int removeLength, int insertLength, int storageOffset )
{ return replace( KHE::KSection::fromWidth(removeStart,removeLength), insertLength, storageOffset ); }


bool RevertablePieceTable::swap( int firstStart, const KHE::KSection &secondSection )
{
    mPieceTable.swap( firstStart, secondSection );

    SwapRangesPieceTableChange *change =
        new SwapRangesPieceTableChange( firstStart, secondSection);

    return mChangeHistory.appendChange( change );
}

bool RevertablePieceTable::swap( int firstStart, int secondStart, int secondLength )
{ return swap( firstStart, KHE::KSection::fromWidth(secondStart,secondLength) ); }

bool RevertablePieceTable::replaceOne( int dataOffset, int storageOffset )
{
    const Piece replacedPiece = mPieceTable.replaceOne( dataOffset, storageOffset );
    const PieceList replacedPieces( replacedPiece );

    ReplacePieceTableChange *change =
        new ReplacePieceTableChange( KHE::KSection::fromWidth(dataOffset,1), 1, storageOffset, replacedPieces );

    return mChangeHistory.appendChange( change );
}

bool RevertablePieceTable::revertBeforeChange( int changeId,
                             KHE::KSectionList *changedRanges, QList<KHE::ReplacementScope> *replacementList )
{
    return mChangeHistory.revertBeforeChange( &mPieceTable, changeId, changedRanges, replacementList );
}

}
