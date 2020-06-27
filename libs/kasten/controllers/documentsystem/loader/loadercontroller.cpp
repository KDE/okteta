/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2006-2008, 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "loadercontroller.hpp"

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
#include <QMimeDatabase>
#include <QApplication>

namespace Kasten {

static constexpr char CreatorConfigGroupId[] = "Recent Files";

LoaderController::LoaderController(AbstractDocumentStrategy* documentStrategy,
                                   KXMLGUIClient* guiClient)
    : mDocumentStrategy(documentStrategy)
{
    auto* openAction = KStandardAction::open(this, &LoaderController::load, this);
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
    auto* dialog = new QFileDialog(QApplication::activeWindow());
    dialog->setAttribute(Qt::WA_DeleteOnClose, true);
    dialog->setFileMode(QFileDialog::ExistingFiles);
    dialog->setMimeTypeFilters(mDocumentStrategy->supportedRemoteTypes());
    connect(dialog, &QFileDialog::urlsSelected, this, &LoaderController::loadUrls);
    dialog->open();
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
