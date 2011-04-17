/*
    This file is part of the Okteta Core library, made within the KDE community.

    Copyright 2007,2009 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef OKTETA_BOOKMARKLIST_H
#define OKTETA_BOOKMARKLIST_H

// lib
#include "bookmark.h"
// Qt
#include <QtCore/QLinkedList>

template <class T> class QList;


namespace Okteta
{

/**
@author Friedrich W. H.  Kossebau
*/
class BookmarkList : public QLinkedList<Bookmark>
{
  public:
    BookmarkList();
    ~BookmarkList();

  public:
    void addBookmark( const Bookmark &bookmark );
    void addBookmarks( const QList<Okteta::Bookmark> &bookmarks );
    void removeBookmark( const Bookmark &bookmark );
    void removeBookmarks( const QList<Okteta::Bookmark> &bookmarks );
    void setBookmark( unsigned int index, const Bookmark& bookmark );

    bool adjustToReplaced( Address offset, Size removedLength, Size insertedLength );
    bool adjustToSwapped( Address firstPartStart, Address secondPartStart, Size secondPartLength );

  public:
    // TODO: this function needs to be called with a valid offset, will return a reference to a zero pointer else
    // want a reference for speed, perhaps need a global static dummy invalid bookmark
    const Bookmark& bookmark( Address offset ) const;
    bool contains( Address offset ) const;
    using QLinkedList<Bookmark>::contains;
    const Bookmark& at( unsigned int index ) const;
    QList<Okteta::Bookmark> list() const;
};

}

#endif
