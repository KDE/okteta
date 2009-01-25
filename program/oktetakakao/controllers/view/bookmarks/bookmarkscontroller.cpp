/*
    This file is part of the Okteta Kakao module, part of the KDE project.

    Copyright 2007-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

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

// controller
#include "bookmarkeditpopup.h"
// lib
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
#include <abstractbytearraymodel.h>
// KDE
#include <KXMLGUIClient>
#include <KLocale>
#include <KAction>
#include <KActionCollection>
#include <KStandardAction>
// Qt
#include <QtGui/QAction>


static const int MaxEntryLength = 150;
static const int MaxBookmarkNameSize = 40;
static const char BookmarkListActionListId[] = "bookmark_list";

// TODO: Sortieren nach Offset oder Zeit

BookmarksController::BookmarksController( KXMLGUIClient* guiClient )
 : mGuiClient( guiClient ), mByteArrayDisplay( 0 ), mByteArray( 0 ), mBookmarks( 0 )
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
    if( mByteArrayDisplay ) mByteArrayDisplay->disconnect( this );
    if( mByteArray ) mByteArray->disconnect( this );

    mByteArrayDisplay = model ? model->findBaseModel<KByteArrayDisplay*>() : 0;

    KByteArrayDocument* document =
        mByteArrayDisplay ? qobject_cast<KByteArrayDocument*>( mByteArrayDisplay->baseModel() ) : 0;
    mByteArray = document ? document->content() : 0;
    mBookmarks = ( mByteArray && mByteArrayDisplay ) ? qobject_cast<KHECore::Bookmarkable*>( mByteArray ) : 0;

    const bool hasViewWithBookmarks = ( mBookmarks != 0 );
    int bookmarksCount = 0;
    if( hasViewWithBookmarks )
    {
        bookmarksCount = mBookmarks->bookmarkList().size();
        connect( mByteArray, SIGNAL(bookmarksAdded( const QList<KHECore::Bookmark>& )),
                 SLOT(onBookmarksAdded( const QList<KHECore::Bookmark>& )) );
        connect( mByteArray, SIGNAL(bookmarksRemoved( const QList<KHECore::Bookmark>& )),
                 SLOT(onBookmarksRemoved( const QList<KHECore::Bookmark>& )) );
        connect( mByteArray, SIGNAL(bookmarksModified( bool )),
                 SLOT(updateBookmarks()) );
        connect( mByteArrayDisplay, SIGNAL( cursorPositionChanged( int )), SLOT( onCursorPositionChanged( int )) );
    }

    updateBookmarks();

    const bool hasBookmarks = hasViewWithBookmarks && ( bookmarksCount != 0 );
    if( hasViewWithBookmarks )
        onCursorPositionChanged( mByteArrayDisplay->cursorPosition() );
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

    const KHECore::BookmarkList bookmarks = mBookmarks->bookmarkList();

    const int startOffset = mByteArrayDisplay->startOffset();
    KHEUI::KOffsetFormat::print printFunction = KHEUI::KOffsetFormat::printFunction( KHEUI::KOffsetFormat::Hexadecimal );

    char codedOffset[KHEUI::KOffsetFormat::MaxFormatWidth+1];
    KHECore::BookmarkList::ConstIterator bit = bookmarks.begin();
    static const int firstWithNumericShortCut = 1;
    static const int lastWithNumericShortCut = 9;
    int b = firstWithNumericShortCut;
    for( ; bit != bookmarks.end(); ++b,++bit )
    {
        KHECore::Bookmark bookmark = *bit;
        printFunction( codedOffset, startOffset+bookmark.offset() );
        QString title = i18nc( "@item description of bookmark", "%1: %2", QLatin1String(codedOffset),bookmark.name() );
        if( b <= lastWithNumericShortCut )
            title = QString::fromLatin1("&%1 %2").arg( b ).arg( title );
        // = KStringHandler::rsqueeze( view->title(), MaxEntryLength );
        QAction *action = new QAction( title, mBookmarksActionGroup );

        action->setData( bookmark.offset() );
        mBookmarksActionGroup->addAction( action );
    }
    mGuiClient->plugActionList( BookmarkListActionListId, mBookmarksActionGroup->actions() );
}

void BookmarksController::onBookmarksAdded( const QList<KHECore::Bookmark> &bookmarks )
{
Q_UNUSED( bookmarks )
    const int currentPosition = mByteArrayDisplay->cursorPosition();
    onCursorPositionChanged( currentPosition );

    const KHECore::BookmarkList bookmarkList = mBookmarks->bookmarkList();
    const int bookmarksCount = bookmarkList.size();
    const bool hasBookmarks = ( bookmarksCount != 0 );

    mDeleteAllAction->setEnabled( hasBookmarks );

    updateBookmarks();
}

void BookmarksController::onBookmarksRemoved( const QList<KHECore::Bookmark> &bookmarks )
{
Q_UNUSED( bookmarks )
    const int currentPosition = mByteArrayDisplay->cursorPosition();
    onCursorPositionChanged( currentPosition );

    const KHECore::BookmarkList bookmarkList = mBookmarks->bookmarkList();
    const int bookmarksCount = bookmarkList.size();
    const bool hasBookmarks = ( bookmarksCount != 0 );

    mDeleteAllAction->setEnabled( hasBookmarks );

    updateBookmarks();
}

void BookmarksController::onCursorPositionChanged( int newPosition )
{
    const KHECore::BookmarkList bookmarkList = mBookmarks->bookmarkList();
    const int bookmarksCount = bookmarkList.size();
    const bool hasBookmarks = ( bookmarksCount != 0 );
    const bool isInsideByteArray = ( newPosition < mByteArray->size() );
    bool isAtBookmark;
    bool hasPrevious;
    bool hasNext;
    if( hasBookmarks )
    {
        isAtBookmark = bookmarkList.contains( newPosition );
        const KHECore::BookmarkList::ConstIterator end = bookmarkList.constEnd();
        const KHECore::BookmarkList::ConstIterator pit = bookmarkList.previousFrom( newPosition-1 );
        hasPrevious = ( pit != end );
        const KHECore::BookmarkList::ConstIterator nit = bookmarkList.nextFrom( newPosition+1 );
        hasNext = ( nit != end );
    }
    else
    {
        isAtBookmark = false;
        hasPrevious = false;
        hasNext = false;
    }

    mCreateAction->setEnabled( !isAtBookmark && isInsideByteArray );
    mDeleteAction->setEnabled( isAtBookmark );
    mGotoNextBookmarkAction->setEnabled( hasNext );
    mGotoPreviousBookmarkAction->setEnabled( hasPrevious );
}

void BookmarksController::createBookmark()
{
    const int cursorPosition = mByteArrayDisplay->cursorPosition();

    // search for text at cursor
    const KHECore::CharCodec* charCodec = KHECore::CharCodec::createCodec( mByteArrayDisplay->charCodingName() );
    const KHECore::WordByteArrayService textService( mByteArray, charCodec );
    QString bookmarkName = textService.text( cursorPosition, cursorPosition+MaxBookmarkNameSize-1 );
    delete charCodec;

    if( bookmarkName.isEmpty() )
        bookmarkName = i18nc( "default name of a bookmark", "Bookmark" );// %1").arg( 0 ) ); // TODO: use counter like with new file, globally

    BookmarkEditPopup* bookmarkEditPopup = new BookmarkEditPopup( mByteArrayDisplay->widget() );
    QPoint popupPoint = mByteArrayDisplay->cursorRect().topLeft();
//     popupPoint.ry() += mSlider->height() / 2;
    popupPoint = mByteArrayDisplay->widget()->mapToGlobal( popupPoint );

    bookmarkEditPopup->setPosition( popupPoint );
    bookmarkEditPopup->setName( bookmarkName );
    const bool success = ( bookmarkEditPopup->exec() != 0 );

    if( success )
    {
        KHECore::Bookmark bookmark( cursorPosition );
        bookmark.setName( bookmarkEditPopup->name() );

        QList<KHECore::Bookmark> bookmarks;
        bookmarks.append( bookmark );
        mBookmarks->addBookmarks( bookmarks );
    }
    delete bookmarkEditPopup;
}

void BookmarksController::deleteBookmark()
{
    const int cursorPosition = mByteArrayDisplay->cursorPosition();
    QList<KHECore::Bookmark> bookmarks;
    bookmarks.append( cursorPosition );
    mBookmarks->removeBookmarks( bookmarks );
}

void BookmarksController::deleteAllBookmarks()
{
    mBookmarks->removeAllBookmarks();
}

void BookmarksController::gotoNextBookmark()
{
    const int currentPosition = mByteArrayDisplay->cursorPosition();
    const KHECore::BookmarkList bookmarkList = mBookmarks->bookmarkList();
    KHECore::BookmarkList::ConstIterator nit = bookmarkList.nextFrom( currentPosition+1 );
    if( nit != bookmarkList.end() )
    {
        const int newPosition = nit->offset();
        mByteArrayDisplay->setCursorPosition( newPosition );
    }
}

void BookmarksController::gotoPreviousBookmark()
{
    const int currentPosition = mByteArrayDisplay->cursorPosition();
    const KHECore::BookmarkList bookmarkList = mBookmarks->bookmarkList();
    KHECore::BookmarkList::ConstIterator pit = bookmarkList.previousFrom( currentPosition-1 );
    if( pit != bookmarkList.end() )
    {
        const int newPosition = pit->offset();
        mByteArrayDisplay->setCursorPosition( newPosition );
    }
}

void BookmarksController::onBookmarkTriggered( QAction* action )
{
    const int newPosition = action->data().toInt();
    mByteArrayDisplay->setCursorPosition( newPosition );
}


BookmarksController::~BookmarksController() {}
