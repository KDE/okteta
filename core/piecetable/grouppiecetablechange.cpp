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

#include "grouppiecetablechange.h"

// lib
#include "piecetable.h"
//
#include <addressrangelist.h>
#include <arraychangemetricslist.h>
// KDE
#include <KLocale>


namespace KPieceTable
{

int GroupPieceTableChange::type() const { return GroupId; }

QString GroupPieceTableChange::description() const
{
    return mDescription;
}

bool GroupPieceTableChange::merge( const AbstractPieceTableChange* other )
{
    bool result = false;

    if( !mChangeStack.isEmpty() )
        result = mChangeStack.last()->merge( other );

    return result;
}

AddressRange GroupPieceTableChange::apply( PieceTable* pieceTable ) const
{
Q_UNUSED( pieceTable )
//     pieceTable->insert( mInsertOffset, mInsertLength, mStorageOffset );

    return AddressRange();//( mInsertOffset, pieceTable->size()-1 );
}

AddressRange GroupPieceTableChange::revert( PieceTable* pieceTable ) const
{
Q_UNUSED( pieceTable )
//     const int oldLast = pieceTable->size() - 1;
//     pieceTable->remove( AddressRange::fromWidth(mInsertOffset,mInsertLength) );
    return AddressRange();//( mInsertOffset, oldLast );
}

ArrayChangeMetrics GroupPieceTableChange::metrics() const
{
    return ArrayChangeMetrics::asReplacement( 0, 0, 0);
}

bool GroupPieceTableChange::appendChange( AbstractPieceTableChange* change )
{
#if 0
    // chop unapplied changes
    if( mAppliedChangesCount < mChangeStack.count() )
    {
        // hide baseindex if needed
        if( mBaseBeforeChangeIndex > mAppliedChangesCount )
            mBaseBeforeChangeIndex = -1;
        do
        {
            AbstractPieceTableChange *droppedChange = mChangeStack.pop();
            delete droppedChange;
        }
        while( mAppliedChangesCount < mChangeStack.count() );
    }
#endif
    mAppliedChangesDataSize += change->dataSize();

    bool isNotMerged = true;
    if( mTryToMergeAppendedChange && mAppliedChangesCount>0 )
        isNotMerged = !mChangeStack.top()->merge( change );
    else
        mTryToMergeAppendedChange = true;

    if( isNotMerged )
    {
        mChangeStack.push( change );
        ++mAppliedChangesCount;
    }
    else
        delete change;

    return isNotMerged;
}


AddressRangeList GroupPieceTableChange::applyGroup( PieceTable* pieceTable ) const
{
    AddressRangeList result;

    foreach( AbstractPieceTableChange* change, mChangeStack )
    {
        if( change->type() == AbstractPieceTableChange::GroupId )
        {
            const GroupPieceTableChange* groupChange = static_cast<const GroupPieceTableChange*>(change);
            const AddressRangeList changedRangeList = groupChange->applyGroup( pieceTable );
            result.addAddressRangeList( changedRangeList );
        }
        else
            result.append( change->apply(pieceTable) );
    }

    return result;
}

AddressRangeList GroupPieceTableChange::revertGroup( PieceTable* pieceTable ) const
{
    AddressRangeList result;

    QStack<AbstractPieceTableChange*>::ConstIterator it = mChangeStack.end();
    while( it != mChangeStack.begin() )
    {
        --it;
        AbstractPieceTableChange *change = *it;
        if( change->type() == AbstractPieceTableChange::GroupId )
        {
            const GroupPieceTableChange* groupChange = static_cast<const GroupPieceTableChange*>(change);
            const AddressRangeList changedRangeList = groupChange->revertGroup( pieceTable );
            result.addAddressRangeList( changedRangeList );
        }
        else
            result.append( change->revert(pieceTable) );
    }

    return result;
}

ArrayChangeMetricsList GroupPieceTableChange::groupMetrics( bool reverted ) const
{
    ArrayChangeMetricsList result;

    foreach( AbstractPieceTableChange* change, mChangeStack )
    {
        if( change->type() == AbstractPieceTableChange::GroupId )
        {
            const GroupPieceTableChange *groupChange = static_cast<const GroupPieceTableChange*>(change);
            const ArrayChangeMetricsList metricsList = groupChange->groupMetrics( reverted );
            result += metricsList;
        }
        else
        {
            ArrayChangeMetrics changeMetrics = change->metrics();
            if( reverted )
                changeMetrics.revert();
            result.append( changeMetrics );
        }
    }

    return result;
}


Size GroupPieceTableChange::dataSize() const
{
    return mAppliedChangesDataSize;
}

GroupPieceTableChange::~GroupPieceTableChange()
{
    while( ! mChangeStack.isEmpty() )
         delete mChangeStack.pop();
}

}
