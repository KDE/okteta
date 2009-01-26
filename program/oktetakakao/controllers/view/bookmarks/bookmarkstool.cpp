/*
    This file is part of the Kakao Framework, part of the KDE project.

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

#include "bookmarkstool.h"

// Kakao core
#include <kbytearraydisplay.h>
#include <kbytearraydocument.h>
// Kakao core
#include <abstractmodel.h>
// Okteta gui
#include <koffsetformat.h>
// Okteta core
#include <wordbytearrayservice.h>
#include <charcodec.h>
#include <kbookmarkable.h>
#include <kbytearraymodel.h>
// KDE
#include <KLocale>
// Qt
#include <QtGui/QWidget>

static const int MaxBookmarkNameSize = 40;


BookmarksTool::BookmarksTool()
 : mByteArrayDisplay( 0 ), mByteArray( 0 ), mBookmarks( 0 ), mCanCreateBookmark( false )
{
    setObjectName( "Bookmarks" );
}


QString BookmarksTool::title() const { return i18nc("@title:window", "Bookmarks"); }
bool BookmarksTool::canCreateBookmark() const { return mCanCreateBookmark; }
KHECore::BookmarkList BookmarksTool::bookmarks() const
{
    KHECore::BookmarkList result;

    if( mBookmarks )
        result = mBookmarks->bookmarkList();

    return result;
}


void BookmarksTool::setTargetModel( AbstractModel* model )
{
    if( mByteArrayDisplay ) mByteArrayDisplay->disconnect( this );
    if( mByteArray ) mByteArray->disconnect( this );

    mByteArrayDisplay = model ? model->findBaseModel<KByteArrayDisplay*>() : 0;

    KByteArrayDocument* document =
        mByteArrayDisplay ? qobject_cast<KByteArrayDocument*>( mByteArrayDisplay->baseModel() ) : 0;
    mByteArray = document ? document->content() : 0;
    mBookmarks = ( mByteArray && mByteArrayDisplay ) ? qobject_cast<KHECore::Bookmarkable*>( mByteArray ) : 0;

    const bool hasViewWithBookmarks = ( mBookmarks != 0 );
    if( hasViewWithBookmarks )
    {
        onCursorPositionChanged( mByteArrayDisplay->cursorPosition() );

        connect( mByteArray, SIGNAL(bookmarksAdded( const QList<KHECore::Bookmark>& )),
                 SIGNAL(bookmarksAdded( const QList<KHECore::Bookmark>& )) );
        connect( mByteArray, SIGNAL(bookmarksRemoved( const QList<KHECore::Bookmark>& )),
                 SIGNAL(bookmarksRemoved( const QList<KHECore::Bookmark>& )) );
        connect( mByteArray, SIGNAL(bookmarksAdded( const QList<KHECore::Bookmark>& )),
                 SLOT(onBookmarksModified()) );
        connect( mByteArray, SIGNAL(bookmarksRemoved( const QList<KHECore::Bookmark>& )),
                 SLOT(onBookmarksModified()) );
//         connect( mByteArray, SIGNAL(bookmarksModified( bool )),
//                  SIGNAL(bookmarksModified( bool )) );
        connect( mByteArrayDisplay, SIGNAL(cursorPositionChanged( int )), SLOT(onCursorPositionChanged( int )) );
    }
    else
    {
        static const bool cantCreateBookmark = false;
        if( mCanCreateBookmark != cantCreateBookmark )
        {
            mCanCreateBookmark = cantCreateBookmark;
            emit canCreateBookmarkChanged( cantCreateBookmark );
        }
    }

    emit hasBookmarksChanged( hasViewWithBookmarks );
}

KHECore::Bookmark BookmarksTool::createBookmark()
{
    KHECore::Bookmark bookmark;

    if( mBookmarks )
    {
        const int cursorPosition = mByteArrayDisplay->cursorPosition();

        // search for text at cursor
        const KHECore::CharCodec* charCodec = KHECore::CharCodec::createCodec( mByteArrayDisplay->charCodingName() );
        const KHECore::WordByteArrayService textService( mByteArray, charCodec );
        QString bookmarkName = textService.text( cursorPosition, cursorPosition+MaxBookmarkNameSize-1 );
        delete charCodec;

        if( bookmarkName.isEmpty() )
            bookmarkName = i18nc( "default name of a bookmark", "Bookmark" );
        // %1").arg( 0 ) ); // TODO: use counter like with new file, globally

        bookmark.setOffset( mByteArrayDisplay->cursorPosition() );
        bookmark.setName( bookmarkName );

        QList<KHECore::Bookmark> bookmarksToBeCreated;
        bookmarksToBeCreated.append( bookmark );
        mBookmarks->addBookmarks( bookmarksToBeCreated );
    }

    return bookmark;
}

void BookmarksTool::deleteBookmarks( const QList<KHECore::Bookmark>& bookmarks )
{
    if( mBookmarks )
        mBookmarks->removeBookmarks( bookmarks );
    mByteArrayDisplay->widget()->setFocus();
}

void BookmarksTool::gotoBookmark( const KHECore::Bookmark& bookmark )
{
    if( mByteArrayDisplay )
    {
        mByteArrayDisplay->setCursorPosition( bookmark.offset() );
        mByteArrayDisplay->widget()->setFocus();
    }
}

void BookmarksTool::setBookmarkName( const QString& name, int bookmarkIndex )
{
    // TODO: this is a very ugly hack!
    // Fix this by turning Bookmarkable into a list API, so the change of one bookmark is a one op call
    KHECore::Bookmark bookmark = mBookmarks->bookmarkList().list().at( bookmarkIndex );

    QList<KHECore::Bookmark> bookmarks;
    bookmarks.append( bookmark );
    mBookmarks->removeBookmarks( bookmarks );

    bookmark.setName( name );
    bookmarks.clear();
    bookmarks.append( bookmark );
    mBookmarks->addBookmarks( bookmarks );

    mByteArrayDisplay->widget()->setFocus();
}

void BookmarksTool::onCursorPositionChanged( int newPosition )
{
    const KHECore::BookmarkList bookmarkList = mBookmarks->bookmarkList();
    const int bookmarksCount = bookmarkList.size();
    const bool hasBookmarks = ( bookmarksCount != 0 );
    const bool isInsideByteArray = ( newPosition < mByteArray->size() );
    const bool isAtBookmark = hasBookmarks ? bookmarkList.contains( newPosition ) : false;
    const bool canCreateBookmark = ( !isAtBookmark && isInsideByteArray );

    if( canCreateBookmark != mCanCreateBookmark )
    {
        mCanCreateBookmark = canCreateBookmark;
        emit canCreateBookmarkChanged( canCreateBookmark );
    }
}

// TODO: is a hack
// better just only check for the added and removed, if they include the current position, then change mCanCreateBookmark
void BookmarksTool::onBookmarksModified()
{
    const int cursorPosition = mByteArrayDisplay->cursorPosition();
    onCursorPositionChanged( cursorPosition );
}

BookmarksTool::~BookmarksTool() {}
