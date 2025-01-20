/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_BOOKMARKLISTCONSTITERATORADAPTER_HPP
#define OKTETA_BOOKMARKLISTCONSTITERATORADAPTER_HPP

// lib
#include "bookmarksconstiteratoradapter.hpp"
// Qt
#include <QLinkedListIterator>

namespace Okteta {
class BookmarkList;

class BookmarkListConstIteratorAdapter : public BookmarksConstIteratorAdapter
{
public:
    explicit BookmarkListConstIteratorAdapter(const BookmarkList& list);
    ~BookmarkListConstIteratorAdapter() override;

public: // BookmarksConstIteratorAdapter API
    [[nodiscard]]
    bool hasNext() const override;
    [[nodiscard]]
    bool hasPrevious() const override;
    [[nodiscard]]
    const Bookmark& peekNext() const override;
    [[nodiscard]]
    const Bookmark& peekPrevious() const override;

    [[nodiscard]]
    bool findNext(const Bookmark& bookmark) override;
    [[nodiscard]]
    bool findPrevious(const Bookmark& bookmark) override;
    [[nodiscard]]
    bool findNextFrom(Address offset) override;
    [[nodiscard]]
    bool findPreviousFrom(Address offset) override;
    [[nodiscard]]
    const Bookmark& next() override;
    [[nodiscard]]
    const Bookmark& previous() override;
    void toBack() override;
    void toFront() override;

private:
    QLinkedListIterator<Bookmark> mIterator;
};

}

#endif
