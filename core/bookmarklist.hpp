/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2007, 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_BOOKMARKLIST_HPP
#define OKTETA_BOOKMARKLIST_HPP

// lib
#include "bookmark.hpp"
// Qt
#include <QLinkedList>

template <class T> class QVector;

namespace Okteta {

/**
   @author Friedrich W. H.  Kossebau
 */
class BookmarkList : public QLinkedList<Bookmark>
{
public:
    BookmarkList();
    ~BookmarkList();

public:
    void addBookmark(const Bookmark& bookmark);
    void addBookmarks(const QVector<Okteta::Bookmark>& bookmarks);
    void removeBookmark(const Bookmark& bookmark);
    void removeBookmarks(const QVector<Okteta::Bookmark>& bookmarks);
    void setBookmark(unsigned int index, const Bookmark& bookmark);

    bool adjustToReplaced(Address offset, Size removedLength, Size insertedLength);
    bool adjustToSwapped(Address firstPartStart, Address secondPartStart, Size secondPartLength);

public:
    // TODO: this function needs to be called with a valid offset, will return a reference to a zero pointer else
    // want a reference for speed, perhaps need a global static dummy invalid bookmark
    const Bookmark& bookmark(Address offset) const;
    bool contains(Address offset) const;
    using QLinkedList<Bookmark>::contains;
    const Bookmark& at(unsigned int index) const;
    QVector<Okteta::Bookmark> list() const;
};

}

#endif
