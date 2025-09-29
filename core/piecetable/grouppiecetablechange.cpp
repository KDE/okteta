/*
    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "grouppiecetablechange.hpp"

//
#include <addressrangelist.hpp>
#include <arraychangemetricslist.hpp>
// Std
#include <utility>

namespace KPieceTable {

int GroupPieceTableChange::type() const { return GroupId; }

QString GroupPieceTableChange::description() const
{
    return mDescription;
}

bool GroupPieceTableChange::merge(const AbstractPieceTableChange* other)
{
    bool result = false;

    if (!mChangeStack.empty()) {
        result = mChangeStack.back()->merge(other);
    }

    return result;
}

AddressRange GroupPieceTableChange::apply(PieceTable* pieceTable) const
{
    Q_UNUSED(pieceTable)
//     pieceTable->insert( mInsertOffset, mInsertLength, mStorageOffset );

    return {}; // ( mInsertOffset, pieceTable->size()-1 );
}

AddressRange GroupPieceTableChange::revert(PieceTable* pieceTable) const
{
    Q_UNUSED(pieceTable)
//     const int oldLast = pieceTable->size() - 1;
//     pieceTable->remove( AddressRange::fromWidth(mInsertOffset,mInsertLength) );
    return {}; // ( mInsertOffset, oldLast );
}

ArrayChangeMetrics GroupPieceTableChange::metrics() const
{
    return ArrayChangeMetrics::asReplacement(0, 0, 0);
}

bool GroupPieceTableChange::appendChange(std::unique_ptr<AbstractPieceTableChange>&& change)
{
#if 0
    // chop unapplied changes
    if (mAppliedChangesCount < mChangeStack.size()) {
        // hide baseindex if needed
        if (mBaseBeforeChangeIndex > mAppliedChangesCount) {
            mBaseBeforeChangeIndex = -1;
        }
        do
        {
            AbstractPieceTableChange* droppedChange = mChangeStack.pop();
            delete droppedChange;
        }
        while (mAppliedChangesCount < mChangeStack.size());
    }
#endif
    mAppliedChangesDataSize += change->dataSize();

    bool isNotMerged = true;
    if (mTryToMergeAppendedChange && mAppliedChangesCount > 0) {
        isNotMerged = !mChangeStack.back()->merge(change.get());
    } else {
        mTryToMergeAppendedChange = true;
    }

    if (isNotMerged) {
        mChangeStack.emplace_back(std::move(change));
        ++mAppliedChangesCount;
    }

    return isNotMerged;
}

AddressRangeList GroupPieceTableChange::applyGroup(PieceTable* pieceTable) const
{
    AddressRangeList result;

    for (const auto& change : mChangeStack) {
        if (change->type() == AbstractPieceTableChange::GroupId) {
            const auto* const groupChange = static_cast<const GroupPieceTableChange*>(change.get());
            const AddressRangeList changedRangeList = groupChange->applyGroup(pieceTable);
            result.addAddressRangeList(changedRangeList);
        } else {
            result.append(change->apply(pieceTable));
        }
    }

    return result;
}

AddressRangeList GroupPieceTableChange::revertGroup(PieceTable* pieceTable) const
{
    AddressRangeList result;

    auto it = mChangeStack.cend();
    while (it != mChangeStack.cbegin()) {
        --it;
        const auto& change = *it;
        if (change->type() == AbstractPieceTableChange::GroupId) {
            const auto* const groupChange = static_cast<const GroupPieceTableChange*>(change.get());
            const AddressRangeList changedRangeList = groupChange->revertGroup(pieceTable);
            result.addAddressRangeList(changedRangeList);
        } else {
            result.append(change->revert(pieceTable));
        }
    }

    return result;
}

ArrayChangeMetricsList GroupPieceTableChange::groupMetrics(bool reverted) const
{
    ArrayChangeMetricsList result;

    for (const auto& change : mChangeStack) {
        if (change->type() == AbstractPieceTableChange::GroupId) {
            const auto* const groupChange = static_cast<const GroupPieceTableChange*>(change.get());
            const ArrayChangeMetricsList metricsList = groupChange->groupMetrics(reverted);
            result += metricsList;
        } else {
            ArrayChangeMetrics changeMetrics = change->metrics();
            if (reverted) {
                changeMetrics.revert();
            }
            result.append(changeMetrics);
        }
    }

    return result;
}

Size GroupPieceTableChange::dataSize() const
{
    return mAppliedChangesDataSize;
}

}
