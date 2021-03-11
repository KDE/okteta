/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_BOOKMARKSCONSTITERATORADAPTER_HPP
#define OKTETA_BOOKMARKSCONSTITERATORADAPTER_HPP

// lib
#include "address.hpp"
// Qt
#include <QSharedData>

namespace Okteta {

class Bookmark;

class BookmarksConstIteratorAdapter : public QSharedData
{
protected:
    BookmarksConstIteratorAdapter();

public:
    virtual ~BookmarksConstIteratorAdapter();

public:
    virtual bool hasNext() const = 0;
    virtual bool hasPrevious() const = 0;
    virtual const Bookmark& peekNext() const = 0;
    virtual const Bookmark& peekPrevious() const = 0;

public:
    virtual bool findNext(const Bookmark& bookmark) = 0;
    virtual bool findPrevious(const Bookmark& bookmark) = 0;
    virtual bool findNextFrom(Address offset) = 0;
    virtual bool findPreviousFrom(Address offset) = 0;
    virtual const Bookmark& next() = 0;
    virtual const Bookmark& previous() = 0;
    virtual void toBack() = 0;
    virtual void toFront() = 0;
};

inline BookmarksConstIteratorAdapter::BookmarksConstIteratorAdapter() = default;
inline BookmarksConstIteratorAdapter::~BookmarksConstIteratorAdapter() = default;

}

#endif
