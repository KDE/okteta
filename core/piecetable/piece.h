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

#ifndef KPIECETABLE_PIECE_H
#define KPIECETABLE_PIECE_H


// lib
#include <section.h>

namespace KPieceTable
{

class Piece : public KHE::Section
{
  public:
    enum {
        OriginalStorage,
        ChangeStorage
    };

  public:
    Piece( int storageOffset, int size, int storageId );
    Piece( const KHE::Section &storageSection, int storageId );
    Piece();

  public:
    int storageId() const;

  public:
    void setStorageId( int storageId );
    Piece splitAt( int storageOffset );
    Piece splitAtLocal( int localStorageOffset );
    Piece remove( const KHE::Section &removeStorageSection );
    Piece removeLocal( const KHE::Section &localRemoveStorageSection );
    Piece removeStartBeforeLocal( int storageOffset );
    Piece removeEndBehindLocal( int storageOffset );
    bool prepend( const Piece &other );
    bool append( const Piece &other );


  public:
    Piece subPiece( const Section &local ) const;

  protected:
    int mStorageId;
};

inline Piece::Piece( int storageOffset, int size, int storageId )
: KHE::Section( KHE::Section::fromWidth(storageOffset,size) ), mStorageId( storageId ) {}
inline Piece::Piece( const KHE::Section &storageSection, int storageId )
: KHE::Section( storageSection ), mStorageId( storageId ) {}
inline Piece::Piece() : mStorageId(OriginalStorage) {}
inline int Piece::storageId() const { return mStorageId; }

inline void Piece::setStorageId( int storageId ) { mStorageId = storageId; }

inline Piece Piece::splitAt( int storageOffset )
{
    return Piece( KHE::Section::splitAt(storageOffset), mStorageId );
}
inline Piece Piece::splitAtLocal( int localStorageOffset )
{
    return Piece( KHE::Section::splitAtLocal(localStorageOffset), mStorageId );
}
inline Piece Piece::remove( const KHE::Section &removeStorageSection )
{
    return Piece( KHE::Section::remove(removeStorageSection), mStorageId );
}
inline Piece Piece::removeLocal( const KHE::Section &localRemoveStorageSection )
{
    return Piece( KHE::Section::removeLocal(localRemoveStorageSection), mStorageId );
}
inline Piece Piece::removeStartBeforeLocal( int storageOffset )
{
    const int oldStart = start();
    moveStartBy( storageOffset );
    return Piece( KHE::Section(oldStart,nextBeforeStart()), mStorageId );
}
inline Piece Piece::removeEndBehindLocal( int storageOffset )
{
    const int oldEnd = end();
    setEndByWidth( storageOffset+1 );
    return Piece( KHE::Section(nextBehindEnd(),oldEnd), mStorageId );
}

inline Piece Piece::subPiece( const Section &local ) const
{
    return Piece( KHE::Section::subSection(local), mStorageId );
}

inline bool Piece::prepend( const Piece &other )
{
    const bool result = ( mStorageId == other.mStorageId && KHE::Section::prepend(other) );
    return result;
}
inline bool Piece::append( const Piece &other )
{
    const bool result = ( mStorageId == other.mStorageId && KHE::Section::append(other) );
    return result;
}

}

#endif
