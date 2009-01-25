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
#include <kbookmarkable.h>
#include <kbytearraymodel.h>
// KDE
#include <KLocale>
// Qt
#include <QtGui/QWidget>


BookmarksTool::BookmarksTool()
 : mByteArrayDisplay( 0 ), mByteArray( 0 ), mBookmarks( 0 )
{
    setObjectName( "Bookmarks" );
}


QString BookmarksTool::title() const { return i18nc("@title:window", "Bookmarks"); }

bool BookmarksTool::hasModel() const { return ( mBookmarks != 0 ); }
int BookmarksTool::cursorPosition() const { return mByteArrayDisplay ? mByteArrayDisplay->cursorPosition() : 0; }
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
        connect( mByteArray, SIGNAL(bookmarksAdded( const QList<KHECore::Bookmark>& )),
                 SIGNAL(bookmarksAdded( const QList<KHECore::Bookmark>& )) );
        connect( mByteArray, SIGNAL(bookmarksRemoved( const QList<KHECore::Bookmark>& )),
                 SIGNAL(bookmarksRemoved( const QList<KHECore::Bookmark>& )) );
        connect( mByteArray, SIGNAL(bookmarksModified( bool )),
                 SIGNAL(bookmarksModified( bool )) );
        connect( mByteArrayDisplay, SIGNAL(cursorPositionChanged( int )), SIGNAL(cursorPositionChanged( int )) );
    }

    emit modelChanged( hasViewWithBookmarks );
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
}


BookmarksTool::~BookmarksTool() {}
