/*
    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bookmarklistconstiteratoradapter.hpp"

namespace Okteta {

BookmarkListConstIteratorAdapter::BookmarkListConstIteratorAdapter(const BookmarkList& list)
    : m_list(list)
    , m_iterator(m_list.begin())
{
}

BookmarkListConstIteratorAdapter::~BookmarkListConstIteratorAdapter() = default;

bool BookmarkListConstIteratorAdapter::hasNext() const
{
    return m_iterator != m_list.end();
}

bool BookmarkListConstIteratorAdapter::hasPrevious() const
{
    return m_iterator != m_list.begin();
}

const Bookmark& BookmarkListConstIteratorAdapter::peekNext() const
{
    return *m_iterator;
}

const Bookmark& BookmarkListConstIteratorAdapter::peekPrevious() const
{
    BookmarkList::ConstIterator p = m_iterator;
    return *(--p);
}

bool BookmarkListConstIteratorAdapter::findNext(const Bookmark& bookmark)
{
    while (m_iterator != m_list.end()) {
        if (*(m_iterator++) == bookmark) {
            return true;
        }
    }
    return false;
}

bool BookmarkListConstIteratorAdapter::findPrevious(const Bookmark& bookmark)
{
    while (m_iterator != m_list.begin()) {
        if (*(--m_iterator) == bookmark) {
            return true;
        }
    }
    return false;
}

bool BookmarkListConstIteratorAdapter::findNextFrom(Address offset)
{
    bool result = false;

    toFront();
    while (hasNext()) {
        if (offset <= peekNext().offset()) {
            result = true;
            break;
        }
        std::ignore = next();
    }

    return result;
}

bool BookmarkListConstIteratorAdapter::findPreviousFrom(Address offset)
{
    bool result = false;

    toBack();
    while (hasPrevious()) {
        if (peekPrevious().offset() <= offset) {
            result = true;
            break;
        }
        std::ignore = previous();
    }

    return result;
}

const Bookmark& BookmarkListConstIteratorAdapter::next()
{
    return *(m_iterator++);
}

const Bookmark& BookmarkListConstIteratorAdapter::previous()
{
    return *(--m_iterator);
}

void BookmarkListConstIteratorAdapter::toBack()
{
    m_iterator = m_list.end();
}

void BookmarkListConstIteratorAdapter::toFront()
{
    m_iterator = m_list.begin();
}

}
