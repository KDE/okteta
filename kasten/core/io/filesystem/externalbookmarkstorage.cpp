/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library. If not, see <http://www.gnu.org/licenses/>.
*/


#include "externalbookmarkstorage.h"

// lib
#include "bytearraydocument.h"
// Okteta core
#include <bookmarkable.h>
#include <bookmarksconstiterator.h>
#include <bookmark.h>
#include <abstractbytearraymodel.h>
// KDE
#include <KStandardDirs>
#include <KBookmarkManager>
#include <KBookmarkGroup>


namespace Kasten2
{

ExternalBookmarkStorage::ExternalBookmarkStorage()
{
    const QString bookmarksFileName =
        KStandardDirs::locateLocal( "data", QLatin1String("okteta/bookmarks.xml") );
    mBookmarkManager = KBookmarkManager::managerForFile( bookmarksFileName, QLatin1String("okteta") );
}

 
void ExternalBookmarkStorage::readBookmarks( ByteArrayDocument* document, const KUrl& url )
{
    Okteta::AbstractByteArrayModel* byteArray = document->content();
    Okteta::Bookmarkable* bookmarkable = qobject_cast<Okteta::Bookmarkable*>( byteArray );

    bookmarkable->removeAllBookmarks();

    const QString urlString = url.isLocalFile() ? url.path() : url.prettyUrl();

    KBookmarkGroup root = mBookmarkManager->root();

    for( KBookmark bm = root.first(); ! bm.isNull(); bm = root.next(bm) )
    {
        if( bm.isSeparator() || ! bm.isGroup() )
            continue;

        if( bm.fullText() == urlString )
        {
            KBookmarkGroup bmGroup = bm.toGroup();

            QList<Okteta::Bookmark> bookmarksToBeCreated;
            Okteta::Bookmark bookmark;
            for( bm = bmGroup.first(); ! bm.isNull(); bm = bmGroup.next(bm) )
            {
                if( bm.isSeparator() || bm.isGroup() )
                    continue;

                bookmark.setOffset( bm.url().htmlRef().toULongLong() );
                bookmark.setName( bm.fullText() );

                bookmarksToBeCreated.append( bookmark );
            }

            bookmarkable->addBookmarks( bookmarksToBeCreated );

            break;
        }
    }
}

void ExternalBookmarkStorage::writeBookmarks( ByteArrayDocument* document, const KUrl& url )
{
    Okteta::AbstractByteArrayModel* byteArray = document->content();
    Okteta::Bookmarkable* bookmarkable = qobject_cast<Okteta::Bookmarkable*>( byteArray );

    if( ! bookmarkable )
        return;

    const QString urlString = url.isLocalFile() ? url.path() : url.prettyUrl();

    KBookmarkGroup root = mBookmarkManager->root();

    // rm old bookmarkable
    KBookmark bm = root.first();
    while( ! bm.isNull() )
    {
        if( bm.isSeparator() || ! bm.isGroup() )
            continue;

        if( bm.fullText() == urlString )
        {
            root.deleteBookmark( bm );
            break;
        }

        bm = root.next( bm );
    }

    // store current bookmarks
    KBookmarkGroup bookmarkGroup = root.createNewFolder( urlString );
    Okteta::BookmarksConstIterator bit = bookmarkable->createBookmarksConstIterator();
    while( bit.hasNext() )
    {
        const Okteta::Bookmark& bookmark = bit.next();
        KUrl bookmarkUrl = url;
        bookmarkUrl.setHTMLRef( QString::number(bookmark.offset()) );
        bookmarkGroup.addBookmark( bookmark.name(), bookmarkUrl, QString() );
    }

    mBookmarkManager->save( false );
}

ExternalBookmarkStorage::~ExternalBookmarkStorage() {}

}
