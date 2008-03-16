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

#include "piecetablechangehistory.h"

// lib
#include <ksectionlist.h>
#include <arraychangemetricslist.h>

namespace KPieceTable
{

void PieceTableChangeHistory::clear()
{
    while( !mChangeStack.isEmpty() )
         delete mChangeStack.pop();

    mMergeChanges = false;
    mChangeGroupOpened = 0;
    mAppliedChangesCount = 0;
    mBaseBeforeChangeIndex = 0;
    mAppliedChangesDataSize = 0;
}

void PieceTableChangeHistory::setBeforeCurrentChangeAsBase(bool hide)
{
    mBaseBeforeChangeIndex = hide ? -1 : mAppliedChangesCount;
    mMergeChanges = false;
}

bool PieceTableChangeHistory::appendChange( AbstractPieceTableChange *change )
{
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

    mAppliedChangesDataSize += change->dataSize();

    bool isNotMerged = true;
    if( mMergeChanges && mAppliedChangesCount>0 )
        isNotMerged = !mChangeStack.top()->merge( change );
    else
        mMergeChanges = true;

    if( isNotMerged )
    {
        mChangeStack.push( change );
        ++mAppliedChangesCount;
    }
    else
        delete change;

    return isNotMerged;
}


bool PieceTableChangeHistory::revertBeforeChange( PieceTable *pieceTable, int changeId,
                                                  KHE::KSectionList *changedRanges,
                                                  KHE::ArrayChangeMetricsList *changeList )
{
    int currentChangeId = mAppliedChangesCount;

    if( currentChangeId == changeId )
        return false;

    if( currentChangeId < changeId )
    {
        for( ; currentChangeId<changeId  ; ++currentChangeId )
        {
            const AbstractPieceTableChange *change = mChangeStack[currentChangeId];

            const KHE::KSection changedSection = change->apply( pieceTable );
            changedRanges->addSection( changedSection );

            const KHE::ArrayChangeMetrics changeMetrics = change->metrics();
            changeList->append( changeMetrics );

            mAppliedChangesDataSize += change->dataSize();
        }
    }
    else
    {
        for( --currentChangeId; changeId<=currentChangeId ; --currentChangeId )
        {
            const AbstractPieceTableChange *change = mChangeStack[currentChangeId];

            const KHE::KSection changedSection = change->revert( pieceTable );
            changedRanges->addSection( changedSection );

            KHE::ArrayChangeMetrics changeMetrics = change->metrics();
            changeMetrics.revert();
            changeList->append( changeMetrics );

            mAppliedChangesDataSize += change->dataSize();
        }
    }
    mAppliedChangesCount = changeId;
    mMergeChanges = false;

    return true;
}

}
