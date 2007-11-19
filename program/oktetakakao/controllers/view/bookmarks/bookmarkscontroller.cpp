/***************************************************************************
                          bookmarkscontroller.cpp  -  description
                             -------------------
    begin                : Thu Nov 1 2007
    copyright            : 2007 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU General Public License           *
 *   version 2 as published by the Free Software Foundation.               *
 *                                                                         *
 ***************************************************************************/


#include "bookmarkscontroller.h"

// Okteta kakao
#include <kbytearraydocument.h>
// kakao
#include <kviewmanager.h>
// Okteta gui
#include <koffsetformat.h>
#include <kbytearrayview.h>
// Okteta core
#include <kbytearraymodel.h>
// KDE
#include <KXmlGuiWindow>
#include <KLocale>
#include <KAction>
#include <KActionCollection>
#include <KStandardAction>
// Qt
#include <QtGui/QAction>

static const int MaxEntryLength = 150;
static const char BookmarkListActionListId[] = "bookmark_list";

// TODO: Sortieren nach Offset oder Zeit

BookmarksController::BookmarksController( KXmlGuiWindow *window )
 : mWindow( window ), mByteArrayView( 0 ), mByteArray( 0 )
{
    KActionCollection *actionCollection = mWindow->actionCollection();

    mCreateAction = KStandardAction::addBookmark( this, SLOT(createBookmark()), actionCollection );

    mDeleteAction = actionCollection->addAction( "bookmark_remove" );
    mDeleteAction->setText( i18n("Remove Bookmark") );
    connect( mDeleteAction, SIGNAL(triggered(bool) ), SLOT(deleteBookmark()) );
    mDeleteAction->setShortcut( Qt::CTRL + Qt::SHIFT + Qt::Key_B );

    mDeleteAllAction = actionCollection->addAction( "bookmark_remove_all" );
    mDeleteAllAction->setText( i18n("Remove All Bookmarks") );
    connect( mDeleteAllAction, SIGNAL(triggered(bool) ), SLOT(deleteAllBookmarks()) );
//     mDeleteAllAction->setShortcut( Qt::CTRL + Qt::Key_G );

    mGotoNextBookmarkAction = actionCollection->addAction( "bookmark_next" );
    mGotoNextBookmarkAction->setText( i18n("Goto Next Bookmark") );
    connect( mGotoNextBookmarkAction, SIGNAL(triggered(bool) ), SLOT(gotoNextBookmark()) );
    mGotoNextBookmarkAction->setShortcut( Qt::ALT + Qt::Key_Down );

    mGotoPreviousBookmarkAction = actionCollection->addAction( "bookmark_previous" );
    mGotoPreviousBookmarkAction->setText( i18n("Goto Previous Bookmark") );
    connect( mGotoPreviousBookmarkAction, SIGNAL(triggered(bool) ), SLOT(gotoPreviousBookmark()) );
    mGotoPreviousBookmarkAction->setShortcut( Qt::ALT + Qt::Key_Up );

    mBookmarksActionGroup = new QActionGroup( this ); // TODO: do we use this only for the signal mapping?
//     mBookmarksActionGroup->setExclusive( true );
    connect( mBookmarksActionGroup, SIGNAL(triggered( QAction* )), SLOT(onBookmarkTriggered( QAction* )) );

    setView( 0 );
}

void BookmarksController::setView( KAbstractView *view )
{
    disconnect( mByteArrayView );

    mByteArrayView = view ? static_cast<KHEUI::KByteArrayView *>( view->widget() ) : 0;
    KByteArrayDocument *document = view ? static_cast<KByteArrayDocument*>( view->document() ) : 0;
    mByteArray = document ? document->content() : 0;

    int bookmarksCount = 0;
    if( mByteArray )
    {
//         bookmarksCount = mByteArray->bookmarkList()->size();
//         connect( mByteArray->bookmarkList(), SIGNAL(bookmarksAdded( const QList<KHECore::KBookmark*>& )),
//                     SLOT(onBookmarksAdded( const QList<KHECore::KBookmark*>& )) );
//         connect( mByteArray->bookmarkList(), SIGNAL(bookmarksRemoved( const QList<KHECore::KBookmark*>& )),
//                     SLOT(onBookmarksRemoved( const QList<KHECore::KBookmark*>& )) );
        connect( mByteArrayView, SIGNAL( cursorPositionChanged( int )), SLOT( onCursorPositionChanged( int )) );
    }
    const bool hasView = ( mByteArray != 0 );
    const bool hasBookmarks = hasView && ( bookmarksCount != 0 );
    mCreateAction->setEnabled( hasView );
    mDeleteAction->setEnabled( hasBookmarks ); //TODO: current is bookmark?
    mDeleteAllAction->setEnabled( hasBookmarks );
    mGotoNextBookmarkAction->setEnabled( hasBookmarks );
    mGotoPreviousBookmarkAction->setEnabled( hasBookmarks );

    updateBookmarks();
}

void BookmarksController::updateBookmarks()
{
    mWindow->unplugActionList( BookmarkListActionListId );

    qDeleteAll( mBookmarksActionGroup->actions() );

    if( mByteArray == 0 )
        return;

    const QList<KHECore::KBookmark*> bookmarks;// = mByteArray->bookmarks();

    const int startOffset = mByteArrayView->startOffset();
    KHEUI::KOffsetFormat::print printFunction = KHEUI::KOffsetFormat::printFunction( KHEUI::KOffsetFormat::Hexadecimal );

    char codedOffset[KHEUI::KOffsetFormat::MaxFormatWidth+1];
    for( int b = 0; b < bookmarks.size(); ++b )
    {
//         KHECore::KBookmark *bookmark = bookmarks.at( b );
        printFunction( codedOffset, startOffset);//+bookmark->position() );
        const QString title = i18n( "Offset: %1", QLatin1String(codedOffset) );
            // = KStringHandler::rsqueeze( view->title(), MaxEntryLength );
        QAction *action = new QAction( b<10 ? QString::fromLatin1("&%1 %2").arg(b).arg(title) : title, mBookmarksActionGroup );

//         action->setData( bookmark->position() );
        mBookmarksActionGroup->addAction( action );
    }
    mWindow->plugActionList( BookmarkListActionListId, mBookmarksActionGroup->actions() );
}

void BookmarksController::onBookmarksAdded( const QList<KHECore::KBookmark*> &bookmarks )
{
Q_UNUSED( bookmarks )
    const int cursorPosition = mByteArrayView->cursorPosition();
    const bool hasBookmark = false;//mByteArray->bookmarkList()->has( newPosition );
    mDeleteAction->setEnabled( hasBookmark );

    int bookmarksCount = 0;
//         bookmarksCount = mByteArray->bookmarkList()->size();
    const bool hasBookmarks = ( bookmarksCount != 0 );
    mDeleteAllAction->setEnabled( hasBookmarks );
    mGotoNextBookmarkAction->setEnabled( hasBookmarks );
    mGotoPreviousBookmarkAction->setEnabled( hasBookmarks );

    updateBookmarks();
}

void BookmarksController::onBookmarksRemoved( const QList<KHECore::KBookmark*> &bookmarks )
{
Q_UNUSED( bookmarks )
    const int cursorPosition = mByteArrayView->cursorPosition();
    const bool hasBookmark = false;//mByteArray->bookmarkList()->has( newPosition );
    mDeleteAction->setEnabled( hasBookmark );

    int bookmarksCount = 0;
//         bookmarksCount = mByteArray->bookmarkList()->size();
    const bool hasBookmarks = ( bookmarksCount != 0 );
    mDeleteAllAction->setEnabled( hasBookmarks );
    mGotoNextBookmarkAction->setEnabled( hasBookmarks );
    mGotoPreviousBookmarkAction->setEnabled( hasBookmarks );

    updateBookmarks();
}

void BookmarksController::onCursorPositionChanged( int newPosition )
{
    const bool hasBookmark = false;//mByteArray->bookmarkList()->has( newPosition );
    mDeleteAction->setEnabled( hasBookmark );
}

void BookmarksController::createBookmark()
{
    const int cursorPosition = mByteArrayView->cursorPosition();
//     mByteArray->bookmarkList()->addBookmark( cursorPosition ); // TODO: popup for name tag
}

void BookmarksController::deleteBookmark()
{
    const int cursorPosition = mByteArrayView->cursorPosition();
//     mByteArray->bookmarkList()->removeBookmark( cursorPosition );
}

void BookmarksController::deleteAllBookmarks()
{
//     mByteArray->bookmarkList()->removeAllBookmark();
}

void BookmarksController::gotoNextBookmark()
{
    const int currentPosition = mByteArrayView->cursorPosition();
    const int newPosition = 0;// mByteArray->bookmarkList()->nextFrom( currentPosition ).position();
    mByteArrayView->setCursorPosition( newPosition );
}

void BookmarksController::gotoPreviousBookmark()
{
    const int currentPosition = mByteArrayView->cursorPosition();
    const int newPosition = 0;// mByteArray->bookmarkList()->previousFrom( currentPosition ).position();
    mByteArrayView->setCursorPosition( newPosition );
}

void BookmarksController::onBookmarkTriggered( QAction* action )
{
    const int newPosition = action->data().toInt();
    mByteArrayView->setCursorPosition( newPosition );
}


BookmarksController::~BookmarksController()
{
}
