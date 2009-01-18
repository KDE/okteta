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

#include "bookmarklistmodel.h"

// lib
#include "bookmarkstool.h"
// Okteta core
#include <bookmarklist.h>
// KDE
#include <KLocale>
#include <KIcon>


BookmarkListModel::BookmarkListModel( BookmarksTool* tool, QObject* parent )
 : QAbstractTableModel( parent ),
   mTool( tool )
{
    mPrintFunction = KHEUI::KOffsetFormat::printFunction( KHEUI::KOffsetFormat::Hexadecimal );

    connect( mTool, SIGNAL(modelChanged( bool )), SLOT(onModelChanged( bool )) );
    connect( mTool, SIGNAL(bookmarksAdded( const QList<KHECore::Bookmark>& )),
             SLOT(onBookmarksChanged()) );
    connect( mTool, SIGNAL(bookmarksRemoved( const QList<KHECore::Bookmark>& )),
             SLOT(onBookmarksChanged()) );
    connect( mTool, SIGNAL(bookmarksModified( bool )), SLOT(onBookmarksChanged()) );
//     connect( mTool, SIGNAL(cursorPositionChanged( int )),
//              SLOT(onCursorPositionChanged( int )) );
}

int BookmarkListModel::rowCount( const QModelIndex& parent ) const
{
Q_UNUSED( parent )
    return mBookmarkList.count();
}

int BookmarkListModel::columnCount( const QModelIndex& parent ) const
{
Q_UNUSED( parent )
    return NoOfColumnIds;
}

QVariant BookmarkListModel::data( const QModelIndex& index, int role ) const
{
    QVariant result;
    if( role == Qt::DisplayRole )
    {
        const int bookmarkIndex = index.row();

        // TODO: think if QLinkedList is really useful here
        const KHECore::Bookmark bookmark = mBookmarkList.at( bookmarkIndex );

        const int tableColumn = index.column();
        switch( tableColumn )
        {
            case OffsetColumnId:
            {
                mPrintFunction( mCodedOffset, bookmark.offset() );

                result = QString().append( mCodedOffset );
                break;
            }
            case TitleColumnId:
                result = "Bookmark";//bookmark.name();
                break;
            default:
                ;
        }
    }
#if 0
    else if( role == Qt::DecorationRole )
    {
        const int tableColumn = index.column();
        if( tableColumn == CurrentColumnId )
        {
            const int versionIndex = index.row();
            if( mVersionControl->versionIndex() == versionIndex )
                result = KIcon( "arrow-right" );
        }
    }
#endif
    return result;
}

QVariant BookmarkListModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
    QVariant result;

    if( role == Qt::DisplayRole )
    {
        const QString titel =
            section == OffsetColumnId ?  i18nc("@title:column offset of the bookmark",     "Offset") :
            section == TitleColumnId ?   i18nc("@title:column title of the bookmark", "Title") :
            QString();
        result = titel;
    }
    else
        result = QAbstractTableModel::headerData( section, orientation, role );

    return result;
}

KHECore::Bookmark BookmarkListModel::bookmark( const QModelIndex& index ) const
{
    const int bookmarkIndex = index.row();
    return mBookmarkList.at( bookmarkIndex );
}


void BookmarkListModel::onModelChanged( bool hasModel )
{
    if( hasModel )
        mBookmarkList = mTool->bookmarks().list();
    else
        mBookmarkList.clear();

    reset();
}

void BookmarkListModel::onBookmarksChanged()
{
    mBookmarkList = mTool->bookmarks().list();
    reset();
}
#if 0
void BookmarkListModel::onRevertedToVersionIndex( int versionIndex )
{
    if( mVersionIndex == versionIndex )
        return;

    const int oldVersionIndex = mVersionIndex;
    mVersionIndex = versionIndex;

    emit dataChanged( index(versionIndex,CurrentColumnId), index(versionIndex,CurrentColumnId) );
    emit dataChanged( index(oldVersionIndex,CurrentColumnId), index(oldVersionIndex,CurrentColumnId) );
}

void BookmarkListModel::onHeadVersionDataChanged( const KDocumentVersionData &versionData )
{
    Q_UNUSED( versionData )
    const int headVersionIndex = mVersionControl->versionCount() - 1;
    emit dataChanged( index(headVersionIndex,CurrentColumnId), index(headVersionIndex,ChangeDescriptionColumnId) );
}
#endif
BookmarkListModel::~BookmarkListModel() {}
