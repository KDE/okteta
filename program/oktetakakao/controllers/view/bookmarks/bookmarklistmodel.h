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

#ifndef BOOKMARKLISTMODEL_H
#define BOOKMARKLISTMODEL_H

// Okteta gui
#include <koffsetformat.h>
// Qt
#include <QtCore/QAbstractTableModel>
#include <QtCore/QList>

class BookmarksTool;
namespace KHECore {
class BookmarkList;
class Bookmark;
}


class BookmarkListModel : public QAbstractTableModel
{
  Q_OBJECT

  public:
    enum ColumnIds
    {
        OffsetColumnId = 0,
        TitleColumnId = 1,
        NoOfColumnIds = 2 // TODO: what pattern is usually used to mark number of ids?
    };

  public:
    explicit BookmarkListModel( BookmarksTool* tool, QObject* parent = 0 );
    virtual ~BookmarkListModel();

  public: // QAbstractTableModel API
    virtual int rowCount( const QModelIndex& parent ) const;
    virtual int columnCount( const QModelIndex& parent ) const;
    virtual QVariant data( const QModelIndex& index, int role ) const;
    virtual QVariant headerData( int section, Qt::Orientation orientation, int role ) const;

  public:
    KHECore::Bookmark bookmark( const QModelIndex& index ) const;

  protected Q_SLOTS:
    void onModelChanged( bool hasModel );
    void onBookmarksChanged();
//     void onHeadVersionChanged( int newHeadVersionIndex );
//     void onHeadVersionDataChanged( const KDocumentVersionData &newVersionData );

  protected:
    BookmarksTool* mTool;
    QList<KHECore::Bookmark> mBookmarkList;

    KHEUI::KOffsetFormat::print mPrintFunction;
    mutable char mCodedOffset[KHEUI::KOffsetFormat::MaxFormatWidth+1];
};

#endif
