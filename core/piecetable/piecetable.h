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
    void insert( int insertDataOffset, int insertLength, int storageOffset );
    /// for use to reapply
    void insert( int insertDataOffset, const PieceList &insertPieceList );
    PieceList remove( const KHE::KSection &removeSection );
    PieceList replace( const KHE::KSection &removeSection,
                       int insertLength, int storageOffset );
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
