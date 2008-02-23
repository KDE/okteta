/***************************************************************************
                          piecetablechangehistory.cpp  -  description
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


#include "piecetablechangehistory.h"

// lib
#include <ksectionlist.h>
#include <kreplacementscope.h>

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
            // TODO: get freed change storage   droppedChange->storageOffset
            delete droppedChange;
        }
        while( mAppliedChangesCount < mChangeStack.count() );
    }

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
                                                  QList<KHE::ReplacementScope> *replacementList )
{
    int currentChangeId = mAppliedChangesCount;

    if( currentChangeId == changeId )
        return false;

    if( currentChangeId < changeId )
    {
        for( ; currentChangeId<changeId  ; ++currentChangeId )
        {
            AbstractPieceTableChange *change = mChangeStack[currentChangeId];
            const KHE::KSection changedSection =
                change->apply( pieceTable );
            changedRanges->addSection( changedSection );
            const KHE::ReplacementScope replacement = change->replacement();
            replacementList->append( replacement );
        }
    }
    else
    {
        for( --currentChangeId; changeId<=currentChangeId ; --currentChangeId )
        {
            AbstractPieceTableChange *change = mChangeStack[currentChangeId];
            const KHE::KSection changedSection =
                change->revert( pieceTable );
            changedRanges->addSection( changedSection );
            KHE::ReplacementScope replacement = change->replacement();
            replacement.revert();
            replacementList->append( replacement );
        }
    }
    mAppliedChangesCount = changeId;
    mMergeChanges = false;

    return true;
}

}
