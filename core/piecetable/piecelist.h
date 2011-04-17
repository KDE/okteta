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

#ifndef KPIECETABLE_PIECELIST_H
#define KPIECETABLE_PIECELIST_H

// lib
#include "piece.h"
// Qt
#include <QtCore/QList>


namespace KPieceTable
{

class PieceList
{
  public:
    PieceList();
    explicit PieceList( const Piece& piece );

    ~PieceList();

  public:
    int size() const;
    bool isEmpty() const;
    Size totalLength() const;
    const Piece& at( int i ) const;

  public:
    void append( const PieceList& other );
    void append( const Piece& piece );
    void prepend( const PieceList& other );

  protected:
    QList<Piece> mList;
    Size mTotalLength;
};


inline PieceList::PieceList() : mTotalLength( 0 ) {}
inline PieceList::PieceList( const Piece& piece )
 : mTotalLength( 0 )
{
    append( piece );
}

inline int PieceList::size()               const { return mList.size(); }
inline bool PieceList::isEmpty()           const { return mList.isEmpty(); }
inline Size PieceList::totalLength()       const { return mTotalLength; }
inline const Piece& PieceList::at( int i ) const { return mList.at( i ); }

inline void PieceList::append( const Piece& piece )
{
    bool isMerged = false;
    if( !mList.isEmpty() )
        isMerged = mList.last().append( piece );
    if( !isMerged )
        mList.append( piece );
    mTotalLength += piece.width();
}
inline void PieceList::append( const PieceList& other )
{
    QList<Piece>::ConstIterator it = other.mList.begin();

    // see if the ones at the border can be merged
    bool isMerged = false;
    if( !mList.isEmpty() && !other.mList.isEmpty() )
        isMerged = mList.last().append( other.mList.first() );
    if( isMerged )
        ++it;

    for( ; it != other.mList.end(); ++it )
        mList.append( *it );
//was:     mList += other.mList;

    mTotalLength += other.mTotalLength;
}
inline void PieceList::prepend( const PieceList& other )
{
    QList<Piece> otherCopy = other.mList;
    QList<Piece>::Iterator it = mList.begin();

    // see if the ones at the border can be merged
    bool isMerged = false;
    if( !otherCopy.isEmpty() && !mList.isEmpty() )
        isMerged = otherCopy.last().append( mList.first() );
    if( isMerged )
        ++it;

    for( ; it != mList.end(); ++it )
        otherCopy.append( *it );
    mList = otherCopy;
//was:     mList = other.mList + mList;

    mTotalLength += other.mTotalLength;
}

inline PieceList::~PieceList() {}

}

#endif
