/***************************************************************************
                          kibookmarks.h  -  description
                             -------------------
    begin                : Fri Nov 30 2007
    copyright            : 2007 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License version 2 as published by the Free Software Foundation.       *
 *                                                                         *
 ***************************************************************************/

#ifndef KDE_IF_BOOKMARKS_H
#define KDE_IF_BOOKMARKS_H

// lib
#include "kbookmarklist.h"
// Qt
#include <QtCore/QtPlugin>
#include <QtCore/QList>

namespace KDE
{
namespace If
{
// TODO: add and remove with lists, not only single items
class Bookmarks
{
  public:
    virtual ~Bookmarks();

  public: // set/action
    virtual void addBookmarks( const QList<KHECore::KBookmark> &bookmarks ) = 0;
    virtual void removeBookmarks( const QList<KHECore::KBookmark> &bookmarks ) = 0;
    virtual void removeAllBookmarks() = 0;

  public: // get
    virtual KHECore::KBookmarkList bookmarkList() const = 0;

  public: // signal
    virtual void bookmarksAdded( const QList<KHECore::KBookmark> &bookmarks ) = 0;
    virtual void bookmarksRemoved( const QList<KHECore::KBookmark> &bookmarks ) = 0;
//     virtual void bookmarksModified( const QList<KHECore::KBookmark> &bookmarks ) = 0; TODO; signal which changed?
    virtual void bookmarksModified( bool modified ) = 0;
};

inline Bookmarks::~Bookmarks() {}

}
}

Q_DECLARE_INTERFACE( KDE::If::Bookmarks, "org.kde.if.bookmarks/1.0" )

#endif
