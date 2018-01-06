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
#include <kasten/okteta/bytearrayview.h>
#include <kasten/okteta/bytearraydocument.h>
// Kasten core
#include <kasten/abstractmodel.h>
// Okteta gui
#include <okteta/offsetformat.h>
// Okteta core
#include <okteta/wordbytearrayservice.h>
#include <okteta/charcodec.h>
#include <okteta/bookmarkable.h>
#include <okteta/bookmarksconstiterator.h>
#include <okteta/bookmark.h>
#include <okteta/abstractbytearraymodel.h>
// KF5
#include <KLocalizedString>
// Qt
#include <QWidget>


namespace Kasten
{

BookmarksTool::BookmarksTool()
 : mByteArrayView( nullptr ), mByteArray( nullptr ), mBookmarks( nullptr ), mCanCreateBookmark( false )
{
    setObjectName( QStringLiteral( "Bookmarks" ) );
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
int BookmarksTool::offsetCoding()            const { return mByteArrayView ? mByteArrayView->offsetCoding() : 0; }


void BookmarksTool::setTargetModel( AbstractModel* model )
{
    const int oldOffsetCoding = offsetCoding();

    if( mByteArrayView ) mByteArrayView->disconnect( this );
    if( mByteArray ) mByteArray->disconnect( this );

    mByteArrayView = model ? model->findBaseModel<ByteArrayView*>() : nullptr;

    ByteArrayDocument* document =
        mByteArrayView ? qobject_cast<ByteArrayDocument*>( mByteArrayView->baseModel() ) : nullptr;
    mByteArray = document ? document->content() : nullptr;
    mBookmarks = ( mByteArray && mByteArrayView ) ? qobject_cast<Okteta::Bookmarkable*>( mByteArray ) : nullptr;

    const bool hasViewWithBookmarks = ( mBookmarks != nullptr );
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
        connect( mByteArrayView, &ByteArrayView::cursorPositionChanged,
                 this, &BookmarksTool::onCursorPositionChanged );
        connect( mByteArrayView, &ByteArrayView::offsetCodingChanged,
                 this, &BookmarksTool::offsetCodingChanged );
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

    const int newOffsetCoding = offsetCoding();
    if( oldOffsetCoding != newOffsetCoding )
        emit offsetCodingChanged( newOffsetCoding );
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

        const QList<Okteta::Bookmark> bookmarksToBeCreated { bookmark };
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
