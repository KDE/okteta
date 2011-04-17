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

#ifndef KPIECETABLE_REVERTABLEPIECETABLE_H
#define KPIECETABLE_REVERTABLEPIECETABLE_H

// lib
#include "piecetablechangehistory.h"
#include "piecetable.h"


namespace KPieceTable
{

class RevertablePieceTable
{
  public:
    RevertablePieceTable();

  public:
    void init( Size size );

  public: // operations return true if it results in a new change and is not merged to the current
    bool insert( Address pos, Size length, Size* storageSize );
    bool remove( const AddressRange& removeRange );
//     bool remove( Address start, Address end );
    bool replace( const AddressRange& removeRange, Size insertLength, Size* storageSize );
    bool replace( Address removeStart, Size removeLength, Size insertLength, Size* storageSize );
    bool replaceOne( Address dataOffset, Size* storageSize );
    bool swap( Address firstStart, const AddressRange& secondRange );
    bool swap( Address firstStart, Address secondStart, Size secondLength );
//     int fill( const char FillChar, unsigned int Pos = 0, int Length = -1 ); TODO: filter change, calculated

  public:
    /**
     * opens a group of changes
     * @param description sets the description of the group
     */
    void openGroupedChange( const QString& description ); // TODO: hand over description? user change id?
    /**
     * closes the current group and sets the parent group as current if there is one
     * @param description sets a new description for the group if not empty
     */
    void closeGroupedChange( const QString& description );

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
    bool revertBeforeChange( int changeId,
                             AddressRangeList* changedRanges, ArrayChangeMetricsList* changeList );
    //TODO: hide should be a flag with or just an own function unsetBase();
    /**
     * @param hide  if true sets the base to none.
     */
    void setBeforeCurrentChangeAsBase( bool hide );

  public:
    bool getStorageData( int* storageId, Address* storageOffset, Address dataOffset ) const;
    Size size() const;
    void getChangeData( ArrayChangeMetrics* metrics, Address* storageOffset, int versionIndex ) const;

  public:
    int changesCount() const;
    int appliedChangesCount() const;
    QString changeDescription( int change ) const;
    QString headChangeDescription() const;
    bool isAtBase() const;

  protected:
    PieceTable mPieceTable;
    PieceTableChangeHistory mChangeHistory;
};


inline bool RevertablePieceTable::getStorageData( int* storageId, Address* storageOffset, Address dataOffset ) const
{
    return mPieceTable.getStorageData( storageId, storageOffset, dataOffset );
}

inline void RevertablePieceTable::setBeforeCurrentChangeAsBase( bool hide )
{
    mChangeHistory.setBeforeCurrentChangeAsBase( hide );
}

inline Size RevertablePieceTable::size()                const { return mPieceTable.size(); }
inline int RevertablePieceTable::changesCount()        const { return mChangeHistory.count(); }
inline int RevertablePieceTable::appliedChangesCount() const { return mChangeHistory.appliedChangesCount(); }
inline bool RevertablePieceTable::isAtBase()           const { return mChangeHistory.isAtBase(); }
inline QString RevertablePieceTable::changeDescription( int change ) const
{
    return mChangeHistory.changeDescription( change );
}
inline QString RevertablePieceTable::headChangeDescription() const
{
    return mChangeHistory.headChangeDescription();
}

// inline bool RevertablePieceTable::remove( Address start, Size length )
// {
//     return remove( AddressRange::fromWidth(start,length) );
// }
inline bool RevertablePieceTable::replace( Address removeStart, Size removeLength, Size insertLength, Size* storageSize )
{
    return replace( AddressRange::fromWidth(removeStart,removeLength), insertLength, storageSize );
}
inline bool RevertablePieceTable::swap( Address firstStart, Address secondStart, Size secondLength )
{
    return swap( firstStart, AddressRange::fromWidth(secondStart,secondLength) );
}

inline void RevertablePieceTable::openGroupedChange( const QString& description )
{
    mChangeHistory.openGroupedChange(description);
}

inline void RevertablePieceTable::closeGroupedChange( const QString& description )
{
    mChangeHistory.closeGroupedChange(description);
}
inline void RevertablePieceTable::finishChange()       { mChangeHistory.finishChange(); }

inline bool RevertablePieceTable::revertBeforeChange( int changeId,
                                                      AddressRangeList* changedRanges,
                                                      ArrayChangeMetricsList* changeList )
{
    return mChangeHistory.revertBeforeChange( &mPieceTable, changeId, changedRanges, changeList );
}


}

#endif
