/*
    This file is part of the Okteta Core library, made within the KDE community.

    Copyright 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef OKTETA_BOOKMARKSCONSTITERATOR_H
#define OKTETA_BOOKMARKSCONSTITERATOR_H

// lib
#include "bookmarksconstiteratoradapter.h"
#include "oktetacore_export.h"
// KDE
#include <KSharedPtr>


namespace Okteta
{

class OKTETACORE_EXPORT BookmarksConstIterator
{
  public:
    explicit BookmarksConstIterator( BookmarksConstIteratorAdapter* adapter = 0 );

  public:
    bool hasList() const;

  public:
    bool hasNext() const;
    bool hasPrevious() const;
    const Okteta::Bookmark& peekNext() const;
    const Okteta::Bookmark& peekPrevious() const;

  public:
    bool findNext( const Okteta::Bookmark& bookmark );
    bool findPrevious( const Okteta::Bookmark& bookmark );
    bool findNextFrom( unsigned int offset );
    bool findPreviousFrom( unsigned int offset );
    const Okteta::Bookmark& next();
    const Okteta::Bookmark& previous();
    void toBack();
    void toFront();

  protected:
    KSharedPtr<BookmarksConstIteratorAdapter> mAdapter;
};


inline BookmarksConstIterator::BookmarksConstIterator( BookmarksConstIteratorAdapter* adapter ) : mAdapter( adapter ) {}

inline bool BookmarksConstIterator::hasList() const { return mAdapter; }

inline bool BookmarksConstIterator::hasNext()     const { return mAdapter->hasNext(); }
inline bool BookmarksConstIterator::hasPrevious() const { return mAdapter->hasPrevious(); }
inline const Okteta::Bookmark& BookmarksConstIterator::peekNext()     const { return mAdapter->peekNext(); }
inline const Okteta::Bookmark& BookmarksConstIterator::peekPrevious() const { return mAdapter->peekPrevious(); }

inline bool BookmarksConstIterator::findNext( const Okteta::Bookmark& bookmark )     { return mAdapter->findNext( bookmark ); }
inline bool BookmarksConstIterator::findPrevious( const Okteta::Bookmark& bookmark ) { return mAdapter->findPrevious( bookmark ); }
inline bool BookmarksConstIterator::findNextFrom( unsigned int offset )     { return mAdapter->findNextFrom( offset ); }
inline bool BookmarksConstIterator::findPreviousFrom( unsigned int offset ) { return mAdapter->findPreviousFrom( offset ); }
inline const Okteta::Bookmark& BookmarksConstIterator::next()     { return mAdapter->next(); }
inline const Okteta::Bookmark& BookmarksConstIterator::previous() { return mAdapter->previous(); }
inline void BookmarksConstIterator::toBack()  { mAdapter->toBack(); }
inline void BookmarksConstIterator::toFront() { mAdapter->toFront(); }

}

#endif
