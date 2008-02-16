/***************************************************************************
                          piecelist.h  -  description
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
    explicit PieceList( const Piece &piece );
    ~PieceList();

  public:
    int size() const;
    int totalLength() const;
    const Piece &at( int i ) const;

  public:
    void append( const PieceList &other );
    void append( const Piece &piece );
    void prepend( const PieceList &other );

  protected:
    QList<Piece> mList;
    int mTotalLength;
};

inline PieceList::PieceList() : mTotalLength( 0 ) {}
inline PieceList::PieceList( const Piece &piece )
 : mTotalLength( 0 )
{
    append( piece );
}

inline int PieceList::size()               const { return mList.size(); }
inline int PieceList::totalLength()        const { return mTotalLength; }
inline const Piece &PieceList::at( int i ) const { return mList.at( i ); }

inline void PieceList::append( const Piece &piece )
{
    mList.append( piece );
    mTotalLength += piece.width();
}
inline void PieceList::append( const PieceList &other )
{
//     QList<Piece>::operator+=( other );// TODO: middle pieces are not joined
    mList += other.mList;
    mTotalLength += other.mTotalLength;
}
inline void PieceList::prepend( const PieceList &other )
{
// TODO: middle pieces are not joined
//     QList<Piece>::operator=( other.QList<Piece>::operator+( *this ) );
    mList = other.mList + mList;
    mTotalLength += other.mTotalLength;
}

inline PieceList::~PieceList() {}

}

#endif
