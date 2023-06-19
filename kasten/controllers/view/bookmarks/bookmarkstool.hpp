/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BOOKMARKSTOOL_HPP
#define KASTEN_BOOKMARKSTOOL_HPP

// Kasten core
#include <Kasten/AbstractTool>
// Okteta core
#include <Okteta/Address>

namespace Okteta {
class Bookmarkable;
class Bookmark;
class AbstractByteArrayModel;
}
template <class T> class QVector;

namespace Kasten {

class ByteArrayView;

class BookmarksTool : public AbstractTool
{
    Q_OBJECT

private:
    static constexpr int MaxBookmarkNameSize = 40;

public:
    BookmarksTool();
    ~BookmarksTool() override;

public:
    bool hasBookmarks() const;
    bool canCreateBookmark() const;
    const Okteta::Bookmark& bookmarkAt(unsigned int index) const;
    int indexOf(const Okteta::Bookmark& bookmark) const;
    unsigned int bookmarksCount() const;
    int offsetCoding() const;

public: // AbstractTool API
//     virtual AbstractModel* targetModel() const;
    QString title() const override;

    void setTargetModel(AbstractModel* model) override;

public:
    Okteta::Bookmark createBookmark();
    void gotoBookmark(const Okteta::Bookmark& bookmark);
    void setBookmarkName(unsigned int bookmarkIndex, const QString& name);
    void deleteBookmarks(const QVector<Okteta::Bookmark>& bookmarks);

Q_SIGNALS:
    void hasBookmarksChanged(bool hasBookmarks);
    void bookmarksAdded(const QVector<Okteta::Bookmark>& bookmarks);
    void bookmarksRemoved(const QVector<Okteta::Bookmark>& bookmarks);
    void bookmarksModified(const QVector<int>& indizes);
    void canCreateBookmarkChanged(bool canCreateBookmark);
    void offsetCodingChanged(int offsetCoding);

private Q_SLOTS:
    void onCursorPositionChanged(Okteta::Address newPosition);
    void onBookmarksModified();

private: // sources
    ByteArrayView* mByteArrayView = nullptr;
    Okteta::AbstractByteArrayModel* mByteArray = nullptr;
    Okteta::Bookmarkable* mBookmarks = nullptr;

    bool mCanCreateBookmark = false;
};

}

#endif
