/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2007, 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_BOOKMARKABLE_HPP
#define OKTETA_BOOKMARKABLE_HPP

// Qt
#include <QtPlugin>

template <class T> class QVector;

namespace Okteta {

class BookmarksConstIterator;
class Bookmark;
// interface to a list of bookmarks, sorted by the offset
//
class Bookmarkable
{
public:
    virtual ~Bookmarkable();

public: // set/action
    virtual void addBookmarks(const QVector<Okteta::Bookmark>& bookmarks) = 0;
    virtual void removeBookmarks(const QVector<Okteta::Bookmark>& bookmarks) = 0;
    virtual void removeAllBookmarks() = 0;
    virtual void setBookmark(unsigned int index, const Okteta::Bookmark& bookmark) = 0;

public: // get
    virtual Okteta::BookmarksConstIterator createBookmarksConstIterator() const = 0;
//     virtual BookmarksMutableIterator createBookmarksMutableIterator() const = 0;
    virtual const Okteta::Bookmark& bookmarkAt(unsigned int index) const = 0;
    virtual const Okteta::Bookmark& bookmarkFor(int offset) const = 0;
    virtual bool containsBookmarkFor(int offset) const = 0;
    virtual unsigned int bookmarksCount() const = 0;

public: // signal
    // TODO: use also QVector<int> for added?
    virtual void bookmarksAdded(const QVector<Okteta::Bookmark>& bookmarks) = 0;
    virtual void bookmarksRemoved(const QVector<Okteta::Bookmark>& bookmarks) = 0;
    virtual void bookmarksModified(const QVector<int>& indizes) = 0;
    virtual void bookmarksModified(bool modified) = 0;
};

inline Bookmarkable::~Bookmarkable() = default;

}

Q_DECLARE_INTERFACE(Okteta::Bookmarkable, "org.kde.okteta.bookmarkable/1.0")

#endif
