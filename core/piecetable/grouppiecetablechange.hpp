/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KPIECETABLE_GROUPPIECETABLECHANGE_HPP
#define KPIECETABLE_GROUPPIECETABLECHANGE_HPP

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

/** class
 * @author Friedrich W. H. Kossebau
 */
class GroupPieceTableChange : public AbstractPieceTableChange
{
public:
    GroupPieceTableChange(GroupPieceTableChange* parent, const QString& description);
    GroupPieceTableChange() = delete;

    ~GroupPieceTableChange() override;

public: // AbstractPieceTableChange API
    int type() const override;
    QString description() const override;
    bool merge(const AbstractPieceTableChange* other) override;
    AddressRange apply(PieceTable* pieceTable) const override;
    AddressRange revert(PieceTable* pieceTable) const override;
    ArrayChangeMetrics metrics() const override;
    Size dataSize() const override;

public:
    void setDescription(const QString& description);

// TODO: think about a function to compress a group, that is not going to be handled in detail anymore
// e.g. several replaces of the same byte or an insert and replace of the inserted data

public:
    AddressRangeList applyGroup(PieceTable* pieceTable) const;
    AddressRangeList revertGroup(PieceTable* pieceTable) const;
    ArrayChangeMetricsList groupMetrics(bool reverted = false) const;
    GroupPieceTableChange* parent() const;

public: // TODO: this interface part is shared with PieceTableChangeHistory, try to use this fact
    bool appendChange(AbstractPieceTableChange* change);
    void finishChange();

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
//     bool isAtBase() const;
    /// @return size of the data used by the applied changes
    int appliedChangesDataSize() const;

private:
    QStack<AbstractPieceTableChange*> mChangeStack;
    GroupPieceTableChange* mParent;

    QString mDescription;
    ///
    int mAppliedChangesCount = 0;
    ///
    Size mAppliedChangesDataSize = 0;
    /// if true, try to merge changes
    bool mTryToMergeAppendedChange = true;
};

inline GroupPieceTableChange::GroupPieceTableChange(GroupPieceTableChange* parent, const QString& description)
    : mParent(parent)
    , mDescription(description)
{}

inline void GroupPieceTableChange::setDescription(const QString& description) { mDescription = description; }
inline GroupPieceTableChange* GroupPieceTableChange::parent() const { return mParent; }
inline void GroupPieceTableChange::finishChange() { mTryToMergeAppendedChange = false; }
inline int GroupPieceTableChange::count()                     const { return mChangeStack.count(); }
inline int GroupPieceTableChange::appliedChangesCount()       const { return mAppliedChangesCount; }
inline QString GroupPieceTableChange::headChangeDescription() const { return changeDescription(count() - 1); }
inline QString GroupPieceTableChange::changeDescription(int changeId) const
{
    const AbstractPieceTableChange* change = mChangeStack.value(changeId);

    return change ? change->description() : QString();
}

}

#endif
