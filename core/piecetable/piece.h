/***************************************************************************
                          piece.h  -  description
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


#ifndef KPIECETABLE_PIECE_H
#define KPIECETABLE_PIECE_H


// lib
#include <ksection.h>

namespace KPieceTable
{

class Piece : public KHE::KSection
{
  public:
    enum {
        OriginalStorage,
        ChangeStorage
    };

  public:
    Piece( int storageOffset, int size, int storageId );
    Piece( const KHE::KSection &storageSection, int storageId );
    Piece();

  public:
    int storageId() const;

  public:
    void setStorageId( int storageId );
    Piece splitAt( int storageOffset );
    Piece splitAtLocal( int localStorageOffset );
    Piece remove( const KHE::KSection &removeStorageSection );
    Piece removeLocal( const KHE::KSection &localRemoveStorageSection );

  public:
    Piece subPiece( const KSection &local ) const;

  protected:
    int mStorageId;
};

inline Piece::Piece( int storageOffset, int size, int storageId )
: KHE::KSection( KHE::KSection::fromWidth(storageOffset,size) ), mStorageId( storageId ) {}
inline Piece::Piece( const KHE::KSection &storageSection, int storageId )
: KHE::KSection( storageSection ), mStorageId( storageId ) {}
inline Piece::Piece() : mStorageId(OriginalStorage) {}
inline int Piece::storageId() const { return mStorageId; }

inline void Piece::setStorageId( int storageId ) { mStorageId = storageId; }

inline Piece Piece::splitAt( int storageOffset )
{
    return Piece( KHE::KSection::splitAt(storageOffset), mStorageId );
}
inline Piece Piece::splitAtLocal( int localStorageOffset )
{
    return Piece( KHE::KSection::splitAtLocal(localStorageOffset), mStorageId );
}
inline Piece Piece::remove( const KHE::KSection &removeStorageSection )
{
    return Piece( KHE::KSection::remove(removeStorageSection), mStorageId );
}
inline Piece Piece::removeLocal( const KHE::KSection &localRemoveStorageSection )
{
    return Piece( KHE::KSection::removeLocal(localRemoveStorageSection), mStorageId );
}

inline Piece Piece::subPiece( const KSection &local ) const
{
    return Piece( KHE::KSection::subSection(local), mStorageId );
}

}

#endif
