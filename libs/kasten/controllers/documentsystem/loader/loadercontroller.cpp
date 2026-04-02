/*
    SPDX-FileCopyrightText: 2006-2008, 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "loadercontroller.hpp"

// Workaround for QTBUG-96157
// QFileDialog's default implementation misses to emit QFileDialog::urlsSelected,
// so if the Qt platform integration does not provide a native file picker dialog,
// there is no option to get async QUrl results.
// There is also no way to defined way to query if a native dialog is used.
// So for now use sync API (as done traditionally by code elsewhere)
#define KASTEN_USE_SYNC_LOAD_FILE_PICKER 1

// Kasten Gui
#include <Kasten/AbstractDocumentStrategy>
// KF
#include <KRecentFilesAction>
#include <KActionCollection>
#include <KStandardAction>
#include <KXMLGUIClient>
#include <KConfigGroup>
#include <KSharedConfig>
// Qt
#include <QFileDialog>
#include <QApplication>
#if KASTEN_USE_SYNC_LOAD_FILE_PICKER
#include <QPointer>
#endif

namespace Kasten {

static constexpr char CreatorConfigGroupId[] = "Recent Files";

LoaderController::LoaderController(AbstractDocumentStrategy* documentStrategy,
                                   KXMLGUIClient* guiClient)
    : mDocumentStrategy(documentStrategy)
{
    QAction* const openAction = KStandardAction::open(this, &LoaderController::load, this);
    mOpenRecentAction = KStandardAction::openRecent(this, &LoaderController::loadRecent, this);

    KActionCollection* const actionCollection = guiClient->actionCollection();
    actionCollection->addAction(openAction->objectName(), openAction);
    actionCollection->addAction(mOpenRecentAction->objectName(), mOpenRecentAction);

    KConfigGroup configGroup(KSharedConfig::openConfig(), CreatorConfigGroupId);
    mOpenRecentAction->loadEntries(configGroup);

    connect(mDocumentStrategy, &AbstractDocumentStrategy::urlUsed, this, &LoaderController::onUrlUsed);
}

LoaderController::~LoaderController()
{
    KConfigGroup configGroup(KSharedConfig::openConfig(), CreatorConfigGroupId);
    mOpenRecentAction->saveEntries(configGroup);
}

void LoaderController::setTargetModel(AbstractModel* model)
{
    Q_UNUSED(model)
}

void LoaderController::load()
{
#if KASTEN_USE_SYNC_LOAD_FILE_PICKER
    QPointer<QFileDialog> dialog = new QFileDialog(QApplication::activeWindow());
#else
    auto* const dialog = new QFileDialog(QApplication::activeWindow());
    dialog->setAttribute(Qt::WA_DeleteOnClose, true);
#endif
    dialog->setFileMode(QFileDialog::ExistingFiles);
    dialog->setMimeTypeFilters(mDocumentStrategy->supportedRemoteTypes());
#if KASTEN_USE_SYNC_LOAD_FILE_PICKER
    if (dialog->exec() == QDialog::Rejected) {
        delete dialog;
        return;
    }

    const QList<QUrl> urls = dialog->selectedUrls();
    delete dialog;

    loadUrls(urls);
#else
    connect(dialog, &QFileDialog::urlsSelected, this, &LoaderController::loadUrls);
    dialog->open();
#endif
}

void LoaderController::loadUrls(const QList<QUrl>& urls)
{
    for (const QUrl& url : urls) {
        mDocumentStrategy->load(url);
    }
}

void LoaderController::loadRecent(const QUrl& url)
{
    mDocumentStrategy->load(url);
}

void LoaderController::onUrlUsed(const QUrl& url)
{
    mOpenRecentAction->addUrl(url);
}

}

#include "moc_loadercontroller.cpp"
