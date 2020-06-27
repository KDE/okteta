/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_EXTERNALBOOKMARKSTORAGE_HPP
#define KASTEN_EXTERNALBOOKMARKSTORAGE_HPP

class KBookmarkManager;
class QUrl;

namespace Kasten {
class ByteArrayDocument;

class ExternalBookmarkStorage
{
public:
    ExternalBookmarkStorage();

    ~ExternalBookmarkStorage();

public:
    void readBookmarks(ByteArrayDocument* document, const QUrl& url);
    void writeBookmarks(ByteArrayDocument* document, const QUrl& url);

private:
    KBookmarkManager* mBookmarkManager;
};

}

#endif
