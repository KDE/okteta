/*
    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KPIECETABLE_GROUPPIECETABLECHANGE_HPP
#define KPIECETABLE_GROUPPIECETABLECHANGE_HPP

// lib
#include "abstractpiecetablechange.hpp"
// Qt
#include <QString>
// Std
#include <memory>
#include <vector>

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
    GroupPieceTableChange(const GroupPieceTableChange&) = delete;
    GroupPieceTableChange(GroupPieceTableChange&&) = delete;

    ~GroupPieceTableChange() override;

    GroupPieceTableChange& operator=(const GroupPieceTableChange&) = delete;
    GroupPieceTableChange& operator=(GroupPieceTableChange&&) = delete;

public: // AbstractPieceTableChange API
    [[nodiscard]]
    int type() const override;
    [[nodiscard]]
    QString description() const override;
    [[nodiscard]]
    bool merge(const AbstractPieceTableChange* other) override;
    [[nodiscard]]
    AddressRange apply(PieceTable* pieceTable) const override;
    [[nodiscard]]
    AddressRange revert(PieceTable* pieceTable) const override;
    [[nodiscard]]
    ArrayChangeMetrics metrics() const override;
    [[nodiscard]]
    Size dataSize() const override;

public:
    void setDescription(const QString& description);

// TODO: think about a function to compress a group, that is not going to be handled in detail anymore
// e.g. several replaces of the same byte or an insert and replace of the inserted data

public:
    [[nodiscard]]
    AddressRangeList applyGroup(PieceTable* pieceTable) const;
    [[nodiscard]]
    AddressRangeList revertGroup(PieceTable* pieceTable) const;
    [[nodiscard]]
    ArrayChangeMetricsList groupMetrics(bool reverted = false) const;
    [[nodiscard]]
    GroupPieceTableChange* parent() const;

public: // TODO: this interface part is shared with PieceTableChangeHistory, try to use this fact
    [[nodiscard]]
    bool appendChange(std::unique_ptr<AbstractPieceTableChange>&& change);
    void finishChange();

public:
    /// @return number of changes in the history
    [[nodiscard]]
    int count() const;
    /// @return number of changes currently applied
    [[nodiscard]]
    int appliedChangesCount() const;
    /// @return description of the change with the id changeId
    [[nodiscard]]
    QString changeDescription(int changeId) const;
    /// @return description of the change at the head, empty if there is none
    [[nodiscard]]
    QString headChangeDescription() const;
    /// @return true if the current change is the base
//     bool isAtBase() const;
    /// @return size of the data used by the applied changes
    [[nodiscard]]
    int appliedChangesDataSize() const;

private:
    std::vector<std::unique_ptr<AbstractPieceTableChange>> mChangeStack;
    GroupPieceTableChange* const mParent;

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

inline GroupPieceTableChange::~GroupPieceTableChange() = default;

inline void GroupPieceTableChange::setDescription(const QString& description) { mDescription = description; }
inline GroupPieceTableChange* GroupPieceTableChange::parent() const { return mParent; }
inline void GroupPieceTableChange::finishChange() { mTryToMergeAppendedChange = false; }
inline int GroupPieceTableChange::count()                     const { return mChangeStack.size(); }
inline int GroupPieceTableChange::appliedChangesCount()       const { return mAppliedChangesCount; }
inline QString GroupPieceTableChange::headChangeDescription() const { return changeDescription(count() - 1); }
inline QString GroupPieceTableChange::changeDescription(int changeId) const
{
    if ((changeId < 0) || (static_cast<int>(mChangeStack.size()) <= changeId)) {
        return {};
    }

    return mChangeStack[changeId]->description();
}

}

#endif
