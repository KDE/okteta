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

#ifndef BOOKMARKLISTMODEL_H
#define BOOKMARKLISTMODEL_H

// Okteta gui
#include <offsetformat.h>
// Qt
#include <QtCore/QAbstractTableModel>

namespace Okteta {
class Bookmark;
}
template <class T> class QList;


namespace Kasten2
{

class BookmarksTool;


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
    virtual Qt::ItemFlags flags( const QModelIndex& index ) const;
    virtual QVariant headerData( int section, Qt::Orientation orientation, int role ) const;
    virtual bool setData( const QModelIndex& index, const QVariant& value, int role );

  public:
    const Okteta::Bookmark& bookmark( const QModelIndex& index ) const;
    QModelIndex index( const Okteta::Bookmark& bookmark, int column = BookmarkListModel::TitleColumnId ) const;
    using QAbstractTableModel::index;

  private Q_SLOTS:
    void onHasBookmarksChanged( bool hasBookmarks );
    void onBookmarksChanged();
    void onBookmarksChanged( const QList<int>& bookmarkIndizes );
//     void onHeadVersionChanged( int newHeadVersionIndex );
//     void onHeadVersionDataChanged( const DocumentVersionData &newVersionData );

  private:
    BookmarksTool* mTool;

    Okteta::OffsetFormat::print mPrintFunction;
    mutable char mCodedOffset[Okteta::OffsetFormat::MaxFormatWidth+1];
};

}

#endif
