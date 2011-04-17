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
    explicit PieceTable( Size size = 0 );

  public:
    void init( Size size );
    void insert( Address insertDataOffset, Size insertLength, Address storageOffset );
    /// for use to reapply
    void insert( Address insertDataOffset, const PieceList& insertPieceList );
    PieceList remove( const AddressRange& removeRange );
    PieceList replace( const AddressRange& removeRange,
                       Size insertLength, Address storageOffset );
    /// for use to reapply
    void replace( const AddressRange& removeRange, const PieceList& insertPieceList );
    void swap( Address firstStart, const AddressRange& secondRange );
    Piece replaceOne( Address dataOffset, Address storageOffset, int storageId = Piece::ChangeStorage );

//     int fill( const char FillChar, unsigned int Pos = 0, int Length = -1 ); TODO: filter change, calculated

  public:
    bool getStorageData( int* storageId, Address* storageOffset, Address dataOffset ) const;
    Size size() const;

  protected:
    QLinkedList<Piece> mList;
    Size mSize;
};

inline Size PieceTable::size() const { return mSize; }

}

#endif
