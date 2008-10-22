/*
    This file is part of the Okteta Kakao module, part of the KDE project.

    Copyright 2007-2008 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "bookmarkscontroller.h"

// lib
#include <kbytearraydisplay.h>
#include <kbytearraydocument.h>
// Kakao core
#include <abstractmodel.h>
// Okteta gui
#include <koffsetformat.h>
#include <kbytearrayview.h>
// Okteta core
#include <kbookmarkable.h>
#include <kbytearraymodel.h>
// KDE
#include <KXMLGUIClient>
#include <KLocale>
#include <KAction>
#include <KActionCollection>
#include <KStandardAction>
// Qt
#include <QtGui/QAction>


static const int MaxEntryLength = 150;
static const char BookmarkListActionListId[] = "bookmark_list";

// TODO: Sortieren nach Offset oder Zeit

BookmarksController::BookmarksController( KXMLGUIClient* guiClient )
 : mGuiClient( guiClient ), mByteArrayView( 0 ), mByteArray( 0 ), mBookmarks( 0 )
{
    KActionCollection* actionCollection = mGuiClient->actionCollection();

    mCreateAction = KStandardAction::addBookmark( this, SLOT(createBookmark()), actionCollection );

    mDeleteAction = actionCollection->addAction( "bookmark_remove" );
    mDeleteAction->setText( i18nc("@action:inmenu","Remove Bookmark") );
    connect( mDeleteAction, SIGNAL(triggered(bool) ), SLOT(deleteBookmark()) );
    mDeleteAction->setShortcut( Qt::CTRL + Qt::SHIFT + Qt::Key_B );

    mDeleteAllAction = actionCollection->addAction( "bookmark_remove_all" );
    mDeleteAllAction->setText( i18nc("@action:inmenu","Remove All Bookmarks") );
    connect( mDeleteAllAction, SIGNAL(triggered(bool) ), SLOT(deleteAllBookmarks()) );
//     mDeleteAllAction->setShortcut( Qt::CTRL + Qt::Key_G );

    mGotoNextBookmarkAction = actionCollection->addAction( "bookmark_next" );
    mGotoNextBookmarkAction->setText( i18nc("@action:inmenu","Goto Next Bookmark") );
    connect( mGotoNextBookmarkAction, SIGNAL(triggered(bool) ), SLOT(gotoNextBookmark()) );
    mGotoNextBookmarkAction->setShortcut( Qt::ALT + Qt::Key_Down );

    mGotoPreviousBookmarkAction = actionCollection->addAction( "bookmark_previous" );
    mGotoPreviousBookmarkAction->setText( i18nc("@action:inmenu","Goto Previous Bookmark") );
    connect( mGotoPreviousBookmarkAction, SIGNAL(triggered(bool) ), SLOT(gotoPreviousBookmark()) );
    mGotoPreviousBookmarkAction->setShortcut( Qt::ALT + Qt::Key_Up );

    mBookmarksActionGroup = new QActionGroup( this ); // TODO: do we use this only for the signal mapping?
//     mBookmarksActionGroup->setExclusive( true );
    connect( mBookmarksActionGroup, SIGNAL(triggered( QAction* )), SLOT(onBookmarkTriggered( QAction* )) );

    setTargetModel( 0 );
}

void BookmarksController::setTargetModel( AbstractModel* model )
{
    if( mByteArrayView ) mByteArrayView->disconnect( this );
    if( mByteArray ) mByteArray->disconnect( this );

    KByteArrayDisplay* view = model ? model->findBaseModel<KByteArrayDisplay*>() : 0;
    mByteArrayView = view ? qobject_cast<KHEUI::KByteArrayView *>( view->widget() ) : 0;

    KByteArrayDocument *document = view ? qobject_cast<KByteArrayDocument*>( view->baseModel() ) : 0;
    mByteArray = document ? document->content() : 0;
    mBookmarks = ( mByteArray && mByteArrayView ) ? qobject_cast<KHECore::Bookmarkable*>( mByteArray ) : 0;

    int bookmarksCount = 0;
    if( mBookmarks )
    {
        bookmarksCount = mBookmarks->bookmarkList().size();
        connect( mByteArray, SIGNAL(bookmarksAdded( const QList<KHECore::KBookmark>& )),
                 SLOT(onBookmarksAdded( const QList<KHECore::KBookmark>& )) );
        connect( mByteArray, SIGNAL(bookmarksRemoved( const QList<KHECore::KBookmark>& )),
                 SLOT(onBookmarksRemoved( const QList<KHECore::KBookmark>& )) );
        connect( mByteArray, SIGNAL(bookmarksModified( bool )),
                 SLOT(updateBookmarks()) );
        connect( mByteArrayView, SIGNAL( cursorPositionChanged( int )), SLOT( onCursorPositionChanged( int )) );
    }

    updateBookmarks();

    const bool hasViewWithBookmarks = ( mBookmarks != 0 );
    const bool hasBookmarks = hasViewWithBookmarks && ( bookmarksCount != 0 );
    if( hasViewWithBookmarks )
        onCursorPositionChanged( mByteArrayView->cursorPosition() );
    else
    {
        mCreateAction->setEnabled( false );
        mDeleteAction->setEnabled( false );
    }
    mDeleteAllAction->setEnabled( hasBookmarks );
    mGotoNextBookmarkAction->setEnabled( hasBookmarks );
    mGotoPreviousBookmarkAction->setEnabled( hasBookmarks );
}

void BookmarksController::updateBookmarks()
{
    mGuiClient->unplugActionList( BookmarkListActionListId );

    qDeleteAll( mBookmarksActionGroup->actions() );

    if( mBookmarks == 0 )
        return;

    const KHECore::KBookmarkList bookmarks = mBookmarks->bookmarkList();

    const int startOffset = mByteArrayView->startOffset();
    KHEUI::KOffsetFormat::print printFunction = KHEUI::KOffsetFormat::printFunction( KHEUI::KOffsetFormat::Hexadecimal );

    char codedOffset[KHEUI::KOffsetFormat::MaxFormatWidth+1];
    KHECore::KBookmarkList::ConstIterator bit = bookmarks.begin();
    static const int firstWithNumericShortCut = 1;
    static const int lastWithNumericShortCut = 9;
    int b = firstWithNumericShortCut;
    for( ; bit != bookmarks.end(); ++b,++bit )
    {
        KHECore::KBookmark bookmark = *bit;
        printFunction( codedOffset, startOffset+bookmark.offset() );
        QString title = i18nc( "@item description of bookmark", "Offset: %1", QLatin1String(codedOffset) );
        if( b <= lastWithNumericShortCut )
            title = QString::fromLatin1("&%1 %2").arg( b ).arg( title );
        // = KStringHandler::rsqueeze( view->title(), MaxEntryLength );
        QAction *action = new QAction( title, mBookmarksActionGroup );

        action->setData( bookmark.offset() );
        mBookmarksActionGroup->addAction( action );
    }
    mGuiClient->plugActionList( BookmarkListActionListId, mBookmarksActionGroup->actions() );
}

void BookmarksController::onBookmarksAdded( const QList<KHECore::KBookmark> &bookmarks )
{
Q_UNUSED( bookmarks )
    onCursorPositionChanged( mByteArrayView->cursorPosition() );

    const int bookmarksCount = mBookmarks->bookmarkList().size();
    const bool hasBookmarks = ( bookmarksCount != 0 );
    mDeleteAllAction->setEnabled( hasBookmarks );
    mGotoNextBookmarkAction->setEnabled( hasBookmarks );
    mGotoPreviousBookmarkAction->setEnabled( hasBookmarks );

    updateBookmarks();
}

void BookmarksController::onBookmarksRemoved( const QList<KHECore::KBookmark> &bookmarks )
{
Q_UNUSED( bookmarks )
    onCursorPositionChanged( mByteArrayView->cursorPosition() );

    const int bookmarksCount = mBookmarks->bookmarkList().size();
    const bool hasBookmarks = ( bookmarksCount != 0 );
    mDeleteAllAction->setEnabled( hasBookmarks );
    mGotoNextBookmarkAction->setEnabled( hasBookmarks );
    mGotoPreviousBookmarkAction->setEnabled( hasBookmarks );

    updateBookmarks();
}

void BookmarksController::onCursorPositionChanged( int newPosition )
{
    const bool isInsideByteArray = ( newPosition < mByteArray->size() );
    const bool hasBookmark = mBookmarks->bookmarkList().contains( newPosition );
    mCreateAction->setEnabled( !hasBookmark && isInsideByteArray );
    mDeleteAction->setEnabled( hasBookmark );
}

void BookmarksController::createBookmark()
{
    const int cursorPosition = mByteArrayView->cursorPosition();
    QList<KHECore::KBookmark> bookmarks;
    bookmarks.append( cursorPosition );
    mBookmarks->addBookmarks( bookmarks ); // TODO: popup for name tag
}

void BookmarksController::deleteBookmark()
{
    const int cursorPosition = mByteArrayView->cursorPosition();
    QList<KHECore::KBookmark> bookmarks;
    bookmarks.append( cursorPosition );
    mBookmarks->removeBookmarks( bookmarks );
}

void BookmarksController::deleteAllBookmarks()
{
    mBookmarks->removeAllBookmarks();
}

void BookmarksController::gotoNextBookmark()
{
    const int currentPosition = mByteArrayView->cursorPosition();
    const KHECore::KBookmarkList bookmarkList = mBookmarks->bookmarkList();
    KHECore::KBookmarkList::ConstIterator nit = bookmarkList.nextFrom( currentPosition+1 );
    if( nit != bookmarkList.end() )
    {
        const int newPosition = nit->offset();
        mByteArrayView->setCursorPosition( newPosition );
    }
}

void BookmarksController::gotoPreviousBookmark()
{
    const int currentPosition = mByteArrayView->cursorPosition();
    const KHECore::KBookmarkList bookmarkList = mBookmarks->bookmarkList();
    KHECore::KBookmarkList::ConstIterator pit = bookmarkList.previousFrom( currentPosition-1 );
    if( pit != bookmarkList.end() )
    {
        const int newPosition = pit->offset();
        mByteArrayView->setCursorPosition( newPosition );
    }
}

void BookmarksController::onBookmarkTriggered( QAction* action )
{
    const int newPosition = action->data().toInt();
    mByteArrayView->setCursorPosition( newPosition );
}


BookmarksController::~BookmarksController() {}
