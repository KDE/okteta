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
#include <KGuiItem>
// Qt
#include <QPushButton>
#include <QLayout>
#include <QTreeView>
#include <QHeaderView>

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
    auto* actionsLayout = new QHBoxLayout();

    const KGuiItem createBookmarkGuiItem =
        KGuiItem(QString() /*i18n("C&opy")*/,
                 QStringLiteral("bookmark-new"),
                 i18nc("@info:tooltip",
                       "Creates a new bookmark for the current cursor position."),
                 i18nc("@info:whatsthis",
                       "If you press this button, a new bookmark will be created "
                       "for the current cursor position."));
    mCreateBookmarkButton = new QPushButton(this);
    KGuiItem::assign(mCreateBookmarkButton, createBookmarkGuiItem);
    mCreateBookmarkButton->setEnabled(mTool->canCreateBookmark());
    connect(mCreateBookmarkButton, &QPushButton::clicked,
            this, &BookmarksView::onCreateBookmarkButtonClicked);
    connect(mTool, &BookmarksTool::canCreateBookmarkChanged,
            mCreateBookmarkButton, &QPushButton::setEnabled);
    actionsLayout->addWidget(mCreateBookmarkButton);

    const KGuiItem deleteBookmarkGuiItem =
        KGuiItem(QString() /*i18n("&Go to")*/,
                 QStringLiteral("edit-delete"),
                 i18nc("@info:tooltip",
                       "Deletes all the selected bookmarks."),
                 i18nc("@info:whatsthis",
                       "If you press this button, all bookmarks which are "
                       "selected will be deleted."));
    mDeleteBookmarksButton = new QPushButton(this);
    KGuiItem::assign(mDeleteBookmarksButton, deleteBookmarkGuiItem);
    connect(mDeleteBookmarksButton, &QPushButton::clicked, this, &BookmarksView::onDeleteBookmarkButtonClicked);
    actionsLayout->addWidget(mDeleteBookmarksButton);

    actionsLayout->addStretch();

    const KGuiItem gotoGuiItem =
        KGuiItem(QString() /*i18n("&Go to")*/,
                 QStringLiteral("go-jump"),
                 i18nc("@info:tooltip",
                       "Moves the cursor to the selected bookmark."),
                 i18nc("@info:whatsthis",
                       "If you press this button, the cursor is moved to the position "
                       "of the bookmark which has been last selected."));
    mGotoBookmarkButton = new QPushButton(this);
    KGuiItem::assign(mGotoBookmarkButton, gotoGuiItem);
    connect(mGotoBookmarkButton, &QPushButton::clicked, this, &BookmarksView::onGotoBookmarkButtonClicked);
    actionsLayout->addWidget(mGotoBookmarkButton);

    const KGuiItem renameGuiItem =
        KGuiItem(QString() /*i18n("&Go to")*/,
                 QStringLiteral("edit-rename"),
                 i18nc("@info:tooltip",
                       "Enables renaming of the selected bookmark."),
                 i18nc("@info:whatsthis",
                       "If you press this button, the name of the bookmark "
                       "which was last selected can be edited."));
    mRenameBookmarkButton = new QPushButton(this);
    KGuiItem::assign(mRenameBookmarkButton, renameGuiItem);
    connect(mRenameBookmarkButton, &QPushButton::clicked, this, &BookmarksView::onRenameBookmarkButtonClicked);
    actionsLayout->addWidget(mRenameBookmarkButton);

    baseLayout->addLayout(actionsLayout);

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
    mDeleteBookmarksButton->setEnabled(hasSelection);

    const bool bookmarkSelected = selectionModel->isSelected(selectionModel->currentIndex());
    mRenameBookmarkButton->setEnabled(bookmarkSelected);
    mGotoBookmarkButton->setEnabled(bookmarkSelected);
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
