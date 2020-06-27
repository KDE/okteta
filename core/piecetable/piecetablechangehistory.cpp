/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "piecetablechangehistory.hpp"

// lib
#include "grouppiecetablechange.hpp"
//
#include <addressrangelist.hpp>
#include <arraychangemetricslist.hpp>

namespace KPieceTable {

void PieceTableChangeHistory::clear()
{
    while (!mChangeStack.isEmpty()) {
        delete mChangeStack.pop();
    }

    mTryToMergeAppendedChange = false;
    mAppliedChangesCount = 0;
    mBaseBeforeChangeIndex = 0;
    mAppliedChangesDataSize = 0;

    mActiveGroupChange = nullptr;
}

void PieceTableChangeHistory::getChangeData(ArrayChangeMetrics* metrics, Address* storageOffset,
                                            int versionIndex) const
{
    AbstractPieceTableChange* change = mChangeStack.at(versionIndex);
    *metrics = change->metrics();
    *storageOffset = change->storageOffset();
}

void PieceTableChangeHistory::setBeforeCurrentChangeAsBase(bool hide)
{
    mBaseBeforeChangeIndex = hide ? -1 : mAppliedChangesCount;
    mTryToMergeAppendedChange = false;
}

void PieceTableChangeHistory::openGroupedChange(const QString& description)
{
    auto* groupChange = new GroupPieceTableChange(mActiveGroupChange, description);

    appendChange(groupChange);
    mActiveGroupChange = groupChange;
}

void PieceTableChangeHistory::closeGroupedChange(const QString& description)
{
    if (mActiveGroupChange) {
        if (!description.isEmpty()) {
            mActiveGroupChange->setDescription(description);
        }
        mActiveGroupChange = mActiveGroupChange->parent();
    }
}

void PieceTableChangeHistory::finishChange()
{
    if (mActiveGroupChange) {
        mActiveGroupChange->finishChange();
    } else {
        mTryToMergeAppendedChange = false;
    }
}

bool PieceTableChangeHistory::appendChange(AbstractPieceTableChange* change)
{
    // chop unapplied changes
    if (mAppliedChangesCount < mChangeStack.count()) {
        // hide baseindex if needed
        if (mBaseBeforeChangeIndex > mAppliedChangesCount) {
            mBaseBeforeChangeIndex = -1;
        }
        do {
            AbstractPieceTableChange* droppedChange = mChangeStack.pop();
            delete droppedChange;
        } while (mAppliedChangesCount < mChangeStack.count());
    }

    mAppliedChangesDataSize += change->dataSize();

    bool isNotMerged = true;
    if (mActiveGroupChange) {
        mActiveGroupChange->appendChange(change);
        isNotMerged = false; // TODO: hack for as long as subgroups are not undoable
    } else {
        if (mTryToMergeAppendedChange && mAppliedChangesCount > 0) {
            isNotMerged = !mChangeStack.top()->merge(change);
        } else {
            mTryToMergeAppendedChange = true;
        }

        if (isNotMerged) {
            mChangeStack.push(change);
            ++mAppliedChangesCount;
        } else {
            delete change;
        }
    }

    return isNotMerged;
}

bool PieceTableChangeHistory::revertBeforeChange(PieceTable* pieceTable, int changeId,
                                                 AddressRangeList* changedRanges,
                                                 ArrayChangeMetricsList* changeList)
{
    int currentChangeId = mAppliedChangesCount;

    if (currentChangeId == changeId) {
        return false;
    }

    // close any grouped changes
    while (mActiveGroupChange) {
        mActiveGroupChange = mActiveGroupChange->parent();
    }

    if (currentChangeId < changeId) {
        for (; currentChangeId < changeId; ++currentChangeId) {
            const AbstractPieceTableChange* change = mChangeStack[currentChangeId];

            if (change->type() == AbstractPieceTableChange::GroupId) {
                const auto* groupChange = static_cast<const GroupPieceTableChange*>(change);
                const AddressRangeList changedRangeList = groupChange->applyGroup(pieceTable);
                changedRanges->addAddressRangeList(changedRangeList);

                const QVector<ArrayChangeMetrics> changeMetricsList = groupChange->groupMetrics();
                *changeList += changeMetricsList;
            } else {
                const AddressRange changedRange = change->apply(pieceTable);
                changedRanges->addAddressRange(changedRange);

                const ArrayChangeMetrics changeMetrics = change->metrics();
                changeList->append(changeMetrics);
            }

            mAppliedChangesDataSize += change->dataSize();
        }
    } else {
        for (--currentChangeId; changeId <= currentChangeId; --currentChangeId) {
            const AbstractPieceTableChange* change = mChangeStack[currentChangeId];

            if (change->type() == AbstractPieceTableChange::GroupId) {
                const auto* groupChange = static_cast<const GroupPieceTableChange*>(change);
                const AddressRangeList changedRangeList = groupChange->revertGroup(pieceTable);
                changedRanges->addAddressRangeList(changedRangeList);

                const QVector<ArrayChangeMetrics> changeMetricsList = groupChange->groupMetrics(true);
                *changeList += changeMetricsList;
            } else {
                const AddressRange changedRange = change->revert(pieceTable);
                changedRanges->addAddressRange(changedRange);

                ArrayChangeMetrics changeMetrics = change->metrics();
                changeMetrics.revert();
                changeList->append(changeMetrics);
            }
            mAppliedChangesDataSize -= change->dataSize();
        }
    }
    mAppliedChangesCount = changeId;
    mTryToMergeAppendedChange = false;

    return true;
}

}
