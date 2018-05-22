/*
    This file is part of the Kasten Framework, made within the KDE community.

    Copyright 2006-2008,2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library. If not, see <http://www.gnu.org/licenses/>.
*/

#include "loadercontroller.h"

// Kasten Gui
#include <kasten/abstractdocumentstrategy.h>
// KF5
#include <KRecentFilesAction>
#include <KActionCollection>
#include <KStandardAction>
#include <KXMLGUIClient>
#include <KConfigGroup>
#include <KSharedConfig>
// Qt
#include <QUrl>
#include <QFileDialog>
#include <QMimeDatabase>

namespace Kasten {

static const char CreatorConfigGroupId[] = "Recent Files";

LoaderController::LoaderController(AbstractDocumentStrategy* documentStrategy,
                                   KXMLGUIClient* guiClient)
    : AbstractXmlGuiController()
    , mDocumentStrategy(documentStrategy)
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
    QFileDialog dialog;
    dialog.setMimeTypeFilters(mDocumentStrategy->supportedRemoteTypes());
    if (dialog.exec()) {
        const QList<QUrl> urls = dialog.selectedUrls();

        for (const QUrl& url : urls) {
            mDocumentStrategy->load(url);
        }
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
