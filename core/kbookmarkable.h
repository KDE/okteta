/*
    This file is part of the Okteta Core library, part of the KDE project.

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

#ifndef KHE_CORE_BOOKMARKABLE_H
#define KHE_CORE_BOOKMARKABLE_H

// Qt
#include <QtCore/QtPlugin>

template <class T> class QList;


namespace KHECore
{

class BookmarksConstIterator;
class Bookmark;
// interface to a list of bookmarks, sorted by the offset
//
class Bookmarkable
{
  public:
    virtual ~Bookmarkable();

  public: // set/action
    virtual void addBookmarks( const QList<KHECore::Bookmark> &bookmarks ) = 0;
    virtual void removeBookmarks( const QList<KHECore::Bookmark> &bookmarks ) = 0;
    virtual void removeAllBookmarks() = 0;
    virtual void setBookmark( unsigned int index, const KHECore::Bookmark& bookmark ) = 0;

  public: // get
    virtual KHECore::BookmarksConstIterator createBookmarksConstIterator() const = 0;
//     virtual BookmarksMutableIterator createBookmarksMutableIterator() const = 0;
    virtual const KHECore::Bookmark& bookmarkAt( unsigned int index ) const = 0;
    virtual const KHECore::Bookmark& bookmarkFor( int offset ) const = 0;
    virtual bool containsBookmarkFor( int offset ) const = 0;
    virtual unsigned int bookmarksCount() const = 0;

  public: // signal
    // TODO: use also QList<int> for added?
    virtual void bookmarksAdded( const QList<KHECore::Bookmark> &bookmarks ) = 0;
    virtual void bookmarksRemoved( const QList<KHECore::Bookmark> &bookmarks ) = 0;
    virtual void bookmarksModified( const QList<int>& indizes ) = 0;
    virtual void bookmarksModified( bool modified ) = 0;
};

inline Bookmarkable::~Bookmarkable() {}

}

Q_DECLARE_INTERFACE( KHECore::Bookmarkable, "org.kde.khecore.bookmarkable/1.0" )

#endif
