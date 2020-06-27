/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bookmarkstool.hpp"

// Okteta Kasten gui
#include <Kasten/Okteta/ByteArrayView>
// Okteta Kasten core
#include <Kasten/Okteta/ByteArrayDocument>
// Kasten core
#include <Kasten/AbstractModel>
// Okteta gui
#include <Okteta/OffsetFormat>
// Okteta core
#include <Okteta/TextByteArrayAnalyzer>
#include <Okteta/CharCodec>
#include <Okteta/Bookmarkable>
#include <Okteta/BookmarksConstIterator>
#include <Okteta/Bookmark>
#include <Okteta/AbstractByteArrayModel>
// KF
#include <KLocalizedString>
// Qt
#include <QWidget>

namespace Kasten {

BookmarksTool::BookmarksTool()
{
    setObjectName(QStringLiteral("Bookmarks"));
}

BookmarksTool::~BookmarksTool() = default;

QString BookmarksTool::title() const { return i18nc("@title:window", "Bookmarks"); }
bool BookmarksTool::canCreateBookmark() const { return mCanCreateBookmark; }
const Okteta::Bookmark& BookmarksTool::bookmarkAt(unsigned int index) const { return mBookmarks->bookmarkAt(index); }
int BookmarksTool::indexOf(const Okteta::Bookmark& bookmark) const
{
    int result = -1;

    Okteta::BookmarksConstIterator bit = mBookmarks->createBookmarksConstIterator();
    int i = 0;
    while (bit.hasNext()) {
        if (bookmark == bit.next()) {
            result = i;
            break;
        }
        ++i;
    }
    return result;
}
unsigned int BookmarksTool::bookmarksCount() const { return mBookmarks ? mBookmarks->bookmarksCount() : 0; }
int BookmarksTool::offsetCoding()            const { return mByteArrayView ? mByteArrayView->offsetCoding() : 0; }

void BookmarksTool::setTargetModel(AbstractModel* model)
{
    const int oldOffsetCoding = offsetCoding();

    if (mByteArrayView) {
        mByteArrayView->disconnect(this);
    }
    if (mByteArray) {
        mByteArray->disconnect(this);
    }

    mByteArrayView = model ? model->findBaseModel<ByteArrayView*>() : nullptr;

    ByteArrayDocument* document =
        mByteArrayView ? qobject_cast<ByteArrayDocument*>(mByteArrayView->baseModel()) : nullptr;
    mByteArray = document ? document->content() : nullptr;
    mBookmarks = (mByteArray && mByteArrayView) ? qobject_cast<Okteta::Bookmarkable*>(mByteArray) : nullptr;

    const bool hasViewWithBookmarks = (mBookmarks != nullptr);
    if (hasViewWithBookmarks) {
        onCursorPositionChanged(mByteArrayView->cursorPosition());

        connect(mByteArray, SIGNAL(bookmarksAdded(QVector<Okteta::Bookmark>)),
                SIGNAL(bookmarksAdded(QVector<Okteta::Bookmark>)));
        connect(mByteArray, SIGNAL(bookmarksRemoved(QVector<Okteta::Bookmark>)),
                SIGNAL(bookmarksRemoved(QVector<Okteta::Bookmark>)));
        connect(mByteArray, SIGNAL(bookmarksAdded(QVector<Okteta::Bookmark>)),
                SLOT(onBookmarksModified()));
        connect(mByteArray, SIGNAL(bookmarksRemoved(QVector<Okteta::Bookmark>)),
                SLOT(onBookmarksModified()));
        connect(mByteArray, SIGNAL(bookmarksModified(QVector<int>)),
                SIGNAL(bookmarksModified(QVector<int>)));
        connect(mByteArrayView, &ByteArrayView::cursorPositionChanged,
                this, &BookmarksTool::onCursorPositionChanged);
        connect(mByteArrayView, &ByteArrayView::offsetCodingChanged,
                this, &BookmarksTool::offsetCodingChanged);
    } else {
        constexpr bool cantCreateBookmark = false;
        if (mCanCreateBookmark != cantCreateBookmark) {
            mCanCreateBookmark = cantCreateBookmark;
            emit canCreateBookmarkChanged(cantCreateBookmark);
        }
    }

    const int newOffsetCoding = offsetCoding();
    if (oldOffsetCoding != newOffsetCoding) {
        emit offsetCodingChanged(newOffsetCoding);
    }
    emit hasBookmarksChanged(hasViewWithBookmarks);
}

Okteta::Bookmark BookmarksTool::createBookmark()
{
    Okteta::Bookmark bookmark;

    if (mBookmarks) {
        const int cursorPosition = mByteArrayView->cursorPosition();

        // search for text at cursor
        const Okteta::CharCodec* charCodec = Okteta::CharCodec::createCodec(mByteArrayView->charCodingName());
        const Okteta::TextByteArrayAnalyzer textAnalyzer(mByteArray, charCodec);
        QString bookmarkName = textAnalyzer.text(cursorPosition, cursorPosition + MaxBookmarkNameSize - 1);
        delete charCodec;

        if (bookmarkName.isEmpty()) {
            bookmarkName = i18nc("default name of a bookmark", "Bookmark");
        }
        // %1").arg( 0 ) ); // TODO: use counter like with new file, globally

        bookmark.setOffset(mByteArrayView->cursorPosition());
        bookmark.setName(bookmarkName);

        const QVector<Okteta::Bookmark> bookmarksToBeCreated { bookmark };
        mBookmarks->addBookmarks(bookmarksToBeCreated);
    }

    return bookmark;
}

void BookmarksTool::deleteBookmarks(const QVector<Okteta::Bookmark>& bookmarks)
{
    if (mBookmarks) {
        mBookmarks->removeBookmarks(bookmarks);
    }
    mByteArrayView->widget()->setFocus();
}

void BookmarksTool::gotoBookmark(const Okteta::Bookmark& bookmark)
{
    if (mByteArrayView) {
        mByteArrayView->setCursorPosition(bookmark.offset());
        mByteArrayView->widget()->setFocus();
    }
}

void BookmarksTool::setBookmarkName(unsigned int bookmarkIndex, const QString& name)
{
    Okteta::Bookmark bookmark = mBookmarks->bookmarkAt(bookmarkIndex);

    bookmark.setName(name);
    mBookmarks->setBookmark(bookmarkIndex, bookmark);

    mByteArrayView->widget()->setFocus();
}

void BookmarksTool::onCursorPositionChanged(Okteta::Address newPosition)
{
    const int bookmarksCount = mBookmarks->bookmarksCount();
    const bool hasBookmarks = (bookmarksCount != 0);
    const bool isInsideByteArray = (newPosition < mByteArray->size());
    const bool isAtBookmark = hasBookmarks ? mBookmarks->containsBookmarkFor(newPosition) : false;
    const bool canCreateBookmark = (!isAtBookmark && isInsideByteArray);

    if (canCreateBookmark != mCanCreateBookmark) {
        mCanCreateBookmark = canCreateBookmark;
        emit canCreateBookmarkChanged(canCreateBookmark);
    }
}

// TODO: is a hack
// better just only check for the added and removed, if they include the current position, then change mCanCreateBookmark
void BookmarksTool::onBookmarksModified()
{
    const int cursorPosition = mByteArrayView->cursorPosition();
    onCursorPositionChanged(cursorPosition);
}

}
