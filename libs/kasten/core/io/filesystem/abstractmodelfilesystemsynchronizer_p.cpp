/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2007-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "abstractmodelfilesystemsynchronizer_p.hpp"

// lib
#include <logging.hpp>
// KF
#include <KDirWatch>

namespace Kasten {

AbstractModelFileSystemSynchronizerPrivate::~AbstractModelFileSystemSynchronizerPrivate() = default;

void AbstractModelFileSystemSynchronizerPrivate::startFileWatching()
{
    Q_Q(AbstractModelFileSystemSynchronizer);

    if (!mDirWatch) {
        mDirWatch = new KDirWatch(q);
        QObject::connect(mDirWatch, &KDirWatch::dirty,
                         q, [&](const QString& path) { onFileDirty(path); });

        QObject::connect(mDirWatch, &KDirWatch::created,
                         q, [&](const QString& path) { onFileCreated(path); });

        QObject::connect(mDirWatch, &KDirWatch::deleted,
                         q, [&](const QString& path) { onFileDeleted(path); });
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

    // Silence deprecation warnings as there is no Qt 5 substitute for QNetworkConfigurationManager
    QT_WARNING_PUSH
    QT_WARNING_DISABLE_CLANG("-Wdeprecated-declarations")
    QT_WARNING_DISABLE_GCC("-Wdeprecated-declarations")
    mNetworkConfigurationManager = std::make_unique<QNetworkConfigurationManager>();
    QObject::connect(mNetworkConfigurationManager.get(), &QNetworkConfigurationManager::onlineStateChanged,
                     q, [&](bool online) { onOnlineStateChanged(online); });
    QT_WARNING_POP
}
void AbstractModelFileSystemSynchronizerPrivate::stopNetworkWatching()
{
    mNetworkConfigurationManager.reset();
}

void AbstractModelFileSystemSynchronizerPrivate::onFileDirty(const QString& fileName)
{
    Q_UNUSED(fileName)
    qCDebug(LOG_KASTEN_CORE) << fileName;
    setRemoteState(RemoteHasChanges);
}

void AbstractModelFileSystemSynchronizerPrivate::onFileCreated(const QString& fileName)
{
    Q_UNUSED(fileName)
    qCDebug(LOG_KASTEN_CORE) << fileName;
    // TODO: could happen after a delete, what to do?
    setRemoteState(RemoteHasChanges);
}

void AbstractModelFileSystemSynchronizerPrivate::onFileDeleted(const QString& fileName)
{
    Q_UNUSED(fileName)
    qCDebug(LOG_KASTEN_CORE) << fileName;
    setRemoteState(RemoteDeleted);
}

void AbstractModelFileSystemSynchronizerPrivate::onOnlineStateChanged(bool isOnline)
{
    qCDebug(LOG_KASTEN_CORE);
    setRemoteState(isOnline ? RemoteUnknown : RemoteUnreachable);
}

}
