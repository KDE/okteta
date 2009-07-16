/*
    This file is part of the Okteta Core library, part of the KDE project.

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

#ifndef BOOKMARKLISTCONSTITERATORADAPTER_H
#define BOOKMARKLISTCONSTITERATORADAPTER_H

// lib
#include "bookmarksconstiteratoradapter.h"
// Qt
#include <QtCore/QLinkedListIterator>


namespace Okteta
{
class BookmarkList;


class BookmarkListConstIteratorAdapter : public BookmarksConstIteratorAdapter
{
  public:
    explicit BookmarkListConstIteratorAdapter( const BookmarkList& list );
    virtual ~BookmarkListConstIteratorAdapter();

  public: // BookmarksConstIteratorAdapter API
    virtual bool hasNext() const;
    virtual bool hasPrevious() const;
    virtual const Okteta::Bookmark& peekNext() const;
    virtual const Okteta::Bookmark& peekPrevious() const;

    virtual bool findNext( const Okteta::Bookmark& bookmark );
    virtual bool findPrevious( const Okteta::Bookmark& bookmark );
    virtual bool findNextFrom( unsigned int offset );
    virtual bool findPreviousFrom( unsigned int offset );
    virtual const Okteta::Bookmark& next();
    virtual const Okteta::Bookmark& previous();
    virtual void toBack();
    virtual void toFront();

  protected:
    QLinkedListIterator<Okteta::Bookmark> mIterator;
};

}

#endif
