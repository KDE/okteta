/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BOOKMARKSVIEW_HPP
#define KASTEN_BOOKMARKSVIEW_HPP

// Qt
#include <QWidget>

class QPushButton;
class QTreeView;
class QModelIndex;

namespace Kasten {

class BookmarkListModel;
class BookmarksTool;

class BookmarksView : public QWidget
{
    Q_OBJECT

public:
    explicit BookmarksView(BookmarksTool* tool, QWidget* parent = nullptr);
    ~BookmarksView() override;

public:
    BookmarksTool* tool() const;

private Q_SLOTS:
    void onBookmarkDoubleClicked(const QModelIndex& index);
    void onBookmarkSelectionChanged();
    void onCreateBookmarkButtonClicked();
    void onDeleteBookmarkButtonClicked();
    void onGotoBookmarkButtonClicked();
    void onRenameBookmarkButtonClicked();

private:
    BookmarksTool* mTool;

    BookmarkListModel* mBookmarkListModel;

    QTreeView* mBookmarkListView;
    QPushButton* mCreateBookmarkButton;
    QPushButton* mDeleteBookmarksButton;
    QPushButton* mGotoBookmarkButton;
    QPushButton* mRenameBookmarkButton;
};

inline BookmarksTool* BookmarksView::tool() const { return mTool; }

}

#endif
