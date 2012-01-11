/*
    This file is part of the Kasten Framework, made within the KDE community.

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

// Kasten core
#include <bytearrayview.h>
#include <bytearraydocument.h>
// Kasten core
#include <abstractmodel.h>
// Okteta gui
#include <offsetformat.h>
// Okteta core
#include <wordbytearrayservice.h>
#include <charcodec.h>
#include <bookmarkable.h>
#include <bookmarksconstiterator.h>
#include <bookmark.h>
#include <abstractbytearraymodel.h>
// KDE
#include <KLocale>
// Qt
#include <QtGui/QWidget>


namespace Kasten2
{

BookmarksTool::BookmarksTool()
 : mByteArrayView( 0 ), mByteArray( 0 ), mBookmarks( 0 ), mCanCreateBookmark( false )
{
    setObjectName( QLatin1String( "Bookmarks" ) );
}


QString BookmarksTool::title() const { return i18nc("@title:window", "Bookmarks"); }
bool BookmarksTool::canCreateBookmark() const { return mCanCreateBookmark; }
const Okteta::Bookmark& BookmarksTool::bookmarkAt( unsigned int index ) const { return mBookmarks->bookmarkAt( index ); }
int BookmarksTool::indexOf( const Okteta::Bookmark& bookmark ) const
{
    int result = -1;

    Okteta::BookmarksConstIterator bit = mBookmarks->createBookmarksConstIterator();
    int i = 0;
    while( bit.hasNext() )
    {
        if( bookmark == bit.next() )
        {
            result = i;
            break;
        }
        ++i;
    }
    return result;
}
unsigned int BookmarksTool::bookmarksCount() const { return mBookmarks ? mBookmarks->bookmarksCount() : 0; }


void BookmarksTool::setTargetModel( AbstractModel* model )
{
    if( mByteArrayView ) mByteArrayView->disconnect( this );
    if( mByteArray ) mByteArray->disconnect( this );

    mByteArrayView = model ? model->findBaseModel<ByteArrayView*>() : 0;

    ByteArrayDocument* document =
        mByteArrayView ? qobject_cast<ByteArrayDocument*>( mByteArrayView->baseModel() ) : 0;
    mByteArray = document ? document->content() : 0;
    mBookmarks = ( mByteArray && mByteArrayView ) ? qobject_cast<Okteta::Bookmarkable*>( mByteArray ) : 0;

    const bool hasViewWithBookmarks = ( mBookmarks != 0 );
    if( hasViewWithBookmarks )
    {
        onCursorPositionChanged( mByteArrayView->cursorPosition() );

        connect( mByteArray, SIGNAL(bookmarksAdded(QList<Okteta::Bookmark>)),
                 SIGNAL(bookmarksAdded(QList<Okteta::Bookmark>)) );
        connect( mByteArray, SIGNAL(bookmarksRemoved(QList<Okteta::Bookmark>)),
                 SIGNAL(bookmarksRemoved(QList<Okteta::Bookmark>)) );
        connect( mByteArray, SIGNAL(bookmarksAdded(QList<Okteta::Bookmark>)),
                 SLOT(onBookmarksModified()) );
        connect( mByteArray, SIGNAL(bookmarksRemoved(QList<Okteta::Bookmark>)),
                 SLOT(onBookmarksModified()) );
        connect( mByteArray, SIGNAL(bookmarksModified(QList<int>)),
                 SIGNAL(bookmarksModified(QList<int>)) );
        connect( mByteArrayView, SIGNAL(cursorPositionChanged(Okteta::Address)), SLOT(onCursorPositionChanged(Okteta::Address)) );
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

Okteta::Bookmark BookmarksTool::createBookmark()
{
    Okteta::Bookmark bookmark;

    if( mBookmarks )
    {
        const int cursorPosition = mByteArrayView->cursorPosition();

        // search for text at cursor
        const Okteta::CharCodec* charCodec = Okteta::CharCodec::createCodec( mByteArrayView->charCodingName() );
        const Okteta::WordByteArrayService textService( mByteArray, charCodec );
        QString bookmarkName = textService.text( cursorPosition, cursorPosition+MaxBookmarkNameSize-1 );
        delete charCodec;

        if( bookmarkName.isEmpty() )
            bookmarkName = i18nc( "default name of a bookmark", "Bookmark" );
        // %1").arg( 0 ) ); // TODO: use counter like with new file, globally

        bookmark.setOffset( mByteArrayView->cursorPosition() );
        bookmark.setName( bookmarkName );

        QList<Okteta::Bookmark> bookmarksToBeCreated;
        bookmarksToBeCreated.append( bookmark );
        mBookmarks->addBookmarks( bookmarksToBeCreated );
    }

    return bookmark;
}

void BookmarksTool::deleteBookmarks( const QList<Okteta::Bookmark>& bookmarks )
{
    if( mBookmarks )
        mBookmarks->removeBookmarks( bookmarks );
    mByteArrayView->widget()->setFocus();
}

void BookmarksTool::gotoBookmark( const Okteta::Bookmark& bookmark )
{
    if( mByteArrayView )
    {
        mByteArrayView->setCursorPosition( bookmark.offset() );
        mByteArrayView->widget()->setFocus();
    }
}

void BookmarksTool::setBookmarkName( unsigned int bookmarkIndex, const QString& name )
{
    Okteta::Bookmark bookmark = mBookmarks->bookmarkAt( bookmarkIndex );

    bookmark.setName( name );
    mBookmarks->setBookmark( bookmarkIndex, bookmark );

    mByteArrayView->widget()->setFocus();
}

void BookmarksTool::onCursorPositionChanged( Okteta::Address newPosition )
{
    const int bookmarksCount = mBookmarks->bookmarksCount();
    const bool hasBookmarks = ( bookmarksCount != 0 );
    const bool isInsideByteArray = ( newPosition < mByteArray->size() );
    const bool isAtBookmark = hasBookmarks ? mBookmarks->containsBookmarkFor( newPosition ) : false;
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
    const int cursorPosition = mByteArrayView->cursorPosition();
    onCursorPositionChanged( cursorPosition );
}

BookmarksTool::~BookmarksTool() {}

}
