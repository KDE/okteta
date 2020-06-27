/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2007-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "synchronizecontroller.hpp"

// Kasten core
#include <Kasten/DocumentSyncManager>
#include <Kasten/AbstractModelFileSystemSynchronizer>
#include <Kasten/AbstractDocument>
// KF
#include <KActionCollection>
#include <KStandardAction>
#include <KXMLGUIClient>
#include <KLocalizedString>
// Qt
#include <QUrl>
#include <QAction>

namespace Kasten {

SynchronizeController::SynchronizeController(DocumentSyncManager* syncManager, KXMLGUIClient* guiClient)
    : mSyncManager(syncManager)
{
    KActionCollection* actionCollection = guiClient->actionCollection();

    mSaveAction = KStandardAction::save(this, &SynchronizeController::save, this);

    mReloadAction = new QAction(QIcon::fromTheme(QStringLiteral("view-refresh")),
                                i18nc("@action:inmenu", "Reloa&d"), this);
    actionCollection->setDefaultShortcuts(mReloadAction, KStandardShortcut::reload());
    connect(mReloadAction, &QAction::triggered,
            this, &SynchronizeController::reload);

    actionCollection->addAction(mSaveAction->objectName(), mSaveAction);
    actionCollection->addAction(QStringLiteral("file_reload"), mReloadAction);

    setTargetModel(nullptr);
}

void SynchronizeController::setTargetModel(AbstractModel* model)
{
    if (mDocument) {
        mDocument->disconnect(this);
    }

    mDocument = model ? model->findBaseModel<AbstractDocument*>() : nullptr;

    if (mDocument) {
        connect(mDocument, &AbstractDocument::synchronizerChanged,
                this, &SynchronizeController::onSynchronizerChanged);
    }
    onSynchronizerChanged(mDocument ? mDocument->synchronizer() : nullptr);
}

void SynchronizeController::save()
{
    mSyncManager->save(mDocument);
}

void SynchronizeController::reload()
{
    mSyncManager->reload(mDocument);
}

void SynchronizeController::onSynchronizerChanged(AbstractModelSynchronizer* newSynchronizer)
{
    if (mSynchronizer) {
        mSynchronizer->disconnect(this);
    }

    mSynchronizer = qobject_cast<AbstractModelFileSystemSynchronizer*>(newSynchronizer);
    // TODO: Storable interface should be used by Synchronizer
    // synchronizer should report about possible activities
    // TODO: check for access rights, may not write
    bool canSync = false;
    if (mSynchronizer) {
        const LocalSyncState localSyncState = mSynchronizer->localSyncState();
        const RemoteSyncState remoteSyncState = mSynchronizer->remoteSyncState();
        canSync = (localSyncState == LocalHasChanges)
                  || (remoteSyncState == RemoteHasChanges)
                  || (remoteSyncState == RemoteUnknown);

        connect(mSynchronizer, &AbstractModelSynchronizer::localSyncStateChanged,
                this, &SynchronizeController::onSyncStateChanged);
        connect(mSynchronizer, &AbstractModelSynchronizer::remoteSyncStateChanged,
                this, &SynchronizeController::onSyncStateChanged);
        connect(mSynchronizer, &QObject::destroyed,
                this, &SynchronizeController::onSynchronizerDeleted);
    }

    mSaveAction->setEnabled(canSync);
    mReloadAction->setEnabled(canSync);
}

void SynchronizeController::onSynchronizerDeleted(QObject* synchronizer)
{
    if (synchronizer != mSynchronizer) {
        return;
    }

    mSynchronizer = nullptr;

    mSaveAction->setEnabled(false);
    mReloadAction->setEnabled(false);
}

void SynchronizeController::onSyncStateChanged()
{
    const LocalSyncState localSyncState = mSynchronizer->localSyncState();
    const RemoteSyncState remoteSyncState = mSynchronizer->remoteSyncState();
    const bool canSync = (localSyncState == LocalHasChanges)
                         || (remoteSyncState == RemoteHasChanges)
                         || (remoteSyncState == RemoteUnknown);

    mSaveAction->setEnabled(canSync);
    mReloadAction->setEnabled(canSync);
}

}
