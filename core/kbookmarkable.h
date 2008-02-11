/***************************************************************************
                          kbookmarkable.h  -  description
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

#ifndef KHE_CORE_BOOKMARKABLE_H
#define KHE_CORE_BOOKMARKABLE_H

// lib
#include "kbookmarklist.h"
// Qt
#include <QtCore/QtPlugin>
#include <QtCore/QList>

namespace KHECore
{
// TODO: add and remove with lists, not only single items
class Bookmarkable
{
  public:
    virtual ~Bookmarkable();

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

inline Bookmarkable::~Bookmarkable() {}

}

Q_DECLARE_INTERFACE( KHECore::Bookmarkable, "org.kde.khecore.bookmarkable/1.0" )

#endif
