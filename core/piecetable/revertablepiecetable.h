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
    void init( int size );

  public: // operations return true if it results in a new change and is not merged to the current
    bool insert( int pos, int length, int *storageSize );
    bool remove( const KHE::KSection &removeSection );
    bool remove( int start, int end );
    bool replace( const KHE::KSection &removeSection, int insertLength, int *storageSize );
    bool replace( int removeStart, int removeLength, int insertLength, int *storageSize );
    bool replaceOne( int dataOffset, int *storageSize );
    bool swap( int firstStart, const KHE::KSection &secondSection );
    bool swap( int firstStart, int secondStart, int secondLength );
//     int fill( const char FillChar, unsigned int Pos = 0, int Length = -1 ); TODO: filter change, calculated

  public:
    /**
     * opens a group of changes
     * @param description sets the description of the group
     */
    void openGroupedChange( const QString &description ); // TODO: hand over description? user change id?
    /**
     * closes the current group and sets the parent group as current if there is one
     * @param description sets a new description for the group if not empty
     */
    void closeGroupedChange( const QString &description );

  public:
    /**
     * closes the current change, so any following operation will not be tried to merge
     */
    void finishChange();
    /**
     * closes the current change, so any following operation will not be tried to merge
     * currently it also closes any opened groups
     * @param 
     * @param changedRanges 
     * @param changeList
     */
    bool revertBeforeChange( int changeId,
                             KHE::KSectionList *changedRanges, KHE::ArrayChangeMetricsList *changeList );
    //TODO: hide should be a flag with or just an own function unsetBase();
    /**
     * @param hide  if true sets the base to none.
     */
    void setBeforeCurrentChangeAsBase( bool hide );

  public:
    bool getStorageData( int *storageId, int *storageOffset, int dataOffset ) const;
    int size() const;
    void getChangeData( KHE::ArrayChangeMetrics* metrics, int *storageOffset, int versionIndex ) const;

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


inline bool RevertablePieceTable::getStorageData( int *storageId, int *storageOffset, int dataOffset ) const
{
    return mPieceTable.getStorageData( storageId, storageOffset, dataOffset );
}

inline void RevertablePieceTable::setBeforeCurrentChangeAsBase( bool hide )
{
    mChangeHistory.setBeforeCurrentChangeAsBase( hide );
}

inline int RevertablePieceTable::size()                const { return mPieceTable.size(); }
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

inline bool RevertablePieceTable::remove( int start, int length )
{
    return remove( KHE::KSection::fromWidth(start,length) );
}
inline bool RevertablePieceTable::replace( int removeStart, int removeLength, int insertLength, int *storageSize )
{
    return replace( KHE::KSection::fromWidth(removeStart,removeLength), insertLength, storageSize );
}
inline bool RevertablePieceTable::swap( int firstStart, int secondStart, int secondLength )
{
    return swap( firstStart, KHE::KSection::fromWidth(secondStart,secondLength) );
}

inline void RevertablePieceTable::openGroupedChange( const QString &description )
{
    mChangeHistory.openGroupedChange(description);
}

inline void RevertablePieceTable::closeGroupedChange( const QString &description )
{
    mChangeHistory.closeGroupedChange(description);
}
inline void RevertablePieceTable::finishChange()       { mChangeHistory.finishChange(); }

inline bool RevertablePieceTable::revertBeforeChange( int changeId,
                                                      KHE::KSectionList *changedRanges,
                                                      KHE::ArrayChangeMetricsList *changeList )
{
    return mChangeHistory.revertBeforeChange( &mPieceTable, changeId, changedRanges, changeList );
}


}

#endif
