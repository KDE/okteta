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

#ifndef KPIECETABLE_PIECETABLECHANGEHISTORY_H
#define KPIECETABLE_PIECETABLECHANGEHISTORY_H

// lib
#include "abstractpiecetablechange.h"
// Qt
#include <QtCore/QStack>
#include <QtCore/QString>


namespace Okteta {
class AddressRangeList;
class ArrayChangeMetricsList;
}

namespace KPieceTable
{

typedef Okteta::AddressRangeList AddressRangeList;
typedef Okteta::ArrayChangeMetricsList ArrayChangeMetricsList;
class GroupPieceTableChange;


class PieceTableChangeHistory
{
  public:
    PieceTableChangeHistory();

    ~PieceTableChangeHistory();

  public:
    void clear();
    /// returns true, if a new change is appended, false if merged
    bool appendChange( AbstractPieceTableChange* change );

    /**
     * @param pieceTable
     * @param changeId
     * @param changedRanges
     * @param changeList
     * @return true if there were changes to revert, false otherwise
     */
    bool revertBeforeChange( PieceTable* pieceTable, int changeId,
                             AddressRangeList* changedRanges, ArrayChangeMetricsList* changeList );

    /// 
    void openGroupedChange( const QString& description ); // TODO: hand over description? user change id?
    void closeGroupedChange( const QString& description );
    /// closes the current change, so any following operation will not be tried to merge
    void finishChange();

    void setBeforeCurrentChangeAsBase( bool hide );

  public:
    /// @return number of changes in the history
    int count() const;
    /// @return number of changes currently applied
    int appliedChangesCount() const;
    /// @return description of the change with the id changeId
    QString changeDescription( int changeId ) const;
    /// @return description of the change at the head, empty if there is none
    QString headChangeDescription() const;
    /// @return true if the current change is the base
    bool isAtBase() const;
    /// @return size of the data used by the applied changes
    Size appliedChangesDataSize() const;

    void getChangeData( ArrayChangeMetrics* metrics, Address* storageOffset, int versionIndex ) const;

  protected:
    /// if true, try to merge changes
    bool mTryToMergeAppendedChange;
    ///
    int mAppliedChangesCount;
    ///
    int mBaseBeforeChangeIndex;
    ///
    QStack<AbstractPieceTableChange*> mChangeStack;
    ///
    Size mAppliedChangesDataSize;

    /// if 0, there is no
    GroupPieceTableChange *mActiveGroupChange;
};


inline PieceTableChangeHistory::PieceTableChangeHistory()
  : mTryToMergeAppendedChange( false ),
    mAppliedChangesCount( 0 ),
    mBaseBeforeChangeIndex( 0 ),
    mAppliedChangesDataSize( 0 ),
    mActiveGroupChange( 0 )
{}

inline int PieceTableChangeHistory::count()                     const { return mChangeStack.count(); }
inline int PieceTableChangeHistory::appliedChangesCount()       const { return mAppliedChangesCount; }
inline QString PieceTableChangeHistory::headChangeDescription() const { return changeDescription( count()-1 ); }
inline bool PieceTableChangeHistory::isAtBase()                 const
{
    return ( mBaseBeforeChangeIndex == mAppliedChangesCount );
}
inline Size PieceTableChangeHistory::appliedChangesDataSize()    const { return mAppliedChangesDataSize; }

inline QString PieceTableChangeHistory::changeDescription( int changeId ) const
{
    const AbstractPieceTableChange* change = mChangeStack.value( changeId );

    return change ? change->description() : QString();
}

inline PieceTableChangeHistory::~PieceTableChangeHistory() { clear(); }

}

#endif
