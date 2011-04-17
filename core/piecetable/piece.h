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

#ifndef KPIECETABLE_PIECE_H
#define KPIECETABLE_PIECE_H


// lib
#include <addressrange.h>

namespace KPieceTable
{

typedef Okteta::Size Size;
typedef Okteta::Address Address;
typedef Okteta::AddressRange AddressRange;


class Piece : public AddressRange
{
  public:
    enum {
        OriginalStorage,
        ChangeStorage
    };

  public:
    Piece( Address storageOffset, Size size, int storageId );
    Piece( const AddressRange& storageRange, int storageId );
    Piece();

  public:
    int storageId() const;

  public:
    void setStorageId( int storageId );
    Piece splitAt( Address storageOffset );
    Piece splitAtLocal( Address localStorageOffset );
    Piece remove( const AddressRange& removeStorageRange );
    Piece removeLocal( const AddressRange& localRemoveStorageRange );
    Piece removeStartBeforeLocal( Address storageOffset );
    Piece removeEndBehindLocal( Address storageOffset );
    bool prepend( const Piece& other );
    bool append( const Piece& other );

  public:
    Piece subPiece( const AddressRange& local ) const;

  protected:
    int mStorageId;
};


inline Piece::Piece( Address storageOffset, Size size, int storageId )
  : AddressRange( AddressRange::fromWidth(storageOffset,size) ),
    mStorageId( storageId )
{}
inline Piece::Piece( const AddressRange& storageRange, int storageId )
  : AddressRange( storageRange ),
    mStorageId( storageId )
{}
inline Piece::Piece() : mStorageId(OriginalStorage) {}

inline int Piece::storageId() const { return mStorageId; }

inline void Piece::setStorageId( int storageId ) { mStorageId = storageId; }

inline Piece Piece::splitAt( Address storageOffset )
{
    return Piece( AddressRange::splitAt(storageOffset), mStorageId );
}
inline Piece Piece::splitAtLocal( Address localStorageOffset )
{
    return Piece( AddressRange::splitAtLocal(localStorageOffset), mStorageId );
}
inline Piece Piece::remove( const AddressRange& removeStorageRange )
{
    return Piece( AddressRange::remove(removeStorageRange), mStorageId );
}
inline Piece Piece::removeLocal( const AddressRange& localRemoveStorageRange )
{
    return Piece( AddressRange::removeLocal(localRemoveStorageRange), mStorageId );
}
inline Piece Piece::removeStartBeforeLocal( Address storageOffset )
{
    const Address oldStart = start();
    moveStartBy( storageOffset );
    return Piece( AddressRange(oldStart,nextBeforeStart()), mStorageId );
}
inline Piece Piece::removeEndBehindLocal( Address storageOffset )
{
    const Address oldEnd = end();
    setEndByWidth( storageOffset+1 );
    return Piece( AddressRange(nextBehindEnd(),oldEnd), mStorageId );
}

inline Piece Piece::subPiece( const AddressRange& local ) const
{
    return Piece( AddressRange::subRange(local), mStorageId );
}

inline bool Piece::prepend( const Piece& other )
{
    const bool result = ( mStorageId == other.mStorageId && AddressRange::prepend(other) );
    return result;
}
inline bool Piece::append( const Piece& other )
{
    const bool result = ( mStorageId == other.mStorageId && AddressRange::append(other) );
    return result;
}

}

#endif
