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

#ifndef KPIECETABLE_PIECETABLECHANGEHISTORY_H
#define KPIECETABLE_PIECETABLECHANGEHISTORY_H


// lib
#include "abstractpiecetablechange.h"
// Qt
#include <QtCore/QStack>
#include <QtCore/QString>

namespace KHE {
class KSectionList;
class ArrayChangeMetricsList;
}

namespace KPieceTable
{

class PieceTableChangeHistory
{
  public:
    PieceTableChangeHistory();
    ~PieceTableChangeHistory();

  public:
    void clear();
    /// returns true, if a new change is appended, false if merged
    bool appendChange( AbstractPieceTableChange *change );

    bool revertBeforeChange( PieceTable *pieceTable, int changeId,
                             KHE::KSectionList *changedRanges, KHE::ArrayChangeMetricsList *changeList );

    // 
    void openGroupedChange( const QString &description ); // TODO: hand over description? user change id?
    void closeGroupedChange( const QString &description );
    // 
    void finishChange();

    void setBeforeCurrentChangeAsBase( bool hide );

  public:
    int count() const;
    int appliedChangesCount() const;
    QString changeDescription( int changeId ) const;
    QString headChangeDescription() const;
    bool isAtBase() const;
    int appliedChangesDataSize() const;

  protected:
    /// if 0, no change group is open
    int mChangeGroupOpened;

    /// if true, try to merge changes
    bool mMergeChanges;
    ///
    int mAppliedChangesCount;
    ///
    int mBaseBeforeChangeIndex;
    ///
    QStack<AbstractPieceTableChange*> mChangeStack;
    ///
    int mAppliedChangesDataSize;
};


inline PieceTableChangeHistory::PieceTableChangeHistory()
 : mChangeGroupOpened( 0 ), mMergeChanges( false ),
   mAppliedChangesCount( 0 ), mBaseBeforeChangeIndex( 0 ), mAppliedChangesDataSize( 0 )
{}

inline int PieceTableChangeHistory::count()                     const { return mChangeStack.count(); }
inline int PieceTableChangeHistory::appliedChangesCount()       const { return mAppliedChangesCount; }
inline QString PieceTableChangeHistory::headChangeDescription() const { return changeDescription( count()-1 ); }
inline bool PieceTableChangeHistory::isAtBase()                 const
{
    return ( mBaseBeforeChangeIndex == mAppliedChangesCount );
}
inline int PieceTableChangeHistory::appliedChangesDataSize()    const { return mAppliedChangesDataSize; }

inline void PieceTableChangeHistory::finishChange()       { mMergeChanges = false; }

inline QString PieceTableChangeHistory::changeDescription( int changeId ) const
{
    const AbstractPieceTableChange *change = mChangeStack.value( changeId );

    return change ? change->description() : QString();
}

inline PieceTableChangeHistory::~PieceTableChangeHistory() { clear(); }

}

#endif
