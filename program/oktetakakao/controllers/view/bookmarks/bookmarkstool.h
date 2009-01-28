/*
    This file is part of the Kakao Framework, part of the KDE project.

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

#ifndef BOOKMARKSTOOL_H
#define BOOKMARKSTOOL_H

// Kakao core
#include <abstracttool.h>

class KByteArrayDisplay;
namespace KHECore {
class Bookmarkable;
class Bookmark;
class AbstractByteArrayModel;
}
template <class T> class QList;


class BookmarksTool : public AbstractTool
{
  Q_OBJECT

  public:
    BookmarksTool();
    virtual ~BookmarksTool();

  public:
    bool hasBookmarks() const;
    bool canCreateBookmark() const;
    const KHECore::Bookmark& bookmarkAt( unsigned int index ) const;
    int indexOf( const KHECore::Bookmark& bookmark ) const;
    unsigned int bookmarksCount() const;

  public: // AbstractTool API
//     virtual AbstractModel* targetModel() const;
    virtual QString title() const;

    virtual void setTargetModel( AbstractModel* model );

  public:
    KHECore::Bookmark createBookmark();
    void gotoBookmark( const KHECore::Bookmark& bookmark );
    void setBookmarkName( unsigned int bookmarkIndex, const QString& name );
    void deleteBookmarks( const QList<KHECore::Bookmark>& bookmarks );

  Q_SIGNALS:
    void hasBookmarksChanged( bool hasBookmarks );
    void bookmarksAdded( const QList<KHECore::Bookmark>& bookmarks );
    void bookmarksRemoved( const QList<KHECore::Bookmark>& bookmarks );
    void bookmarksModified( const QList<int>& indizes );
    void canCreateBookmarkChanged( bool canCreateBookmark );

  protected Q_SLOTS:
    void onCursorPositionChanged( int newPosition );
    void onBookmarksModified();

  protected: // sources
    KByteArrayDisplay* mByteArrayDisplay;
    KHECore::AbstractByteArrayModel* mByteArray;
    KHECore::Bookmarkable* mBookmarks;

    bool mCanCreateBookmark;
};

#endif
