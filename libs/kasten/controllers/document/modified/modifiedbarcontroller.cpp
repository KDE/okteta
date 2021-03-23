/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009-2010, 2012 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "modifiedbarcontroller.hpp"

// controller
#include "iconlabel.hpp"
// Kasten ui
#include <Kasten/StatusBar>
// Kasten core
#include <Kasten/AbstractModelSynchronizer>
#include <Kasten/AbstractDocument>
// KF
#include <KLocalizedString>
// Qt
#include <QIcon>

namespace Kasten {
ModifiedBarController::ModifiedBarController(StatusBar* statusBar)
{
    mLocalStateLabel = new IconLabel(statusBar);
    statusBar->addWidget(mLocalStateLabel);

    mRemoteStateLabel = new IconLabel(statusBar);
    statusBar->addWidget(mRemoteStateLabel);

    setTargetModel(nullptr);
}

void ModifiedBarController::setTargetModel(AbstractModel* model)
{
    AbstractDocument* newDocument = model ? model->findBaseModel<AbstractDocument*>() : nullptr;

    if (mDocument == newDocument) {
        return;
    }

    if (mDocument) {
        mDocument->disconnect(this);
    }

    mDocument = newDocument;

    if (mDocument) {
        connect(mDocument, &Kasten::AbstractDocument::synchronizerChanged,
                this,      &ModifiedBarController::onSynchronizerChanged);
    }

    mLocalStateLabel->setEnabled(mDocument != nullptr);
    mRemoteStateLabel->setEnabled(mDocument != nullptr);

    onSynchronizerChanged(mDocument ? mDocument->synchronizer() : nullptr);
}

void ModifiedBarController::onContentFlagsChanged(ContentFlags contentFlags)
{
    const bool hasChanges = (contentFlags & ContentHasUnstoredChanges);
    onLocalSyncStateChanged(hasChanges ? LocalHasChanges : LocalInSync);
}

void ModifiedBarController::onLocalSyncStateChanged(LocalSyncState localSyncState)
{
    const bool isModified = (localSyncState == LocalHasChanges);

    const QIcon icon = isModified ? QIcon::fromTheme(QStringLiteral("document-save")) : QIcon();
    mLocalStateLabel->setIcon(icon);

    mLocalStateLabel->setToolTip(isModified ?
                                 i18nc("@tooltip the document is modified", "Modified.") :
                                 i18nc("@tooltip the document is not modified", "Not modified."));

}

void ModifiedBarController::onRemoteSyncStateChanged(RemoteSyncState remoteSyncState)
{
    const char* const iconName =
        (!mSynchronizer) ?                       "document-new" :
        (remoteSyncState == RemoteHasChanges) ?  "document-save" :
        (remoteSyncState == RemoteDeleted) ?     "edit-delete" :
        (remoteSyncState == RemoteUnknown) ?     "flag-yellow" :
        (remoteSyncState == RemoteUnreachable) ? "network-disconnect" :
        /* else */ nullptr;

    const QIcon icon = iconName ? QIcon::fromTheme(QLatin1String(iconName)) : QIcon();
    mRemoteStateLabel->setIcon(icon);

    // TODO: tooltips
}

void ModifiedBarController::onSynchronizerChanged(AbstractModelSynchronizer* newSynchronizer)
{
    if (mSynchronizer) {
        mSynchronizer->disconnect(this);
    }

    AbstractModelSynchronizer* oldSynchronizer = mSynchronizer;
    mSynchronizer = newSynchronizer;

    LocalSyncState localState;
    RemoteSyncState remoteState;
    if (mSynchronizer) {
        if (!oldSynchronizer) {
            disconnect(mDocument, &Kasten::AbstractDocument::contentFlagsChanged,
                       this, &ModifiedBarController::onContentFlagsChanged);
        }

        localState = mSynchronizer->localSyncState();
        remoteState = mSynchronizer->remoteSyncState();

        connect(mSynchronizer, &Kasten::AbstractModelSynchronizer::localSyncStateChanged,
                this, &ModifiedBarController::onLocalSyncStateChanged);
        connect(mSynchronizer, &Kasten::AbstractModelSynchronizer::remoteSyncStateChanged,
                this, &ModifiedBarController::onRemoteSyncStateChanged);
        connect(mSynchronizer, &QObject::destroyed,
                this, &ModifiedBarController::onSynchronizerDeleted);
    } else if (mDocument) {
        const bool hasChanges = (mDocument->contentFlags() & ContentHasUnstoredChanges);
        localState = (hasChanges ? LocalHasChanges : LocalInSync);
        // TODO: onRemoteSyncStateChanged(...) checks for mSynchronizer and ignores this
        remoteState = RemoteInSync;

        connect(mDocument, &Kasten::AbstractDocument::contentFlagsChanged,
                this, &ModifiedBarController::onContentFlagsChanged);
    } else {
        localState = LocalInSync;
        // TODO: onRemoteSyncStateChanged(...) checks for mSynchronizer and ignores this
        remoteState = RemoteInSync;
    }

    onLocalSyncStateChanged(localState);
    onRemoteSyncStateChanged(remoteState);
}

void ModifiedBarController::onSynchronizerDeleted(QObject* synchronizer)
{
    if (synchronizer != mSynchronizer) {
        return;
    }

    mSynchronizer = nullptr;

    // switch to document state
    connect(mDocument, &Kasten::AbstractDocument::contentFlagsChanged,
            this, &ModifiedBarController::onContentFlagsChanged);

    onContentFlagsChanged(mDocument->contentFlags());
    // TODO: onRemoteSyncStateChanged(...) checks for mSynchronizer and ignores the parameter
    onRemoteSyncStateChanged(RemoteInSync);
}

}
