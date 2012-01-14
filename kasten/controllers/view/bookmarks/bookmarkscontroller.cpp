/*
    This file is part of the Okteta Kasten module, made within the KDE community.

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


namespace Kasten2
{

static const char BookmarkListActionListId[] = "bookmark_list";

// TODO: Sortieren nach Offset oder Zeit

BookmarksController::BookmarksController( KXMLGUIClient* guiClient )
 : mGuiClient( guiClient ), mByteArrayView( 0 ), mByteArray( 0 ), mBookmarks( 0 )
{
    KActionCollection* actionCollection = mGuiClient->actionCollection();

    mCreateAction = KStandardAction::addBookmark( this, SLOT(createBookmark()), actionCollection );

    mDeleteAction = actionCollection->addAction( QLatin1String("bookmark_remove"),
                                                 this, SLOT(deleteBookmark()) );
    mDeleteAction->setText( i18nc("@action:inmenu","Remove Bookmark") );
    mDeleteAction->setShortcut( Qt::CTRL + Qt::SHIFT + Qt::Key_B );

    mDeleteAllAction = actionCollection->addAction( QLatin1String("bookmark_remove_all"),
                                                    this, SLOT(deleteAllBookmarks()) );
    mDeleteAllAction->setText( i18nc("@action:inmenu","Remove All Bookmarks") );
//     mDeleteAllAction->setShortcut( Qt::CTRL + Qt::Key_G );

    mGotoNextBookmarkAction = actionCollection->addAction( QLatin1String("bookmark_next"),
                                                           this, SLOT(gotoNextBookmark()) );
    mGotoNextBookmarkAction->setText( i18nc("@action:inmenu","Go to Next Bookmark") );
    mGotoNextBookmarkAction->setShortcut( Qt::ALT + Qt::Key_Down );

    mGotoPreviousBookmarkAction = actionCollection->addAction( QLatin1String("bookmark_previous"),
                                                               this, SLOT(gotoPreviousBookmark()) );
    mGotoPreviousBookmarkAction->setText( i18nc("@action:inmenu","Go to Previous Bookmark") );
    mGotoPreviousBookmarkAction->setShortcut( Qt::ALT + Qt::Key_Up );

    mBookmarksActionGroup = new QActionGroup( this ); // TODO: do we use this only for the signal mapping?
//     mBookmarksActionGroup->setExclusive( true );
    connect( mBookmarksActionGroup, SIGNAL(triggered(QAction*)),
             SLOT(onBookmarkTriggered(QAction*)) );

    setTargetModel( 0 );
}

void BookmarksController::setTargetModel( AbstractModel* model )
{
    if( mByteArrayView ) mByteArrayView->disconnect( this );
    if( mByteArray ) mByteArray->disconnect( this );

    mByteArrayView = model ? model->findBaseModel<ByteArrayView*>() : 0;

    ByteArrayDocument* document =
        mByteArrayView ? qobject_cast<ByteArrayDocument*>( mByteArrayView->baseModel() ) : 0;
    mByteArray = document ? document->content() : 0;
    mBookmarks = ( mByteArray && mByteArrayView ) ? qobject_cast<Okteta::Bookmarkable*>( mByteArray ) : 0;

    const bool hasViewWithBookmarks = ( mBookmarks != 0 );
    int bookmarksCount = 0;
    if( hasViewWithBookmarks )
    {
        bookmarksCount = mBookmarks->bookmarksCount();
        connect( mByteArray, SIGNAL(bookmarksAdded(QList<Okteta::Bookmark>)),
                 SLOT(onBookmarksAdded(QList<Okteta::Bookmark>)) );
        connect( mByteArray, SIGNAL(bookmarksRemoved(QList<Okteta::Bookmark>)),
                 SLOT(onBookmarksRemoved(QList<Okteta::Bookmark>)) );
        connect( mByteArray, SIGNAL(bookmarksModified(QList<int>)),
                 SLOT(updateBookmarks()) );
        connect( mByteArrayView, SIGNAL(cursorPositionChanged(Okteta::Address)),
                 SLOT(onCursorPositionChanged(Okteta::Address)) );
    }

    updateBookmarks();

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
    mGuiClient->unplugActionList( QLatin1String(BookmarkListActionListId) );

    qDeleteAll( mBookmarksActionGroup->actions() );

    if( mBookmarks == 0 )
        return;

    const int startOffset = mByteArrayView->startOffset();
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
    mGuiClient->plugActionList( QLatin1String(BookmarkListActionListId),
                                mBookmarksActionGroup->actions() );
}

void BookmarksController::onBookmarksAdded( const QList<Okteta::Bookmark> &bookmarks )
{
Q_UNUSED( bookmarks )
    const int currentPosition = mByteArrayView->cursorPosition();
    onCursorPositionChanged( currentPosition );

    const int bookmarksCount = mBookmarks->bookmarksCount();
    const bool hasBookmarks = ( bookmarksCount != 0 );

    mDeleteAllAction->setEnabled( hasBookmarks );

    updateBookmarks();
}

void BookmarksController::onBookmarksRemoved( const QList<Okteta::Bookmark> &bookmarks )
{
Q_UNUSED( bookmarks )
    const int currentPosition = mByteArrayView->cursorPosition();
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
    const int cursorPosition = mByteArrayView->cursorPosition();

    // search for text at cursor
    const Okteta::CharCodec* charCodec = Okteta::CharCodec::createCodec( mByteArrayView->charCodingName() );
    const Okteta::WordByteArrayService textService( mByteArray, charCodec );
    QString bookmarkName = textService.text( cursorPosition, cursorPosition+MaxBookmarkNameSize-1 );
    delete charCodec;

    if( bookmarkName.isEmpty() )
        bookmarkName = i18nc( "default name of a bookmark", "Bookmark" );// %1").arg( 0 ) ); // TODO: use counter like with new file, globally

    BookmarkEditPopup* bookmarkEditPopup = new BookmarkEditPopup( mByteArrayView->widget() );
    QPoint popupPoint = mByteArrayView->cursorRect().topLeft();
//     popupPoint.ry() += mSlider->height() / 2;
    popupPoint = mByteArrayView->widget()->mapToGlobal( popupPoint );

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
    const int cursorPosition = mByteArrayView->cursorPosition();
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
    const int currentPosition = mByteArrayView->cursorPosition();

    Okteta::BookmarksConstIterator bookmarksIterator = mBookmarks->createBookmarksConstIterator();
    const bool hasNext = bookmarksIterator.findNextFrom( currentPosition );
    if( hasNext )
    {
        const int newPosition = bookmarksIterator.next().offset();
        mByteArrayView->setCursorPosition( newPosition );
    }
}

void BookmarksController::gotoPreviousBookmark()
{
    const int currentPosition = mByteArrayView->cursorPosition();

    Okteta::BookmarksConstIterator bookmarksIterator = mBookmarks->createBookmarksConstIterator();
    const bool hasPrevious = bookmarksIterator.findPreviousFrom( currentPosition );
    if( hasPrevious )
    {
        const int newPosition = bookmarksIterator.previous().offset();
        mByteArrayView->setCursorPosition( newPosition );
    }
}

void BookmarksController::onBookmarkTriggered( QAction* action )
{
    const int newPosition = action->data().toInt();
    mByteArrayView->setCursorPosition( newPosition );
}


BookmarksController::~BookmarksController() {}

}
