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

class Piece : public KDE::Section
{
  public:
    enum {
        OriginalStorage,
        ChangeStorage
    };

  public:
    Piece( int storageOffset, int size, int storageId );
    Piece( const KDE::Section &storageSection, int storageId );
    Piece();

  public:
    int storageId() const;

  public:
    void setStorageId( int storageId );
    Piece splitAt( int storageOffset );
    Piece splitAtLocal( int localStorageOffset );
    Piece remove( const KDE::Section &removeStorageSection );
    Piece removeLocal( const KDE::Section &localRemoveStorageSection );
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
: KDE::Section( KDE::Section::fromWidth(storageOffset,size) ), mStorageId( storageId ) {}
inline Piece::Piece( const KDE::Section &storageSection, int storageId )
: KDE::Section( storageSection ), mStorageId( storageId ) {}
inline Piece::Piece() : mStorageId(OriginalStorage) {}
inline int Piece::storageId() const { return mStorageId; }

inline void Piece::setStorageId( int storageId ) { mStorageId = storageId; }

inline Piece Piece::splitAt( int storageOffset )
{
    return Piece( KDE::Section::splitAt(storageOffset), mStorageId );
}
inline Piece Piece::splitAtLocal( int localStorageOffset )
{
    return Piece( KDE::Section::splitAtLocal(localStorageOffset), mStorageId );
}
inline Piece Piece::remove( const KDE::Section &removeStorageSection )
{
    return Piece( KDE::Section::remove(removeStorageSection), mStorageId );
}
inline Piece Piece::removeLocal( const KDE::Section &localRemoveStorageSection )
{
    return Piece( KDE::Section::removeLocal(localRemoveStorageSection), mStorageId );
}
inline Piece Piece::removeStartBeforeLocal( int storageOffset )
{
    const int oldStart = start();
    moveStartBy( storageOffset );
    return Piece( KDE::Section(oldStart,nextBeforeStart()), mStorageId );
}
inline Piece Piece::removeEndBehindLocal( int storageOffset )
{
    const int oldEnd = end();
    setEndByWidth( storageOffset+1 );
    return Piece( KDE::Section(nextBehindEnd(),oldEnd), mStorageId );
}

inline Piece Piece::subPiece( const Section &local ) const
{
    return Piece( KDE::Section::subSection(local), mStorageId );
}

inline bool Piece::prepend( const Piece &other )
{
    const bool result = ( mStorageId == other.mStorageId && KDE::Section::prepend(other) );
    return result;
}
inline bool Piece::append( const Piece &other )
{
    const bool result = ( mStorageId == other.mStorageId && KDE::Section::append(other) );
    return result;
}

}

#endif
