/*
    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "filesystembrowserview.hpp"

// lib
#include "filesystembrowsertool.hpp"
// KF
#include <KUrlNavigator>
#include <KFilePlacesModel>
#include <KDirOperator>
#include <KActionCollection>
#include <KToolBar>
#include <KLocalizedString>
// Qt
#include <QVBoxLayout>
#include <QUrl>
#include <QDir>

namespace Kasten {

FileSystemBrowserView::FileSystemBrowserView(FileSystemBrowserTool* tool, QWidget* parent)
    : QWidget(parent)
    , mTool(tool)
{
    QMetaObject::invokeMethod(this, "init", Qt::QueuedConnection);
}

FileSystemBrowserView::~FileSystemBrowserView() = default;

void FileSystemBrowserView::init()
{
    auto* const layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    // tool bar
    mToolbar = new KToolBar(this);
    mToolbar->setMovable(false);
    mToolbar->setToolButtonStyle(Qt::ToolButtonIconOnly);
    mToolbar->setIconDimensions(16);
    mToolbar->setContextMenuPolicy(Qt::NoContextMenu);
    layout->addWidget(mToolbar);

    // url bar
    auto* const filePlacesModel = new KFilePlacesModel(this);
    mUrlNavigator = new KUrlNavigator(filePlacesModel, QUrl::fromLocalFile(QDir::homePath()), this);
    connect(mUrlNavigator, &KUrlNavigator::urlChanged, this, &FileSystemBrowserView::setDirOperatorUrl);
    layout->addWidget(mUrlNavigator);

    // view
    mDirOperator = new KDirOperator(QUrl::fromLocalFile(QDir::homePath()), this);
    mDirOperator->setViewMode(KFile::Detail);
    connect(mDirOperator, &KDirOperator::urlEntered, this, &FileSystemBrowserView::setNavigatorUrl);
    connect(mDirOperator, &KDirOperator::fileSelected, this, &FileSystemBrowserView::openFile);
    layout->addWidget(mDirOperator);

    // fill toolbar
    constexpr KDirOperator::Action toolbarActions[] = {
        KDirOperator::Back,
        KDirOperator::Forward,
        KDirOperator::Up,
        KDirOperator::Home,
        KDirOperator::ShortView,
        KDirOperator::DetailedView,
        KDirOperator::TreeView,
    };

    for (const KDirOperator::Action toolbarAction : toolbarActions) {
        QAction* const action = mDirOperator->action(toolbarAction);
        if (action) {
            mToolbar->addAction(action);
        }
    }

    mActionCollection = new KActionCollection(this);
    QAction* const syncDirAction = mActionCollection->addAction(QStringLiteral("sync_dir"),
                                                                this, &FileSystemBrowserView::syncCurrentDocumentDirectory);
    syncDirAction->setIcon(QIcon::fromTheme(QStringLiteral("go-parent-folder")));
    syncDirAction->setText(i18nc("@action:intoolbar", "Folder of Current Document"));
    connect(mTool, &FileSystemBrowserTool::hasCurrentUrlChanged, syncDirAction, &QAction::setEnabled);
    syncDirAction->setEnabled(mTool->hasCurrentUrl());
    mToolbar->addAction(syncDirAction);
}

void FileSystemBrowserView::setDirOperatorUrl(const QUrl& url)
{
    mDirOperator->setUrl(url, true);
}

void FileSystemBrowserView::setNavigatorUrl(const QUrl& url)
{
    mUrlNavigator->setLocationUrl(url);
}

void FileSystemBrowserView::syncCurrentDocumentDirectory()
{
    const QUrl url = mTool->currentUrl();

    if (!url.isEmpty()) {
        setNavigatorUrl(url);
    }
}

void FileSystemBrowserView::openFile(const KFileItem& fileItem)
{
    mTool->open(fileItem.url());
}

}

#include "moc_filesystembrowserview.cpp"
