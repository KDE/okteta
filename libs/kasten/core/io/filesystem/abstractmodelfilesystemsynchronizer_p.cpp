/*
    SPDX-FileCopyrightText: 2007-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "abstractmodelfilesystemsynchronizer_p.hpp"

// lib
#include <logging.hpp>
// KF
#include <KDirWatch>
// Qt
#include <QNetworkInformation>

namespace Kasten {

AbstractModelFileSystemSynchronizerPrivate::~AbstractModelFileSystemSynchronizerPrivate() = default;

void AbstractModelFileSystemSynchronizerPrivate::startFileWatching()
{
    Q_Q(AbstractModelFileSystemSynchronizer);

    if (!mDirWatch) {
        mDirWatch = new KDirWatch(q);
        QObject::connect(mDirWatch, &KDirWatch::dirty,
                         q, [this](const QString& path) { onFileDirty(path); });

        QObject::connect(mDirWatch, &KDirWatch::created,
                         q, [this](const QString& path) { onFileCreated(path); });

        QObject::connect(mDirWatch, &KDirWatch::deleted,
                         q, [this](const QString& path) { onFileDeleted(path); });
    }

    mDirWatch->addFile(mUrl.toLocalFile());
}

void AbstractModelFileSystemSynchronizerPrivate::stopFileWatching()
{
    if (!mDirWatch) {
        return;
    }

    mDirWatch->removeFile(mUrl.toLocalFile());
}

void AbstractModelFileSystemSynchronizerPrivate::pauseFileWatching()
{
    if (!mDirWatch) {
        return;
    }

    mDirWatch->stopScan();
}

void AbstractModelFileSystemSynchronizerPrivate::unpauseFileWatching()
{
    if (!mDirWatch) {
        return;
    }

    mDirWatch->startScan();
}

void AbstractModelFileSystemSynchronizerPrivate::startNetworkWatching()
{
    Q_Q(AbstractModelFileSystemSynchronizer);

    QNetworkInformation::loadBackendByFeatures(QNetworkInformation::Feature::Reachability);
    mReachabilityChangedConnection =
    QObject::connect(QNetworkInformation::instance(), &QNetworkInformation::reachabilityChanged,
                     q, [this](QNetworkInformation::Reachability reachability) { onOnlineStateChanged(reachability == QNetworkInformation::Reachability::Online); });
}

void AbstractModelFileSystemSynchronizerPrivate::stopNetworkWatching()
{
    QObject::disconnect(mReachabilityChangedConnection);
}

void AbstractModelFileSystemSynchronizerPrivate::onFileDirty(const QString& fileName)
{
    Q_UNUSED(fileName)
    setRemoteState(RemoteHasChanges);
}

void AbstractModelFileSystemSynchronizerPrivate::onFileCreated(const QString& fileName)
{
    Q_UNUSED(fileName)
    // TODO: could happen after a delete, what to do?
    setRemoteState(RemoteHasChanges);
}

void AbstractModelFileSystemSynchronizerPrivate::onFileDeleted(const QString& fileName)
{
    Q_UNUSED(fileName)
    setRemoteState(RemoteDeleted);
}

void AbstractModelFileSystemSynchronizerPrivate::onOnlineStateChanged(bool isOnline)
{
    qCDebug(LOG_KASTEN_CORE);
    setRemoteState(isOnline ? RemoteUnknown : RemoteUnreachable);
}

}
