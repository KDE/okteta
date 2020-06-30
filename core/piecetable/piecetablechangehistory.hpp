/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KPIECETABLE_PIECETABLECHANGEHISTORY_HPP
#define KPIECETABLE_PIECETABLECHANGEHISTORY_HPP

// lib
#include "abstractpiecetablechange.hpp"
// Qt
#include <QStack>
#include <QString>

namespace Okteta {
class AddressRangeList;
class ArrayChangeMetricsList;
}

namespace KPieceTable {

using AddressRangeList = Okteta::AddressRangeList;
using ArrayChangeMetricsList = Okteta::ArrayChangeMetricsList;
class GroupPieceTableChange;

class PieceTableChangeHistory
{
public:
    PieceTableChangeHistory();
    PieceTableChangeHistory(const PieceTableChangeHistory&) = delete;

    ~PieceTableChangeHistory();

    PieceTableChangeHistory& operator=(const PieceTableChangeHistory&) = delete;

public:
    void clear();
    /// returns true, if a new change is appended, false if merged
    bool appendChange(AbstractPieceTableChange* change);

    /**
     * @param pieceTable
     * @param changeId
     * @param changedRanges
     * @param changeList
     * @return true if there were changes to revert, false otherwise
     */
    bool revertBeforeChange(PieceTable* pieceTable, int changeId,
                            AddressRangeList* changedRanges, ArrayChangeMetricsList* changeList);

    ///
    void openGroupedChange(const QString& description);   // TODO: hand over description? user change id?
    void closeGroupedChange(const QString& description);
    /// closes the current change, so any following operation will not be tried to merge
    void finishChange();

    void setBeforeCurrentChangeAsBase(bool hide);

public:
    /// @return number of changes in the history
    int count() const;
    /// @return number of changes currently applied
    int appliedChangesCount() const;
    /// @return description of the change with the id changeId
    QString changeDescription(int changeId) const;
    /// @return description of the change at the head, empty if there is none
    QString headChangeDescription() const;
    /// @return true if the current change is the base
    bool isAtBase() const;
    /// @return size of the data used by the applied changes
    Size appliedChangesDataSize() const;

    void getChangeData(ArrayChangeMetrics* metrics, Address* storageOffset, int versionIndex) const;

private:
    /// if true, try to merge changes
    bool mTryToMergeAppendedChange = false;
    ///
    int mAppliedChangesCount = 0;
    ///
    int mBaseBeforeChangeIndex = 0;
    ///
    QStack<AbstractPieceTableChange*> mChangeStack;
    ///
    Size mAppliedChangesDataSize = 0;

    /// if 0, there is no
    GroupPieceTableChange* mActiveGroupChange = nullptr;
};

inline PieceTableChangeHistory::PieceTableChangeHistory() = default;

inline PieceTableChangeHistory::~PieceTableChangeHistory() { clear(); }

inline int PieceTableChangeHistory::count()                     const { return mChangeStack.count(); }
inline int PieceTableChangeHistory::appliedChangesCount()       const { return mAppliedChangesCount; }
inline QString PieceTableChangeHistory::headChangeDescription() const { return changeDescription(count() - 1); }
inline bool PieceTableChangeHistory::isAtBase()                 const
{
    return (mBaseBeforeChangeIndex == mAppliedChangesCount);
}
inline Size PieceTableChangeHistory::appliedChangesDataSize()    const { return mAppliedChangesDataSize; }

inline QString PieceTableChangeHistory::changeDescription(int changeId) const
{
    const AbstractPieceTableChange* change = mChangeStack.value(changeId);

    return change ? change->description() : QString();
}

}

#endif
