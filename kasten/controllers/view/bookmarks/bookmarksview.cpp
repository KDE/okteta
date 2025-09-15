/*
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
#include <QApplication>
#include <QClipboard>
#include <QToolBar>
#include <QLabel>
#include <QAction>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTreeView>
#include <QHeaderView>
#include <QIcon>
#include <QMenu>

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
    mBookmarkListView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(mBookmarkListView, &QTreeView::doubleClicked,
            this, &BookmarksView::onBookmarkDoubleClicked);
    connect(mBookmarkListView->selectionModel(),
            &QItemSelectionModel::selectionChanged,
            this, &BookmarksView::onBookmarkSelectionChanged);
    connect(mBookmarkListView, &QWidget::customContextMenuRequested,
            this, &BookmarksView::onCustomContextMenuRequested);

    // TODO. share code for all these empty-list placeholders
    auto* bookmarkListViewViewPort = mBookmarkListView->viewport();
    m_emptyListOverlayLabel = new QLabel(bookmarkListViewViewPort);
    m_emptyListOverlayLabel->setText(i18nc("@info", "No bookmarks"));
    m_emptyListOverlayLabel->setAttribute(Qt::WA_TransparentForMouseEvents);
    m_emptyListOverlayLabel->setEnabled(false);
    m_emptyListOverlayLabel->setWordWrap(true);
    m_emptyListOverlayLabel->setAlignment(Qt::AlignCenter);
    m_emptyListOverlayLabel->setVisible(mTool->isBookmarkListEmpty());
    m_emptyListOverlayLabel->setContextMenuPolicy(Qt::NoContextMenu);
    auto* centeringLayout = new QVBoxLayout(bookmarkListViewViewPort);
    centeringLayout->addWidget(m_emptyListOverlayLabel);
    centeringLayout->setAlignment(m_emptyListOverlayLabel, Qt::AlignCenter);
    connect(mTool, &BookmarksTool::hasBookmarksChanged,
            this, &BookmarksView::updateEmptyListOverlayLabel);
    connect(mTool, &BookmarksTool::bookmarksAdded,
            this, &BookmarksView::updateEmptyListOverlayLabel);
    connect(mTool, &BookmarksTool::bookmarksRemoved,
            this, &BookmarksView::updateEmptyListOverlayLabel);

    baseLayout->addWidget(mBookmarkListView, 10);

    // actions TODO: make this view work like the filebrowser, with actions on top?
    auto* actionsToolBar = new QToolBar(this);

    mCreateBookmarkAction = new QAction(QIcon::fromTheme(QStringLiteral("bookmark-new")),
                                        i18nc("@action", "Create New"), this);
    mCreateBookmarkAction->setToolTip(i18nc("@info:tooltip",
                                            "Creates a new bookmark for the current cursor position."));
    mCreateBookmarkAction->setWhatsThis(i18nc("@info:whatsthis",
                                              "If you press this button, a new bookmark will be created "
                                              "for the current cursor position."));
    mCreateBookmarkAction->setEnabled(mTool->canCreateBookmark());
    connect(mTool, &BookmarksTool::canCreateBookmarkChanged,
            mCreateBookmarkAction, &QAction::setEnabled);
    connect(mCreateBookmarkAction, &QAction::triggered,
            this, &BookmarksView::onCreateBookmarkButtonClicked);
    actionsToolBar->addAction(mCreateBookmarkAction);

    mDeleteBookmarksAction =new QAction(QIcon::fromTheme(QStringLiteral("edit-delete")),
                                        i18nc("@action", "Delete"), this);
    mDeleteBookmarksAction->setToolTip(i18nc("@info:tooltip",
                                             "Deletes all the selected bookmarks."));
    mDeleteBookmarksAction->setWhatsThis(i18nc("@info:whatsthis",
                                               "If you press this button, all bookmarks which are "
                                               "selected will be deleted."));
    connect(mDeleteBookmarksAction, &QAction::triggered,
            this, &BookmarksView::onDeleteBookmarkButtonClicked);
    actionsToolBar->addAction(mDeleteBookmarksAction);

    auto* stretcher = new QWidget(this);
    stretcher->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    actionsToolBar->addWidget(stretcher);

    mGotoBookmarkAction = new QAction(QIcon::fromTheme(QStringLiteral("go-jump")),
                                      i18nc("@action", "Go to Offset"), this);
    mGotoBookmarkAction->setToolTip(i18nc("@info:tooltip",
                                          "Moves the cursor to the selected bookmark."));
    mGotoBookmarkAction->setWhatsThis(i18nc("@info:whatsthis",
                                            "If you press this button, the cursor is moved to the position "
                                            "of the bookmark which has been last selected."));
    mGotoBookmarkAction->setPriority(QAction::HighPriority);
    connect(mGotoBookmarkAction, &QAction::triggered,
            this, &BookmarksView::onGotoBookmarkButtonClicked);
    actionsToolBar->addAction(mGotoBookmarkAction);

    mRenameBookmarkAction =new QAction(QIcon::fromTheme(QStringLiteral("edit-rename")),
                                       i18nc("@action", "Rename"), this);
    mRenameBookmarkAction->setToolTip(i18nc("@info:tooltip",
                                            "Enables renaming of the selected bookmark."));
    mRenameBookmarkAction->setWhatsThis(i18nc("@info:whatsthis",
                                              "If you press this button, the name of the bookmark "
                                              "which was last selected can be edited."));
    connect(mRenameBookmarkAction, &QAction::triggered,
            this, &BookmarksView::onRenameBookmarkButtonClicked);
    actionsToolBar->addAction(mRenameBookmarkAction);

    baseLayout->addWidget(actionsToolBar);

    mCopyOffsetAction = new QAction(QIcon::fromTheme(QStringLiteral("edit-copy")),
                                    i18nc("@action", "Copy Offset"), this);
    mCopyOffsetAction->setToolTip(i18nc("@info:tooltip",
                                        "Copies the offset to the clipboard."));
    connect(mCopyOffsetAction, &QAction::triggered,
            this, &BookmarksView::onCopyOffsetTriggered);
    onBookmarkSelectionChanged();
}

BookmarksView::~BookmarksView() = default;

void BookmarksView::updateEmptyListOverlayLabel()
{
    m_emptyListOverlayLabel->setVisible(mTool->isBookmarkListEmpty());
}

void BookmarksView::onBookmarkDoubleClicked(const QModelIndex& index)
{
    const int column = index.column();
    const bool isOffsetColum = (column == BookmarkListModel::OffsetColumnId);
    if (isOffsetColum) {
        mTool->gotoBookmark(mBookmarkListModel->bookmark(index));
    }
}

void BookmarksView::onCustomContextMenuRequested(QPoint pos)
{
    auto* menu = new QMenu(this);
    menu->setAttribute(Qt::WA_DeleteOnClose);

    const QModelIndex index = mBookmarkListView->indexAt(pos);

    if (index.isValid()) {
        menu->addAction(mGotoBookmarkAction);
        menu->addAction(mRenameBookmarkAction);
        menu->addSeparator();
        menu->addAction(mCopyOffsetAction);
        menu->addSeparator();
        menu->addAction(mDeleteBookmarksAction);
        menu->addSeparator();
    }

    menu->addAction(mCreateBookmarkAction);

    menu->popup(mBookmarkListView->viewport()->mapToGlobal(pos));
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

void BookmarksView::onCopyOffsetTriggered()
{
    const QModelIndex index = mBookmarkListView->selectionModel()->currentIndex();
    if (index.isValid()) {
        const QString offsetText = index.data(BookmarkListModel::OffsetStringRole).toString();
        QApplication::clipboard()->setText(offsetText);
    }
}

}

#include "moc_bookmarksview.cpp"
