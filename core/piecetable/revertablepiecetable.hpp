/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KPIECETABLE_REVERTABLEPIECETABLE_HPP
#define KPIECETABLE_REVERTABLEPIECETABLE_HPP

// lib
#include "piecetablechangehistory.hpp"
#include "piecetable.hpp"

namespace KPieceTable {

class RevertablePieceTable
{
public:
    RevertablePieceTable();
    RevertablePieceTable(const RevertablePieceTable&) = delete;
    RevertablePieceTable(RevertablePieceTable&&) = delete;

    ~RevertablePieceTable();

    RevertablePieceTable& operator=(const RevertablePieceTable&) = delete;
    RevertablePieceTable& operator=(RevertablePieceTable&&) = delete;

public:
    void init(Size size);

public: // operations return true if it results in a new change and is not merged to the current
    bool insert(Address dataOffset, Size length, Address* storageOffset);
    bool remove(const AddressRange& removeRange);
//     bool remove( Address start, Address end );
    bool replace(const AddressRange& removeRange, Size insertLength, Size* storageSize);
    bool replace(Address removeStart, Size removeLength, Size insertLength, Size* storageSize);
    bool replaceOne(Address dataOffset, Size* storageSize);
    bool swap(Address firstStart, const AddressRange& secondRange);
    bool swap(Address firstStart, Address secondStart, Size secondLength);
//     int fill( const char FillChar, unsigned int Pos = 0, int Length = -1 ); TODO: filter change, calculated

public:
    /**
     * opens a group of changes
     * @param description sets the description of the group
     */
    void openGroupedChange(const QString& description);   // TODO: hand over description? user change id?
    /**
     * closes the current group and sets the parent group as current if there is one
     * @param description sets a new description for the group if not empty
     */
    void closeGroupedChange(const QString& description);

public:
    /**
     * closes the current change, so any following operation will not be tried to merge
     */
    void finishChange();
    /**
     * closes the current change, so any following operation will not be tried to merge
     * currently it also closes any opened groups
     * @param changeId
     * @param changedRanges
     * @param changeList
     */
    bool revertBeforeChange(int changeId,
                            AddressRangeList* changedRanges, ArrayChangeMetricsList* changeList);
    // TODO: hide should be a flag with or just an own function unsetBase();
    /**
     * @param hide  if true sets the base to none.
     */
    void setBeforeCurrentChangeAsBase(bool hide);

public:
    bool getStorageData(int* storageId, Address* storageOffset, Address dataOffset) const;
    [[nodiscard]]
    Size size() const;
    void getChangeData(ArrayChangeMetrics* metrics, Address* storageOffset, int versionIndex) const;

public:
    [[nodiscard]]
    int changesCount() const;
    [[nodiscard]]
    int appliedChangesCount() const;
    [[nodiscard]]
    QString changeDescription(int change) const;
    [[nodiscard]]
    QString headChangeDescription() const;
    [[nodiscard]]
    bool isAtBase() const;

private:
    PieceTable mPieceTable;
    PieceTableChangeHistory mChangeHistory;
};

inline RevertablePieceTable::~RevertablePieceTable() = default;

inline bool RevertablePieceTable::getStorageData(int* storageId, Address* storageOffset, Address dataOffset) const
{
    return mPieceTable.getStorageData(storageId, storageOffset, dataOffset);
}

inline void RevertablePieceTable::setBeforeCurrentChangeAsBase(bool hide)
{
    mChangeHistory.setBeforeCurrentChangeAsBase(hide);
}

inline Size RevertablePieceTable::size()                const { return mPieceTable.size(); }
inline int RevertablePieceTable::changesCount()        const { return mChangeHistory.count(); }
inline int RevertablePieceTable::appliedChangesCount() const { return mChangeHistory.appliedChangesCount(); }
inline bool RevertablePieceTable::isAtBase()           const { return mChangeHistory.isAtBase(); }
inline QString RevertablePieceTable::changeDescription(int change) const
{
    return mChangeHistory.changeDescription(change);
}
inline QString RevertablePieceTable::headChangeDescription() const
{
    return mChangeHistory.headChangeDescription();
}

// inline bool RevertablePieceTable::remove( Address start, Size length )
// {
//     return remove( AddressRange::fromWidth(start,length) );
// }
inline bool RevertablePieceTable::replace(Address removeStart, Size removeLength, Size insertLength, Size* storageSize)
{
    return replace(AddressRange::fromWidth(removeStart, removeLength), insertLength, storageSize);
}
inline bool RevertablePieceTable::swap(Address firstStart, Address secondStart, Size secondLength)
{
    return swap(firstStart, AddressRange::fromWidth(secondStart, secondLength));
}

inline void RevertablePieceTable::openGroupedChange(const QString& description)
{
    mChangeHistory.openGroupedChange(description);
}

inline void RevertablePieceTable::closeGroupedChange(const QString& description)
{
    mChangeHistory.closeGroupedChange(description);
}
inline void RevertablePieceTable::finishChange()       { mChangeHistory.finishChange(); }

inline bool RevertablePieceTable::revertBeforeChange(int changeId,
                                                     AddressRangeList* changedRanges,
                                                     ArrayChangeMetricsList* changeList)
{
    return mChangeHistory.revertBeforeChange(&mPieceTable, changeId, changedRanges, changeList);
}

}

#endif
