/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bookmarklistconstiteratoradapter.hpp"

// lib
#include "bookmarklist.hpp"

namespace Okteta {

BookmarkListConstIteratorAdapter::BookmarkListConstIteratorAdapter(const BookmarkList& list)
    : mIterator(list)
{
}

BookmarkListConstIteratorAdapter::~BookmarkListConstIteratorAdapter() = default;

bool BookmarkListConstIteratorAdapter::hasNext() const { return mIterator.hasNext(); }
bool BookmarkListConstIteratorAdapter::hasPrevious() const { return mIterator.hasPrevious(); }
const Bookmark& BookmarkListConstIteratorAdapter::peekNext() const { return mIterator.peekNext(); }
const Bookmark& BookmarkListConstIteratorAdapter::peekPrevious() const { return mIterator.peekPrevious(); }

bool BookmarkListConstIteratorAdapter::findNext(const Bookmark& bookmark)     { return mIterator.findNext(bookmark); }
bool BookmarkListConstIteratorAdapter::findPrevious(const Bookmark& bookmark) { return mIterator.findPrevious(bookmark); }

bool BookmarkListConstIteratorAdapter::findNextFrom(Address offset)
{
    bool result = false;

    mIterator.toFront();
    while (mIterator.hasNext()) {
        if (offset <= mIterator.peekNext().offset()) {
            result = true;
            break;
        }
        next();
    }

    return result;
}

bool BookmarkListConstIteratorAdapter::findPreviousFrom(Address offset)
{
    bool result = false;

    mIterator.toBack();
    while (mIterator.hasPrevious()) {
        if (mIterator.peekPrevious().offset() <= offset) {
            result = true;
            break;
        }
        previous();
    }

    return result;
}

const Bookmark& BookmarkListConstIteratorAdapter::next()     { return mIterator.next(); }
const Bookmark& BookmarkListConstIteratorAdapter::previous() { return mIterator.previous(); }

void BookmarkListConstIteratorAdapter::toBack()  { mIterator.toBack(); }
void BookmarkListConstIteratorAdapter::toFront() { mIterator.toFront(); }

}
