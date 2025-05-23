/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BOOKMARKLISTMODEL_HPP
#define KASTEN_BOOKMARKLISTMODEL_HPP

// Okteta gui
#include <Okteta/OffsetFormat>
// Qt
#include <QAbstractTableModel>
#include <QFont>

namespace Okteta {
class Bookmark;
}
template <class T> class QVector;

namespace Kasten {

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

    enum Roles {
        OffsetStringRole = Qt::UserRole,
    };

public:
    explicit BookmarkListModel(BookmarksTool* tool, QObject* parent = nullptr);
    ~BookmarkListModel() override;

public: // QAbstractTableModel API
    [[nodiscard]]
    int rowCount(const QModelIndex& parent) const override;
    [[nodiscard]]
    int columnCount(const QModelIndex& parent) const override;
    [[nodiscard]]
    QVariant data(const QModelIndex& index, int role) const override;
    [[nodiscard]]
    Qt::ItemFlags flags(const QModelIndex& index) const override;
    [[nodiscard]]
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    [[nodiscard]]
    bool setData(const QModelIndex& index, const QVariant& value, int role) override;

public:
    [[nodiscard]]
    const Okteta::Bookmark& bookmark(const QModelIndex& index) const;
    [[nodiscard]]
    QModelIndex index(const Okteta::Bookmark& bookmark, int column = BookmarkListModel::TitleColumnId) const;
    using QAbstractTableModel::index;

private Q_SLOTS:
    void onHasBookmarksChanged(bool hasBookmarks);
    void onBookmarksChanged();
    void onBookmarksChanged(const QVector<int>& bookmarkIndizes);
//     void onHeadVersionChanged( int newHeadVersionIndex );
//     void onHeadVersionDataChanged( const DocumentVersionData &newVersionData );
    void onOffsetCodingChanged(int offsetCoding);

private:
    BookmarksTool* const mTool;

    Okteta::OffsetFormat::print mPrintFunction;
    mutable char mCodedOffset[Okteta::OffsetFormat::MaxFormatWidth + 1];
    QFont mFixedFont;
};

}

#endif
