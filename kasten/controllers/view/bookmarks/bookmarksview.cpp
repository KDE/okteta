/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bookmarksview.hpp"

// tool
#include "bookmarklistmodel.hpp"
#include "bookmarkstool.hpp"
// Okteta core
#include <Okteta/Bookmark>
// KF
#include <KLocalizedString>
// Qt
#include <QToolBar>
#include <QAction>
#include <QLayout>
#include <QTreeView>
#include <QHeaderView>
#include <QIcon>

namespace Kasten {

BookmarksView::BookmarksView(BookmarksTool* tool, QWidget* parent)
    : QWidget(parent)
    , mTool(tool)
{
    mBookmarkListModel = new BookmarkListModel(mTool, this);
    connect(mBookmarkListModel, &BookmarkListModel::modelReset,
            this, &BookmarksView::onBookmarkSelectionChanged);

    auto* baseLayout = new QVBoxLayout(this);
    baseLayout->setContentsMargins(0, 0, 0, 0);
    baseLayout->setSpacing(0);

    // bookmarks list
    mBookmarkListView = new QTreeView(this);
    mBookmarkListView->setObjectName(QStringLiteral("BookmarkListView"));
    mBookmarkListView->setRootIsDecorated(false);
    mBookmarkListView->setItemsExpandable(false);
    mBookmarkListView->setUniformRowHeights(true);
    mBookmarkListView->setAllColumnsShowFocus(true);
    mBookmarkListView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    mBookmarkListView->setModel(mBookmarkListModel);
    mBookmarkListView->header()->setSectionResizeMode(QHeaderView::Interactive);
    connect(mBookmarkListView, &QTreeView::doubleClicked,
            this, &BookmarksView::onBookmarkDoubleClicked);
    connect(mBookmarkListView->selectionModel(),
            &QItemSelectionModel::selectionChanged,
            this, &BookmarksView::onBookmarkSelectionChanged);

    baseLayout->addWidget(mBookmarkListView, 10);

    // actions TODO: make this view work like the filebrowser, with actions on top?
    auto* actionsToolBar = new QToolBar(this);

    mCreateBookmarkAction =
        actionsToolBar->addAction(QIcon::fromTheme(QStringLiteral("bookmark-new")),
                                  QString() /*i18n("C&opy")*/,
                                  this, &BookmarksView::onCreateBookmarkButtonClicked);
    mCreateBookmarkAction->setToolTip(i18nc("@info:tooltip",
                                            "Creates a new bookmark for the current cursor position."));
    mCreateBookmarkAction->setWhatsThis(i18nc("@info:whatsthis",
                                              "If you press this button, a new bookmark will be created "
                                              "for the current cursor position."));
    mCreateBookmarkAction->setEnabled(mTool->canCreateBookmark());
    connect(mTool, &BookmarksTool::canCreateBookmarkChanged,
            mCreateBookmarkAction, &QAction::setEnabled);

    mDeleteBookmarksAction =
        actionsToolBar->addAction(QIcon::fromTheme(QStringLiteral("edit-delete")),
                                  QString(),
                                  this, &BookmarksView::onDeleteBookmarkButtonClicked);
    mDeleteBookmarksAction->setToolTip(i18nc("@info:tooltip",
                                             "Deletes all the selected bookmarks."));
    mDeleteBookmarksAction->setWhatsThis(i18nc("@info:whatsthis",
                                               "If you press this button, all bookmarks which are "
                                               "selected will be deleted."));

    auto* stretcher = new QWidget(this);
    stretcher->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    actionsToolBar->addWidget(stretcher);

    mGotoBookmarkAction =
        actionsToolBar->addAction(QIcon::fromTheme(QStringLiteral("go-jump")),
                                  QString() /*i18n("&Go to")*/,
                                  this, &BookmarksView::onGotoBookmarkButtonClicked);
    mGotoBookmarkAction->setToolTip(i18nc("@info:tooltip",
                                          "Moves the cursor to the selected bookmark."));
    mGotoBookmarkAction->setWhatsThis(i18nc("@info:whatsthis",
                                            "If you press this button, the cursor is moved to the position "
                                            "of the bookmark which has been last selected."));

    mRenameBookmarkAction =
        actionsToolBar->addAction(QIcon::fromTheme(QStringLiteral("edit-rename")),
                                  QString(),
                                  this, &BookmarksView::onRenameBookmarkButtonClicked);
    mRenameBookmarkAction->setToolTip(i18nc("@info:tooltip",
                                            "Enables renaming of the selected bookmark."));
    mRenameBookmarkAction->setWhatsThis(i18nc("@info:whatsthis",
                                              "If you press this button, the name of the bookmark "
                                              "which was last selected can be edited."));

    baseLayout->addWidget(actionsToolBar);

    onBookmarkSelectionChanged();
}

BookmarksView::~BookmarksView() = default;

void BookmarksView::onBookmarkDoubleClicked(const QModelIndex& index)
{
    const int column = index.column();
    const bool isOffsetColum = (column == BookmarkListModel::OffsetColumnId);
    if (isOffsetColum) {
        mTool->gotoBookmark(mBookmarkListModel->bookmark(index));
    }
}

void BookmarksView::onBookmarkSelectionChanged()
{
    const QItemSelectionModel* selectionModel = mBookmarkListView->selectionModel();

    // TODO: selectionModel->selectedIndexes() is a expensive operation,
    // but with Qt 4.4.3 hasSelection() has the flaw to return true with a current index
    const bool hasSelection = !selectionModel->selectedIndexes().isEmpty();
    mDeleteBookmarksAction->setEnabled(hasSelection);

    const bool bookmarkSelected = selectionModel->isSelected(selectionModel->currentIndex());
    mRenameBookmarkAction->setEnabled(bookmarkSelected);
    mGotoBookmarkAction->setEnabled(bookmarkSelected);
}

void BookmarksView::onCreateBookmarkButtonClicked()
{
    const Okteta::Bookmark bookmark = mTool->createBookmark();
    if (bookmark.isValid()) {
        const QModelIndex index = mBookmarkListModel->index(bookmark, BookmarkListModel::TitleColumnId);
        if (index.isValid()) {
            mBookmarkListView->edit(index);
        }
    }
}

void BookmarksView::onDeleteBookmarkButtonClicked()
{
    const QModelIndexList selectedRows = mBookmarkListView->selectionModel()->selectedRows();

    QVector<Okteta::Bookmark> bookmarksToBeDeleted;
    bookmarksToBeDeleted.reserve(selectedRows.size());
    for (const QModelIndex& index : selectedRows) {
        const Okteta::Bookmark& bookmark = mBookmarkListModel->bookmark(index);
        bookmarksToBeDeleted.append(bookmark);
    }

    mTool->deleteBookmarks(bookmarksToBeDeleted);
}

void BookmarksView::onGotoBookmarkButtonClicked()
{
    const QModelIndex index = mBookmarkListView->selectionModel()->currentIndex();
    if (index.isValid()) {
        mTool->gotoBookmark(mBookmarkListModel->bookmark(index));
    }
}

void BookmarksView::onRenameBookmarkButtonClicked()
{
    QModelIndex index = mBookmarkListView->selectionModel()->currentIndex();
    const QModelIndex nameIndex = index.sibling(index.row(), BookmarkListModel::TitleColumnId);
    if (nameIndex.isValid()) {
        mBookmarkListView->edit(nameIndex);
    }
}

}
