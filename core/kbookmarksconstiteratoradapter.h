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

#ifndef KHE_CORE_BOOKMARKSCONSTITERATORADAPTER_H
#define KHE_CORE_BOOKMARKSCONSTITERATORADAPTER_H

// Qt
#include <QtCore/QSharedData>


namespace KHECore
{

class Bookmark;


class BookmarksConstIteratorAdapter : public QSharedData
{
  protected:
    BookmarksConstIteratorAdapter();
  public:
    virtual ~BookmarksConstIteratorAdapter();

  public:
    virtual bool hasNext() const = 0;
    virtual bool hasPrevious() const = 0;
    virtual const KHECore::Bookmark& peekNext() const = 0;
    virtual const KHECore::Bookmark& peekPrevious() const = 0;

  public:
    virtual bool findNext( const KHECore::Bookmark& bookmark ) = 0;
    virtual bool findPrevious( const KHECore::Bookmark& bookmark ) = 0;
    virtual bool findNextFrom( unsigned int offset ) = 0;
    virtual bool findPreviousFrom( unsigned int offset ) = 0;
    virtual const KHECore::Bookmark& next() = 0;
    virtual const KHECore::Bookmark& previous() = 0;
    virtual void toBack() = 0;
    virtual void toFront() = 0;
};


inline BookmarksConstIteratorAdapter::BookmarksConstIteratorAdapter() {}
inline BookmarksConstIteratorAdapter::~BookmarksConstIteratorAdapter() {}

}

#endif
