/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2009-2010,2012 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "modifiedbarcontroller.hpp"

// Kasten ui
#include <kasten/statusbar.hpp>
// Kasten core
#include <kasten/abstractmodelsynchronizer.hpp>
#include <kasten/abstractdocument.hpp>
// KF5
#include <KLocalizedString>
// Qt
#include <QLabel>
#include <QIcon>

namespace Kasten {
static const int modifiedPixmapWidth = 16;

ModifiedBarController::ModifiedBarController(StatusBar* statusBar)
{
    // TODO: depend an statusbar height
    const QSize modifiedPixmapSize = QSize(modifiedPixmapWidth, modifiedPixmapWidth);

    mLocalStateLabel = new QLabel(statusBar);
    mLocalStateLabel->setAlignment(Qt::AlignCenter);
    mLocalStateLabel->setFixedSize(modifiedPixmapSize);
    statusBar->addWidget(mLocalStateLabel);

    mRemoteStateLabel = new QLabel(statusBar);
    mRemoteStateLabel->setAlignment(Qt::AlignCenter);
    mRemoteStateLabel->setFixedSize(modifiedPixmapSize);
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

    mLocalStateLabel->setEnabled(mDocument);
    mRemoteStateLabel->setEnabled(mDocument);

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

    // TODO: depend an statusbar height
    const QPixmap pixmap = isModified ?
                           QIcon::fromTheme(QStringLiteral("document-save")).pixmap(modifiedPixmapWidth) :
                           QPixmap();
    mLocalStateLabel->setPixmap(pixmap);

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

    // TODO: depend an statusbar height
    const QPixmap pixmap = iconName ?
                           QIcon::fromTheme(QLatin1String(iconName)).pixmap(modifiedPixmapWidth) :
                           QPixmap();
    mRemoteStateLabel->setPixmap(pixmap);

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
