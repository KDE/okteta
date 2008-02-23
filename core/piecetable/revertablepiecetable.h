/***************************************************************************
                          revertablepiecetable.h  -  description
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

    bool insert( int pos, int length, int storageOffset );
    bool remove( const KHE::KSection &removeSection );
    bool remove( int start, int end );
    bool replace( const KHE::KSection &removeSection, int insertLength, int storageOffset );
    bool replace( int removeStart, int removeLength, int insertLength, int storageOffset );
    bool swap( int firstStart, const KHE::KSection &secondSection );
    bool swap( int firstStart, int secondStart, int secondLength );
    bool replaceOne( int dataOffset, int storageOffset );
//     int fill( const char FillChar, unsigned int Pos = 0, int Length = -1 ); TODO: filter change, calculated

  public:
    // 
    void openGroupedChange(); // TODO: hand over description? user change id?
    void closeGroupedChange();
    // 
    void finishChange();
    bool revertBeforeChange( int changeId,
                             KHE::KSectionList *changedRanges, QList<KHE::ReplacementScope> *replacementList );

    void setBeforeCurrentChangeAsBase( bool hide );

  public:
    bool getStorageData( int *storageId, int *storageOffset, int dataOffset ) const;
    int size() const;

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
{ return mPieceTable.getStorageData( storageId, storageOffset, dataOffset ); }
inline void RevertablePieceTable::setBeforeCurrentChangeAsBase( bool hide )
{ mChangeHistory.setBeforeCurrentChangeAsBase( hide ); }

inline int RevertablePieceTable::size()                const { return mPieceTable.size(); }
inline int RevertablePieceTable::changesCount()        const { return mChangeHistory.count(); }
inline int RevertablePieceTable::appliedChangesCount() const { return mChangeHistory.appliedChangesCount(); }

inline void RevertablePieceTable::openGroupedChange() { mChangeHistory.openGroupedChange(); }
inline void RevertablePieceTable::closeGroupedChange() { mChangeHistory.closeGroupedChange(); }
inline void RevertablePieceTable::finishChange() { mChangeHistory.finishChange(); }

inline QString RevertablePieceTable::changeDescription( int change ) const { return mChangeHistory.changeDescription( change ); }
inline QString RevertablePieceTable::headChangeDescription()         const { return mChangeHistory.headChangeDescription(); }
inline bool RevertablePieceTable::isAtBase() const { return mChangeHistory.isAtBase(); }

}

#endif
