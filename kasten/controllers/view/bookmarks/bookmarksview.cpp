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

#include "bookmarksview.h"

// tool
#include "bookmarklistmodel.h"
#include "bookmarkstool.h"
// Okteta core
#include <bookmark.h>
// KDE
#include <KPushButton>
#include <KLocale>
#include <KGuiItem>
#include <KToolBar>
// #include <KActionCollection>
// Qt
#include <QtGui/QLayout>
#include <QtGui/QTreeView>
#include <QtGui/QHeaderView>


namespace Kasten2
{

BookmarksView::BookmarksView( BookmarksTool* tool, QWidget* parent )
  : QWidget( parent ),
    mTool( tool )
{
    mBookmarkListModel = new BookmarkListModel( mTool, this );
    connect( mBookmarkListModel, SIGNAL(modelReset()),
             SLOT(onBookmarkSelectionChanged()) );

    QVBoxLayout* baseLayout = new QVBoxLayout( this );
    baseLayout->setMargin( 0 );

    // tool bar
    KToolBar* toolbar = new KToolBar( this );
    toolbar->setMovable( false );
    toolbar->setToolButtonStyle( Qt::ToolButtonIconOnly );
    toolbar->setIconDimensions( 16 );
    toolbar->setContextMenuPolicy( Qt::NoContextMenu );
    baseLayout->addWidget( toolbar );

    mBookmarkListView = new QTreeView( this );
    mBookmarkListView->setObjectName( QLatin1String( "BookmarkListView" ) );
    mBookmarkListView->setRootIsDecorated( false );
    mBookmarkListView->setItemsExpandable( false );
    mBookmarkListView->setUniformRowHeights( true );
    mBookmarkListView->setAllColumnsShowFocus( true );
    mBookmarkListView->setSelectionMode( QAbstractItemView::ExtendedSelection );
    mBookmarkListView->setModel( mBookmarkListModel );
    mBookmarkListView->header()->setResizeMode( QHeaderView::Interactive );
    connect( mBookmarkListView, SIGNAL(doubleClicked(QModelIndex)),
             SLOT(onBookmarkDoubleClicked(QModelIndex)) );
    connect( mBookmarkListView->selectionModel(),
             SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
             SLOT(onBookmarkSelectionChanged()) );

    baseLayout->addWidget( mBookmarkListView, 10 );

    // actions TODO: make this view work like the filebrowser?
//     KActionCollection* actionCollection = new KActionCollection( this );


    QHBoxLayout* actionsLayout = new QHBoxLayout();

    const KGuiItem createBookmarkGuiItem =
        KGuiItem( QString()/*i18n("C&opy")*/,
                  QLatin1String("bookmark-new"),
                  i18nc("@info:tooltip",
                        "Creates a new bookmark for the current cursor position."),
                  i18nc("@info:whatsthis",
                        "If you press this button, a new bookmark will be created "
                        "for the current cursor position.") );
    mCreateBookmarkButton = new KPushButton( createBookmarkGuiItem, this );
    mCreateBookmarkButton->setEnabled( mTool->canCreateBookmark() );
    connect( mCreateBookmarkButton, SIGNAL(clicked(bool)),
             SLOT(onCreateBookmarkButtonClicked()) );
    connect( mTool, SIGNAL(canCreateBookmarkChanged(bool)),
             mCreateBookmarkButton, SLOT(setEnabled(bool)) );
    actionsLayout->addWidget( mCreateBookmarkButton );

    const KGuiItem deleteBookmarkGuiItem =
        KGuiItem( QString()/*i18n("&Go to")*/,
                  QLatin1String("edit-delete"),
                  i18nc("@info:tooltip",
                        "Deletes all the selected bookmarks."),
                  i18nc("@info:whatsthis",
                        "If you press this button, all bookmarks which are "
                        "selected will be deleted.") );
    mDeleteBookmarksButton = new KPushButton( deleteBookmarkGuiItem, this );
    connect( mDeleteBookmarksButton, SIGNAL(clicked(bool)), SLOT(onDeleteBookmarkButtonClicked()) );
    actionsLayout->addWidget( mDeleteBookmarksButton );

    actionsLayout->addStretch();

    const KGuiItem gotoGuiItem =
        KGuiItem( QString()/*i18n("&Go to")*/,
                  QLatin1String("go-jump"),
                  i18nc("@info:tooltip",
                        "Moves the cursor to the selected bookmark."),
                  i18nc("@info:whatsthis",
                        "If you press this button, the cursor is moved to the position "
                        "of the bookmark which has been last selected.") );
    mGotoBookmarkButton = new KPushButton( gotoGuiItem, this );
    connect( mGotoBookmarkButton, SIGNAL(clicked(bool)), SLOT(onGotoBookmarkButtonClicked()) );
    actionsLayout->addWidget( mGotoBookmarkButton );

    const KGuiItem renameGuiItem =
        KGuiItem( QString()/*i18n("&Go to")*/,
                  QLatin1String("edit-rename"),
                  i18nc("@info:tooltip",
                        "Enables renaming of the selected bookmark."),
                  i18nc("@info:whatsthis",
                        "If you press this button, the name of the bookmark "
                        "which was last selected can be edited.") );
    mRenameBookmarkButton = new KPushButton( renameGuiItem, this );
    connect( mRenameBookmarkButton, SIGNAL(clicked(bool)), SLOT(onRenameBookmarkButtonClicked()) );
    actionsLayout->addWidget( mRenameBookmarkButton );

    baseLayout->addLayout( actionsLayout );

    onBookmarkSelectionChanged();
}


void BookmarksView::onBookmarkDoubleClicked( const QModelIndex& index )
{
    const int column = index.column();
    const bool isOffsetColum = ( column == BookmarkListModel::OffsetColumnId );
    if( isOffsetColum )
        mTool->gotoBookmark( mBookmarkListModel->bookmark(index) );
}

void BookmarksView::onBookmarkSelectionChanged()
{
    const QItemSelectionModel* selectionModel = mBookmarkListView->selectionModel();

    // TODO: selectionModel->selectedIndexes() is a expensive operation,
    // but with Qt 4.4.3 hasSelection() has the flaw to return true with a current index
    const bool hasSelection = !selectionModel->selectedIndexes().isEmpty();
    mDeleteBookmarksButton->setEnabled( hasSelection );

    const bool bookmarkSelected = selectionModel->isSelected( selectionModel->currentIndex() );
    mRenameBookmarkButton->setEnabled( bookmarkSelected );
    mGotoBookmarkButton->setEnabled( bookmarkSelected );
}

void BookmarksView::onCreateBookmarkButtonClicked()
{
    const Okteta::Bookmark bookmark = mTool->createBookmark();
    if( bookmark.isValid() )
    {
        const QModelIndex index = mBookmarkListModel->index( bookmark, BookmarkListModel::TitleColumnId );
        if( index.isValid() )
            mBookmarkListView->edit( index );
    }
}

void BookmarksView::onDeleteBookmarkButtonClicked()
{
    const QModelIndexList selectedRows = mBookmarkListView->selectionModel()->selectedRows();

    QList<Okteta::Bookmark> bookmarksToBeDeleted;
    foreach( const QModelIndex& index, selectedRows )
    {
        const Okteta::Bookmark& bookmark = mBookmarkListModel->bookmark( index );
        bookmarksToBeDeleted.append( bookmark );
    }
    mTool->deleteBookmarks( bookmarksToBeDeleted );
}

void BookmarksView::onGotoBookmarkButtonClicked()
{
    const QModelIndex index = mBookmarkListView->selectionModel()->currentIndex();
    if( index.isValid() )
        mTool->gotoBookmark( mBookmarkListModel->bookmark(index) );
}

void BookmarksView::onRenameBookmarkButtonClicked()
{
    QModelIndex index = mBookmarkListView->selectionModel()->currentIndex();
    const QModelIndex nameIndex = index.sibling( index.row(), BookmarkListModel::TitleColumnId );
    if( nameIndex.isValid() )
        mBookmarkListView->edit( nameIndex );
}


BookmarksView::~BookmarksView() {}

}
