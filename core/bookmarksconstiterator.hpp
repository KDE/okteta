/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_BOOKMARKSCONSTITERATOR_HPP
#define OKTETA_BOOKMARKSCONSTITERATOR_HPP

// lib
#include "bookmarksconstiteratoradapter.hpp"
#include "oktetacore_export.hpp"
// Qt
#include <QExplicitlySharedDataPointer>

namespace Okteta {

class OKTETACORE_EXPORT BookmarksConstIterator
{
public:
    explicit BookmarksConstIterator(BookmarksConstIteratorAdapter* adapter = nullptr);

public:
    bool hasList() const;

public:
    bool hasNext() const;
    bool hasPrevious() const;
    const Okteta::Bookmark& peekNext() const;
    const Okteta::Bookmark& peekPrevious() const;

public:
    bool findNext(const Okteta::Bookmark& bookmark);
    bool findPrevious(const Okteta::Bookmark& bookmark);
    bool findNextFrom(unsigned int offset);
    bool findPreviousFrom(unsigned int offset);
    const Okteta::Bookmark& next();
    const Okteta::Bookmark& previous();
    void toBack();
    void toFront();

private:
    QExplicitlySharedDataPointer<BookmarksConstIteratorAdapter> mAdapter;
};

inline BookmarksConstIterator::BookmarksConstIterator(BookmarksConstIteratorAdapter* adapter) : mAdapter(adapter) {}

inline bool BookmarksConstIterator::hasList() const { return mAdapter; }

inline bool BookmarksConstIterator::hasNext()     const { return mAdapter->hasNext(); }
inline bool BookmarksConstIterator::hasPrevious() const { return mAdapter->hasPrevious(); }
inline const Okteta::Bookmark& BookmarksConstIterator::peekNext()     const { return mAdapter->peekNext(); }
inline const Okteta::Bookmark& BookmarksConstIterator::peekPrevious() const { return mAdapter->peekPrevious(); }

inline bool BookmarksConstIterator::findNext(const Okteta::Bookmark& bookmark)     { return mAdapter->findNext(bookmark); }
inline bool BookmarksConstIterator::findPrevious(const Okteta::Bookmark& bookmark) { return mAdapter->findPrevious(bookmark); }
inline bool BookmarksConstIterator::findNextFrom(unsigned int offset)     { return mAdapter->findNextFrom(offset); }
inline bool BookmarksConstIterator::findPreviousFrom(unsigned int offset) { return mAdapter->findPreviousFrom(offset); }
inline const Okteta::Bookmark& BookmarksConstIterator::next()     { return mAdapter->next(); }
inline const Okteta::Bookmark& BookmarksConstIterator::previous() { return mAdapter->previous(); }
inline void BookmarksConstIterator::toBack()  { mAdapter->toBack(); }
inline void BookmarksConstIterator::toFront() { mAdapter->toFront(); }

}

#endif
