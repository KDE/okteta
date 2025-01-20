/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BOOKMARKSVIEW_HPP
#define KASTEN_BOOKMARKSVIEW_HPP

// Qt
#include <QWidget>

class QTreeView;
class QLabel;
class QAction;
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
    [[nodiscard]]
    BookmarksTool* tool() const;

private Q_SLOTS:
    void updateEmptyListOverlayLabel();

    void onBookmarkDoubleClicked(const QModelIndex& index);
    void onBookmarkSelectionChanged();
    void onCreateBookmarkButtonClicked();
    void onDeleteBookmarkButtonClicked();
    void onGotoBookmarkButtonClicked();
    void onRenameBookmarkButtonClicked();

private:
    BookmarksTool* const mTool;

    BookmarkListModel* mBookmarkListModel;

    QTreeView* mBookmarkListView;
    QLabel* m_emptyListOverlayLabel;
    QAction* mCreateBookmarkAction;
    QAction* mDeleteBookmarksAction;
    QAction* mGotoBookmarkAction;
    QAction* mRenameBookmarkAction;
};

inline BookmarksTool* BookmarksView::tool() const { return mTool; }

}

#endif
