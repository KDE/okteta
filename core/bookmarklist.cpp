/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2007, 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bookmarklist.hpp"

// Qt
#include <QVector>

namespace Okteta {

BookmarkList::BookmarkList() = default;

BookmarkList::~BookmarkList() = default;

void BookmarkList::addBookmark(const Bookmark& bookmark)
{
    if (!bookmark.isValid()) {
        return;
    }

    iterator B = begin();
    for (; B != end(); ++B) {
        // new bookmark before next bookmark?
        if (bookmark.offset() < B->offset()) {
            // put the new before it
            insert(B, bookmark);
            return;
        }

        // bookmark already present?
        if (bookmark.offset() == B->offset()) {
            *B = bookmark;
            return;
        }
    }

    // all others before the new?
    if (B == end()) {
        // add it at the end
        append(bookmark);
    }
}

void BookmarkList::addBookmarks(const QVector<Okteta::Bookmark>& bookmarks)
{
    for (const Bookmark& bookmark : bookmarks) {
        addBookmark(bookmark);
    }
}

void BookmarkList::removeBookmark(const Bookmark& bookmark)
{
    if (!bookmark.isValid()) {
        return;
    }

    iterator B = begin();
    for (; B != end(); ++B) {
        if (bookmark.offset() == B->offset()) {
            erase(B);
            break;
        }
    }
}

void BookmarkList::removeBookmarks(const QVector<Okteta::Bookmark>& bookmarks)
{
    for (const Bookmark& bookmark : bookmarks) {
        removeBookmark(bookmark);
    }
}

void BookmarkList::setBookmark(unsigned int index, const Bookmark& bookmark)
{
    const Iterator endIt = end();
    unsigned int i = 0;
    for (Iterator it = begin(); it != endIt; ++it, ++i) {
        if (i == index) {
            *it = bookmark;
            break;
        }
    }
}

bool BookmarkList::adjustToReplaced(Address offset, Size removedLength, Size insertedLength)
{
    bool result = false;

    iterator bIt = begin();
    while (bIt != end() && bIt->offset() < offset) {
        ++bIt;
    }
    // remove bookmarks in removed section
    while (bIt != end() && bIt->offset() < offset + removedLength) {
        bIt = erase(bIt);
        result = true;
    }
    // adjust bookmarks in moved section
    const Size diff = insertedLength - removedLength;
    if (diff != 0) {
        for (; bIt != end(); ++bIt) {
            (*bIt).move(diff);
            result = true;
        }
    }

    return result;
}

bool BookmarkList::adjustToSwapped(Address firstPartStart, Address secondPartStart, Size secondPartLength)
{
    bool result = false;

    iterator bIt = begin();
    while (bIt != end() && bIt->offset() < firstPartStart) {
        ++bIt;
    }
    QVector<Okteta::Bookmark> bookmarksInFirstPart;
    // take bookmarks from first part
    while (bIt != end() && bIt->offset() < secondPartStart) {
        bookmarksInFirstPart.append(*bIt);
        bIt = erase(bIt);
    }
    // move bookmarks from second to first
    const Size diff = firstPartStart - secondPartStart;
    const Address behindLast = secondPartStart + secondPartLength;
    for (; bIt != end() && bIt->offset() < behindLast; ++bIt) {
        (*bIt).move(diff);
        result = true;
    }

    // append bookmarks from first part as second
    if (!bookmarksInFirstPart.isEmpty()) {
        for (Bookmark bookmark : qAsConst(bookmarksInFirstPart)) { // krazy:exclude=foreach
            bookmark.move(secondPartLength);
            insert(bIt, bookmark);
        }

        result = true;
    }

    return result;
}

QVector<Okteta::Bookmark> BookmarkList::list() const
{
    QVector<Okteta::Bookmark> result;
    result.reserve(size());

    for (const Bookmark& bookmark : *this) {
        result.append(bookmark);
    }

    return result;
}

const Bookmark& BookmarkList::bookmark(Address offset) const
{
    const ConstIterator endIt = end();
    auto it = std::find_if(begin(), endIt, [offset](const Bookmark& bookmark) {
        return (bookmark.offset() == offset);
    });
    if (it != endIt) {
        return *it;
    }

    static const Bookmark* const noBookmark = nullptr;

    return (const Bookmark&)*noBookmark;
}

bool BookmarkList::contains(Address offset) const
{
    return std::any_of(begin(), end(), [offset](const Bookmark& bookmark) {
        return (bookmark.offset() == offset);
    });
}

const Bookmark& BookmarkList::at(unsigned int index) const
{
    Q_ASSERT_X((int)index < size(), "BookmarkList::at", "index out of range");

    const ConstIterator endIt = end();
    unsigned int i = 0;
    for (ConstIterator it = begin(); it != endIt; ++it, ++i) {
        if (i == index) {
            return *it;
        }
    }

    static const Bookmark* const noBookmark = nullptr;
    return (const Bookmark&)*noBookmark;
}

}
