/*
    This file is part of the Okteta Core library, part of the KDE project.

    Copyright 2007 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef KHE_CORE_BOOKMARKLIST_H
#define KHE_CORE_BOOKMARKLIST_H

// lib
#include "bookmark.h"
// Qt
#include <QtCore/QList>
#include <QtCore/QLinkedList>


namespace KHECore {

/**
@author Friedrich W. H.  Kossebau
*/
class OKTETACORE_EXPORT BookmarkList : public QLinkedList<Bookmark>
{
  public:
    BookmarkList();
    ~BookmarkList();

  public:
    void addBookmark( const Bookmark &bookmark );
    void addBookmarks( const QList<KHECore::Bookmark> &bookmarks );
    void removeBookmark( const Bookmark &bookmark );
    void removeBookmarks( const QList<KHECore::Bookmark> &bookmarks );

    bool adjustToReplaced( int offset, int removedLength, int insertedLength );
    bool adjustToSwapped( int firstPartStart, int secondPartStart, int secondPartLength );

  public:
    bool contains( int offset ) const;
    using QLinkedList<Bookmark>::contains;
    const_iterator previousFrom( int offset ) const;
    const_iterator nextFrom( int offset ) const;
//     const_iterator begin( int offset ) const;
//     using QLinkedList<Bookmark>::begin;
    QList<KHECore::Bookmark> list() const;
};

}

#endif
