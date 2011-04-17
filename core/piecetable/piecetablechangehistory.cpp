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

#include "piecetablechangehistory.h"

// lib
#include "grouppiecetablechange.h"
//
#include <addressrangelist.h>
#include <arraychangemetricslist.h>


namespace KPieceTable
{

void PieceTableChangeHistory::clear()
{
    while( !mChangeStack.isEmpty() )
         delete mChangeStack.pop();

    mTryToMergeAppendedChange = false;
    mAppliedChangesCount = 0;
    mBaseBeforeChangeIndex = 0;
    mAppliedChangesDataSize = 0;

    mActiveGroupChange = 0;
}

void PieceTableChangeHistory::getChangeData( ArrayChangeMetrics* metrics, Address* storageOffset,
                                             int versionIndex ) const
{
    AbstractPieceTableChange *change = mChangeStack.at( versionIndex );
    *metrics = change->metrics();
    *storageOffset = change->storageOffset();
}


void PieceTableChangeHistory::setBeforeCurrentChangeAsBase( bool hide )
{
    mBaseBeforeChangeIndex = hide ? -1 : mAppliedChangesCount;
    mTryToMergeAppendedChange = false;
}

void PieceTableChangeHistory::openGroupedChange( const QString& description )
{
    GroupPieceTableChange *groupChange = new GroupPieceTableChange( mActiveGroupChange, description );

    appendChange( groupChange );
    mActiveGroupChange = groupChange;
}

void PieceTableChangeHistory::closeGroupedChange(const QString & description )
{
    if( mActiveGroupChange != 0 )
    {
        if( !description.isEmpty() )
            mActiveGroupChange->setDescription( description );
        mActiveGroupChange = mActiveGroupChange->parent();
    }
}

void PieceTableChangeHistory::finishChange()
{
    if( mActiveGroupChange != 0 )
        mActiveGroupChange->finishChange();
    else
        mTryToMergeAppendedChange = false;
}

bool PieceTableChangeHistory::appendChange( AbstractPieceTableChange* change )
{
    // chop unapplied changes
    if( mAppliedChangesCount < mChangeStack.count() )
    {
        // hide baseindex if needed
        if( mBaseBeforeChangeIndex > mAppliedChangesCount )
            mBaseBeforeChangeIndex = -1;
        do
        {
            AbstractPieceTableChange* droppedChange = mChangeStack.pop();
            delete droppedChange;
        }
        while( mAppliedChangesCount < mChangeStack.count() );
    }

    mAppliedChangesDataSize += change->dataSize();

    bool isNotMerged = true;
    if( mActiveGroupChange != 0 )
    {
        mActiveGroupChange->appendChange( change );
        isNotMerged = false; // TODO: hack for as long as subgroups are not undoable
    }
    else
    {
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
    }

    return isNotMerged;
}


bool PieceTableChangeHistory::revertBeforeChange( PieceTable* pieceTable, int changeId,
                                                  AddressRangeList* changedRanges,
                                                  ArrayChangeMetricsList* changeList )
{
    int currentChangeId = mAppliedChangesCount;

    if( currentChangeId == changeId )
        return false;

    // close any grouped changes
    while( mActiveGroupChange != 0 )
        mActiveGroupChange = mActiveGroupChange->parent();

    if( currentChangeId < changeId )
    {
        for( ; currentChangeId<changeId  ; ++currentChangeId )
        {
            const AbstractPieceTableChange* change = mChangeStack[currentChangeId];

            if( change->type() == AbstractPieceTableChange::GroupId )
            {
                const GroupPieceTableChange* groupChange = static_cast<const GroupPieceTableChange*>(change);
                const AddressRangeList changedRangeList = groupChange->applyGroup( pieceTable );
                changedRanges->addAddressRangeList( changedRangeList );

                const QList<ArrayChangeMetrics> changeMetricsList = groupChange->groupMetrics();
                *changeList += changeMetricsList;
            }
            else
            {
                const AddressRange changedRange = change->apply( pieceTable );
                changedRanges->addAddressRange( changedRange );

                const ArrayChangeMetrics changeMetrics = change->metrics();
                changeList->append( changeMetrics );
            }

            mAppliedChangesDataSize += change->dataSize();
        }
    }
    else
    {
        for( --currentChangeId; changeId<=currentChangeId ; --currentChangeId )
        {
            const AbstractPieceTableChange* change = mChangeStack[currentChangeId];

            if( change->type() == AbstractPieceTableChange::GroupId )
            {
                const GroupPieceTableChange* groupChange = static_cast<const GroupPieceTableChange*>(change);
                const AddressRangeList changedRangeList = groupChange->revertGroup( pieceTable );
                changedRanges->addAddressRangeList( changedRangeList );

                const QList<ArrayChangeMetrics> changeMetricsList = groupChange->groupMetrics( true );
                *changeList += changeMetricsList;
            }
            else
            {
                const AddressRange changedRange = change->revert( pieceTable );
                changedRanges->addAddressRange( changedRange );

                ArrayChangeMetrics changeMetrics = change->metrics();
                changeMetrics.revert();
                changeList->append( changeMetrics );
            }
            mAppliedChangesDataSize -= change->dataSize();
        }
    }
    mAppliedChangesCount = changeId;
    mTryToMergeAppendedChange = false;

    return true;
}

}
