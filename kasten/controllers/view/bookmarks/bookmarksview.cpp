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
#include <KActionMenu>
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
#include <QSortFilterProxyModel>
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

    // actions
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

    mDeleteBookmarksAction =new QAction(QIcon::fromTheme(QStringLiteral("edit-delete")),
                                        i18nc("@action", "Delete"), this);
    mDeleteBookmarksAction->setToolTip(i18nc("@info:tooltip",
                                             "Deletes all the selected bookmarks."));
    mDeleteBookmarksAction->setWhatsThis(i18nc("@info:whatsthis",
                                               "If you press this button, all bookmarks which are "
                                               "selected will be deleted."));
    connect(mDeleteBookmarksAction, &QAction::triggered,
            this, &BookmarksView::onDeleteBookmarkButtonClicked);

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

    mRenameBookmarkAction =new QAction(QIcon::fromTheme(QStringLiteral("edit-rename")),
                                       i18nc("@action", "Rename"), this);
    mRenameBookmarkAction->setToolTip(i18nc("@info:tooltip",
                                            "Enables renaming of the selected bookmark."));
    mRenameBookmarkAction->setWhatsThis(i18nc("@info:whatsthis",
                                              "If you press this button, the name of the bookmark "
                                              "which was last selected can be edited."));
    connect(mRenameBookmarkAction, &QAction::triggered,
            this, &BookmarksView::onRenameBookmarkButtonClicked);

    mCopyOffsetAction = new QAction(QIcon::fromTheme(QStringLiteral("edit-copy")),
                                    i18nc("@action", "Copy Offset"), this);
    mCopyOffsetAction->setToolTip(i18nc("@info:tooltip",
                                        "Copies the offset to the clipboard."));
    connect(mCopyOffsetAction, &QAction::triggered,
            this, &BookmarksView::onCopyOffsetTriggered);

    // UI content
    auto* const baseLayout = new QVBoxLayout(this);
    baseLayout->setContentsMargins(0, 0, 0, 0);
    baseLayout->setSpacing(0);

    // bookmarks list
    m_sortProxyModel = new QSortFilterProxyModel(this);
    m_sortProxyModel->setDynamicSortFilter(true);
    m_sortProxyModel->setSourceModel(mBookmarkListModel);

    mBookmarkListView = new QTreeView(this);
    mBookmarkListView->setObjectName(QStringLiteral("BookmarkListView"));
    mBookmarkListView->setRootIsDecorated(false);
    mBookmarkListView->setItemsExpandable(false);
    mBookmarkListView->setAlternatingRowColors(true);
    mBookmarkListView->setUniformRowHeights(true);
    mBookmarkListView->setAllColumnsShowFocus(true);
    mBookmarkListView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    mBookmarkListView->setSortingEnabled(true);
    mBookmarkListView->header()->setSectionResizeMode(QHeaderView::Interactive);
    mBookmarkListView->setContextMenuPolicy(Qt::CustomContextMenu);
    mBookmarkListView->setModel(m_sortProxyModel);
    mBookmarkListView->sortByColumn(BookmarkListModel::OffsetColumnId, Qt::AscendingOrder);
    connect(mBookmarkListView, &QTreeView::doubleClicked,
            this, &BookmarksView::onBookmarkDoubleClicked);
    connect(mBookmarkListView->selectionModel(),
            &QItemSelectionModel::selectionChanged,
            this, &BookmarksView::onBookmarkSelectionChanged);
    connect(mBookmarkListView, &QWidget::customContextMenuRequested,
            this, &BookmarksView::onCustomContextMenuRequested);

    // TODO. share code for all these empty-list placeholders
    QWidget* const bookmarkListViewViewPort = mBookmarkListView->viewport();
    m_emptyListOverlayLabel = new QLabel(bookmarkListViewViewPort);
    m_emptyListOverlayLabel->setText(i18nc("@info", "No bookmarks"));
    m_emptyListOverlayLabel->setAttribute(Qt::WA_TransparentForMouseEvents);
    m_emptyListOverlayLabel->setEnabled(false);
    m_emptyListOverlayLabel->setWordWrap(true);
    m_emptyListOverlayLabel->setAlignment(Qt::AlignCenter);
    m_emptyListOverlayLabel->setVisible(mTool->isBookmarkListEmpty());
    m_emptyListOverlayLabel->setContextMenuPolicy(Qt::NoContextMenu);
    auto* const centeringLayout = new QVBoxLayout(bookmarkListViewViewPort);
    centeringLayout->addWidget(m_emptyListOverlayLabel);
    centeringLayout->setAlignment(m_emptyListOverlayLabel, Qt::AlignCenter);
    connect(mTool, &BookmarksTool::hasBookmarksChanged,
            this, &BookmarksView::updateEmptyListOverlayLabel);
    connect(mTool, &BookmarksTool::bookmarksAdded,
            this, &BookmarksView::updateEmptyListOverlayLabel);
    connect(mTool, &BookmarksTool::bookmarksRemoved,
            this, &BookmarksView::updateEmptyListOverlayLabel);

    baseLayout->addWidget(mBookmarkListView, 10);

    // toolbar TODO: make this view work like the filebrowser, with actions on top?
    auto* const actionsToolBar = new QToolBar(this);
    actionsToolBar->setToolButtonStyle(Qt::ToolButtonFollowStyle);

    actionsToolBar->addAction(mCreateBookmarkAction);

    auto* const stretcher = new QWidget(this);
    stretcher->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    actionsToolBar->addWidget(stretcher);

    actionsToolBar->addAction(mGotoBookmarkAction);

    auto* const menu = new KActionMenu(QIcon::fromTheme(QStringLiteral("overflow-menu")), QString(), this);
    menu->setPopupMode(QToolButton::InstantPopup);
    menu->addAction(mRenameBookmarkAction);
    menu->addSeparator();
    menu->addAction(mCopyOffsetAction);
    menu->addSeparator();
    menu->addAction(mDeleteBookmarksAction);
    actionsToolBar->addAction(menu);

    baseLayout->addWidget(actionsToolBar);

    // init state
    onBookmarkSelectionChanged();
}

BookmarksView::~BookmarksView() = default;

void BookmarksView::updateEmptyListOverlayLabel()
{
    m_emptyListOverlayLabel->setVisible(mTool->isBookmarkListEmpty());
}

void BookmarksView::onBookmarkDoubleClicked(const QModelIndex& index)
{
    const QModelIndex bookmarkIndex = m_sortProxyModel->mapToSource(index);
    const int column = bookmarkIndex.column();
    const bool isOffsetColum = (column == BookmarkListModel::OffsetColumnId);
    if (isOffsetColum) {
        mTool->gotoBookmark(mBookmarkListModel->bookmark(bookmarkIndex));
    }
}

void BookmarksView::onCustomContextMenuRequested(QPoint pos)
{
    auto* const menu = new QMenu(this);
    menu->setAttribute(Qt::WA_DeleteOnClose);

    const QModelIndex index = mBookmarkListView->indexAt(pos);

    if (index.isValid()) {
        const QModelIndexList selectedRows = mBookmarkListView->selectionModel()->selectedRows();
        const bool isOneSelected = (selectedRows.size() == 1);

        if (isOneSelected) {
            menu->addAction(mGotoBookmarkAction);
            menu->addAction(mRenameBookmarkAction);
            menu->addSeparator();
            menu->addAction(mCopyOffsetAction);
            menu->addSeparator();
        }
        menu->addAction(mDeleteBookmarksAction);
    } else {
        menu->addAction(mCreateBookmarkAction);
    }

    menu->popup(mBookmarkListView->viewport()->mapToGlobal(pos));
}

void BookmarksView::onBookmarkSelectionChanged()
{
    const QItemSelectionModel* const selectionModel = mBookmarkListView->selectionModel();

    // TODO: selectionModel->selectedIndexes() is a expensive operation,
    // but with Qt 4.4.3 hasSelection() has the flaw to return true with a current index
    const QModelIndexList selectedRows = selectionModel->selectedRows();
    const bool hasSelection = !selectedRows.isEmpty();
    mDeleteBookmarksAction->setEnabled(hasSelection);
    const bool bookmarkSelected =
        (selectedRows.size() == 1) &&
        selectionModel->isSelected(selectionModel->currentIndex());
    mGotoBookmarkAction->setEnabled(bookmarkSelected);
    mRenameBookmarkAction->setEnabled(bookmarkSelected);
    mCopyOffsetAction->setEnabled(bookmarkSelected);
}

void BookmarksView::onCreateBookmarkButtonClicked()
{
    const Okteta::Bookmark bookmark = mTool->createBookmark();
    if (bookmark.isValid()) {
        const QModelIndex bookmarkIndex = mBookmarkListModel->index(bookmark, BookmarkListModel::TitleColumnId);
        const QModelIndex sortedIndex = m_sortProxyModel->mapFromSource(bookmarkIndex);
        if (sortedIndex.isValid()) {
            mBookmarkListView->edit(sortedIndex);
        }
    }
}

void BookmarksView::onDeleteBookmarkButtonClicked()
{
    const QModelIndexList selectedRows = mBookmarkListView->selectionModel()->selectedRows();

    QList<Okteta::Bookmark> bookmarksToBeDeleted;
    bookmarksToBeDeleted.reserve(selectedRows.size());
    for (const QModelIndex& sortedIndex : selectedRows) {
        const QModelIndex bookmarkIndex = m_sortProxyModel->mapToSource(sortedIndex);
        const Okteta::Bookmark& bookmark = mBookmarkListModel->bookmark(bookmarkIndex);
        bookmarksToBeDeleted.append(bookmark);
    }

    mTool->deleteBookmarks(bookmarksToBeDeleted);
}

void BookmarksView::onGotoBookmarkButtonClicked()
{
    const QModelIndex sortedIndex = mBookmarkListView->selectionModel()->currentIndex();
    const QModelIndex bookmarkIndex = m_sortProxyModel->mapToSource(sortedIndex);
    if (bookmarkIndex.isValid()) {
        mTool->gotoBookmark(mBookmarkListModel->bookmark(bookmarkIndex));
    }
}

void BookmarksView::onRenameBookmarkButtonClicked()
{
    const QModelIndex index = mBookmarkListView->selectionModel()->currentIndex();
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
