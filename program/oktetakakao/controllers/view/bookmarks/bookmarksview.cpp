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

#include "bookmarksview.h"

// tool
#include "bookmarklistmodel.h"
#include "bookmarkstool.h"
// Okteta core
#include <bookmark.h>
// Qt
#include <QtGui/QLayout>
#include <QtGui/QTreeView>


BookmarksView::BookmarksView( BookmarksTool* tool, QWidget* parent )
 : QWidget( parent ), mTool( tool )
{
    mBookmarkListModel = new BookmarkListModel( mTool, this );

    QVBoxLayout *baseLayout = new QVBoxLayout( this );
    baseLayout->setMargin( 0 );

    mBookmarkListView = new QTreeView( this );
    mBookmarkListView->setObjectName( "BookmarkListView" );
    mBookmarkListView->setRootIsDecorated( false );
    mBookmarkListView->setItemsExpandable( false );
    mBookmarkListView->setUniformRowHeights( true );
    mBookmarkListView->setAllColumnsShowFocus( true );
    mBookmarkListView->setModel( mBookmarkListModel );
    for( int c = 0; c<BookmarkListModel::NoOfColumnIds; ++c )
        mBookmarkListView->resizeColumnToContents( c );
    connect( mBookmarkListView, SIGNAL(doubleClicked( const QModelIndex& )),
             SLOT(onBookmarkDoubleClicked( const QModelIndex& )) );

    baseLayout->addWidget( mBookmarkListView, 10 );
}


void BookmarksView::onBookmarkDoubleClicked( const QModelIndex& index )
{
    const int column = index.column();
    const bool isOffsetColum = ( column == BookmarkListModel::OffsetColumnId );
    if( mTool->hasModel() && isOffsetColum )
        mTool->gotoBookmark( mBookmarkListModel->bookmark(index) );
}

BookmarksView::~BookmarksView() {}
