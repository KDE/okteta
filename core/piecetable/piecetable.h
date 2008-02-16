/***************************************************************************
                          piecetable.h  -  description
                             -------------------
    begin                : Fri Jan 18 2008
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


#ifndef KPIECETABLE_PIECETABLE_H
#define KPIECETABLE_PIECETABLE_H


// lib
#include "piecelist.h"
// Qt
#include <QtCore/QLinkedList>

namespace KPieceTable
{

class PieceTable
{
  public:
    explicit PieceTable( int size = 0 );

  public:
    void init( int size );
    void insert( int insertDataOffset, int insertLength, int storageOffset, int storageId = Piece::ChangeStorage );
    /// for use to reapply
    void insert( int insertDataOffset, const PieceList &insertPieceList );
    PieceList remove( const KHE::KSection &removeSection );
    PieceList replace( const KHE::KSection &removeSection,
                       int insertLength, int storageOffset, int storageId = Piece::ChangeStorage );
    /// for use to reapply
    void replace( const KHE::KSection &removeSection, const PieceList &insertPieceList );
    void swap( int firstStart, const KHE::KSection &secondSection );
    Piece replaceOne( int dataOffset, int storageOffset, int storageId = Piece::ChangeStorage );

//     int fill( const char FillChar, unsigned int Pos = 0, int Length = -1 ); TODO: filter change, calculated

  public:
    bool getStorageData( int *storageId, int *storageOffset, int dataOffset ) const;
    int size() const;

  protected:
    QLinkedList<Piece> mList;
    int mSize;
};

inline int PieceTable::size() const { return mSize; }

}

#endif
