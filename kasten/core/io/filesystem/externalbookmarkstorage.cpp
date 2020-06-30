/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "externalbookmarkstorage.hpp"

// lib
#include "bytearraydocument.hpp"
// Okteta core
#include <Okteta/Bookmarkable>
#include <Okteta/BookmarksConstIterator>
#include <Okteta/Bookmark>
#include <Okteta/AbstractByteArrayModel>
// KF
#include <KBookmarkManager>
// Qt
#include <QStandardPaths>

namespace Kasten {

ExternalBookmarkStorage::ExternalBookmarkStorage()
{
    const QString bookmarksFileName =
        QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + QLatin1String("/okteta/bookmarks.xml");
    mBookmarkManager = KBookmarkManager::managerForFile(bookmarksFileName, QStringLiteral("okteta"));
}

ExternalBookmarkStorage::~ExternalBookmarkStorage() = default;

void ExternalBookmarkStorage::readBookmarks(ByteArrayDocument* document, const QUrl& url)
{
    Okteta::AbstractByteArrayModel* byteArray = document->content();
    Okteta::Bookmarkable* bookmarkable = qobject_cast<Okteta::Bookmarkable*>(byteArray);

    bookmarkable->removeAllBookmarks();

    const QString urlString = url.toDisplayString(QUrl::PrettyDecoded | QUrl::PreferLocalFile);

    KBookmarkGroup root = mBookmarkManager->root();

    for (KBookmark bm = root.first(); !bm.isNull(); bm = root.next(bm)) {
        if (bm.isSeparator() || !bm.isGroup()) {
            continue;
        }

        if (bm.fullText() == urlString) {
            KBookmarkGroup bmGroup = bm.toGroup();

            QVector<Okteta::Bookmark> bookmarksToBeCreated;
            Okteta::Bookmark bookmark;
            for (bm = bmGroup.first(); !bm.isNull(); bm = bmGroup.next(bm)) {
                if (bm.isSeparator() || bm.isGroup()) {
                    continue;
                }

                bookmark.setOffset(bm.url().fragment().toULongLong());
                bookmark.setName(bm.fullText());

                bookmarksToBeCreated.append(bookmark);
            }

            bookmarkable->addBookmarks(bookmarksToBeCreated);

            break;
        }
    }
}

void ExternalBookmarkStorage::writeBookmarks(ByteArrayDocument* document, const QUrl& url)
{
    Okteta::AbstractByteArrayModel* byteArray = document->content();
    Okteta::Bookmarkable* bookmarkable = qobject_cast<Okteta::Bookmarkable*>(byteArray);

    if (!bookmarkable) {
        return;
    }

    const QString urlString = url.toDisplayString(QUrl::PrettyDecoded | QUrl::PreferLocalFile);

    KBookmarkGroup root = mBookmarkManager->root();

    // rm old bookmarkable
    KBookmark bm = root.first();
    while (!bm.isNull()) {
        if (bm.isSeparator() || !bm.isGroup()) {
            continue;
        }

        if (bm.fullText() == urlString) {
            root.deleteBookmark(bm);
            break;
        }

        bm = root.next(bm);
    }

    // store current bookmarks
    KBookmarkGroup bookmarkGroup = root.createNewFolder(urlString);
    Okteta::BookmarksConstIterator bit = bookmarkable->createBookmarksConstIterator();
    while (bit.hasNext()) {
        const Okteta::Bookmark& bookmark = bit.next();
        QUrl bookmarkUrl = url;
        bookmarkUrl.setFragment(QString::number(bookmark.offset()));
        bookmarkGroup.addBookmark(bookmark.name(), bookmarkUrl, QString());
    }

    mBookmarkManager->save(false);
}

}
