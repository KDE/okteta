/*
    This file is part of the Okteta Kasten module, part of the KDE project.

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
#include <KXMLGUIClient>
#include <KLocale>
#include <KAction>
#include <KActionCollection>
#include <KStandardAction>
// Qt
#include <QtGui/QAction>


namespace Kasten
{

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
    mGotoNextBookmarkAction->setText( i18nc("@action:inmenu","Go to Next Bookmark") );
    connect( mGotoNextBookmarkAction, SIGNAL(triggered(bool) ), SLOT(gotoNextBookmark()) );
    mGotoNextBookmarkAction->setShortcut( Qt::ALT + Qt::Key_Down );

    mGotoPreviousBookmarkAction = actionCollection->addAction( "bookmark_previous" );
    mGotoPreviousBookmarkAction->setText( i18nc("@action:inmenu","Go to Previous Bookmark") );
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

    mByteArrayDisplay = model ? model->findBaseModel<ByteArrayView*>() : 0;

    ByteArrayDocument* document =
        mByteArrayDisplay ? qobject_cast<ByteArrayDocument*>( mByteArrayDisplay->baseModel() ) : 0;
    mByteArray = document ? document->content() : 0;
    mBookmarks = ( mByteArray && mByteArrayDisplay ) ? qobject_cast<Okteta::Bookmarkable*>( mByteArray ) : 0;

    const bool hasViewWithBookmarks = ( mBookmarks != 0 );
    int bookmarksCount = 0;
    if( hasViewWithBookmarks )
    {
        bookmarksCount = mBookmarks->bookmarksCount();
        connect( mByteArray, SIGNAL(bookmarksAdded( const QList<Okteta::Bookmark>& )),
                 SLOT(onBookmarksAdded( const QList<Okteta::Bookmark>& )) );
        connect( mByteArray, SIGNAL(bookmarksRemoved( const QList<Okteta::Bookmark>& )),
                 SLOT(onBookmarksRemoved( const QList<Okteta::Bookmark>& )) );
        connect( mByteArray, SIGNAL(bookmarksModified( const QList<int>& )),
                 SLOT(updateBookmarks()) );
        connect( mByteArrayDisplay, SIGNAL( cursorPositionChanged( Okteta::Address )), SLOT( onCursorPositionChanged( Okteta::Address )) );
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

    const int startOffset = mByteArrayDisplay->startOffset();
    Okteta::OffsetFormat::print printFunction = Okteta::OffsetFormat::printFunction( Okteta::OffsetFormat::Hexadecimal );

    char codedOffset[Okteta::OffsetFormat::MaxFormatWidth+1];

    static const int firstWithNumericShortCut = 1;
    static const int lastWithNumericShortCut = 9;
    int b = firstWithNumericShortCut;

    Okteta::BookmarksConstIterator bit = mBookmarks->createBookmarksConstIterator();
    while( bit.hasNext() )
    {
        const Okteta::Bookmark& bookmark = bit.next();
        printFunction( codedOffset, startOffset+bookmark.offset() );
        QString title = i18nc( "@item description of bookmark", "%1: %2", QLatin1String(codedOffset),bookmark.name() );
        if( b <= lastWithNumericShortCut )
        {
            title = QString::fromLatin1("&%1 %2").arg( b ).arg( title );
        // = KStringHandler::rsqueeze( view->title(), MaxEntryLength );
            ++b;
        }
        QAction *action = new QAction( title, mBookmarksActionGroup );

        action->setData( bookmark.offset() );
        mBookmarksActionGroup->addAction( action );
    }
    mGuiClient->plugActionList( BookmarkListActionListId, mBookmarksActionGroup->actions() );
}

void BookmarksController::onBookmarksAdded( const QList<Okteta::Bookmark> &bookmarks )
{
Q_UNUSED( bookmarks )
    const int currentPosition = mByteArrayDisplay->cursorPosition();
    onCursorPositionChanged( currentPosition );

    const int bookmarksCount = mBookmarks->bookmarksCount();
    const bool hasBookmarks = ( bookmarksCount != 0 );

    mDeleteAllAction->setEnabled( hasBookmarks );

    updateBookmarks();
}

void BookmarksController::onBookmarksRemoved( const QList<Okteta::Bookmark> &bookmarks )
{
Q_UNUSED( bookmarks )
    const int currentPosition = mByteArrayDisplay->cursorPosition();
    onCursorPositionChanged( currentPosition );

    const int bookmarksCount = mBookmarks->bookmarksCount();
    const bool hasBookmarks = ( bookmarksCount != 0 );

    mDeleteAllAction->setEnabled( hasBookmarks );

    updateBookmarks();
}

void BookmarksController::onCursorPositionChanged( Okteta::Address newPosition )
{
    const int bookmarksCount = mBookmarks->bookmarksCount();
    const bool hasBookmarks = ( bookmarksCount != 0 );
    const bool isInsideByteArray = ( newPosition < mByteArray->size() );
    bool isAtBookmark;
    bool hasPrevious;
    bool hasNext;
    if( hasBookmarks )
    {
        isAtBookmark = mBookmarks->containsBookmarkFor( newPosition );
        Okteta::BookmarksConstIterator bookmarksIterator = mBookmarks->createBookmarksConstIterator();
        hasPrevious = bookmarksIterator.findPreviousFrom( newPosition );
        hasNext = bookmarksIterator.findNextFrom( newPosition );
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
    const Okteta::CharCodec* charCodec = Okteta::CharCodec::createCodec( mByteArrayDisplay->charCodingName() );
    const Okteta::WordByteArrayService textService( mByteArray, charCodec );
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
        Okteta::Bookmark bookmark( cursorPosition );
        bookmark.setName( bookmarkEditPopup->name() );

        QList<Okteta::Bookmark> bookmarks;
        bookmarks.append( bookmark );
        mBookmarks->addBookmarks( bookmarks );
    }
    delete bookmarkEditPopup;
}

void BookmarksController::deleteBookmark()
{
    const int cursorPosition = mByteArrayDisplay->cursorPosition();
    QList<Okteta::Bookmark> bookmarks;
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

    Okteta::BookmarksConstIterator bookmarksIterator = mBookmarks->createBookmarksConstIterator();
    const bool hasNext = bookmarksIterator.findNextFrom( currentPosition );
    if( hasNext )
    {
        const int newPosition = bookmarksIterator.next().offset();
        mByteArrayDisplay->setCursorPosition( newPosition );
    }
}

void BookmarksController::gotoPreviousBookmark()
{
    const int currentPosition = mByteArrayDisplay->cursorPosition();

    Okteta::BookmarksConstIterator bookmarksIterator = mBookmarks->createBookmarksConstIterator();
    const bool hasPrevious = bookmarksIterator.findPreviousFrom( currentPosition );
    if( hasPrevious )
    {
        const int newPosition = bookmarksIterator.previous().offset();
        mByteArrayDisplay->setCursorPosition( newPosition );
    }
}

void BookmarksController::onBookmarkTriggered( QAction* action )
{
    const int newPosition = action->data().toInt();
    mByteArrayDisplay->setCursorPosition( newPosition );
}


BookmarksController::~BookmarksController() {}

}
