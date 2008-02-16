/***************************************************************************
                          piecetablechangehistory.h  -  description
                             -------------------
    begin                : Mon Feb 11 2008
    copyright            : 2008 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License version 2 as published by the Free Software Foundation.       *
 *                                                                         *
 ***************************************************************************/


#ifndef KPIECETABLE_PIECETABLECHANGEHISTORY_H
#define KPIECETABLE_PIECETABLECHANGEHISTORY_H


// lib
#include "abstractpiecetablechange.h"
// Qt
#include <QtCore/QStack>
#include <QtCore/QString>
#include <QtCore/QList>

class QString;

namespace KHE {
class KSectionList;
class ReplacementScope;
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
                             KHE::KSectionList *changedRanges, QList<KHE::ReplacementScope> *replacementList );

    // 
    void openGroupedChange(); // TODO: hand over description? user change id?
    void closeGroupedChange();
    // 
    void finishChange();

  public:
    int count() const;
    int appliedChangesCount() const;
    QString changeDescription( int changeId ) const;
    QString headChangeDescription() const;

  protected:
    /// if 0, no change group is open
    int mChangeGroupOpened;

    /// if true, try to merge changes
    bool mMergeChanges;
    ///
    int mAppliedChangesCount;
    ///
    QStack<AbstractPieceTableChange*> mChangeStack;
};


inline PieceTableChangeHistory::PieceTableChangeHistory()
 : mChangeGroupOpened( 0 ), mMergeChanges( false ), mAppliedChangesCount( 0 ) {}
inline int PieceTableChangeHistory::count()                     const { return mChangeStack.count(); }
inline int PieceTableChangeHistory::appliedChangesCount()       const { return mAppliedChangesCount; }
inline QString PieceTableChangeHistory::headChangeDescription() const { return changeDescription( count()-1 ); }

inline void PieceTableChangeHistory::openGroupedChange()  { ++mChangeGroupOpened; }
inline void PieceTableChangeHistory::closeGroupedChange() { if( mChangeGroupOpened > 0 ) --mChangeGroupOpened; }
inline void PieceTableChangeHistory::finishChange()       { mMergeChanges = false; }

inline QString PieceTableChangeHistory::changeDescription( int changeId ) const
{
    const AbstractPieceTableChange *change = mChangeStack.value( changeId );

    return change ? change->description() : QString();
}

inline PieceTableChangeHistory::~PieceTableChangeHistory() { clear(); }

}

#endif
