/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2007-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ABSTRACTMODELFILESYSTEMSYNCHRONIZER_P_HPP
#define KASTEN_ABSTRACTMODELFILESYSTEMSYNCHRONIZER_P_HPP

#include "abstractmodelfilesystemsynchronizer.hpp"

// lib
#include <abstractmodelsynchronizer_p.hpp>
// Qt
#include <QDateTime>

class QNetworkConfigurationManager;

namespace Kasten {

class AbstractModelFileSystemSynchronizerPrivate : public AbstractModelSynchronizerPrivate
{
public:
    explicit AbstractModelFileSystemSynchronizerPrivate(AbstractModelFileSystemSynchronizer* parent);
    AbstractModelFileSystemSynchronizerPrivate() = delete;

    ~AbstractModelFileSystemSynchronizerPrivate() override;

public:
    RemoteSyncState remoteSyncState() const;

public:
    void setRemoteState(RemoteSyncState remoteState);
    void setFileDateTimeOnSync(const QDateTime& fileDateTime);

    void startFileWatching();
    void stopFileWatching();
    void pauseFileWatching();
    void unpauseFileWatching();

    void startNetworkWatching();
    void stopNetworkWatching();

public:
    void onFileDirty(const QString& fileName);
    void onFileCreated(const QString& fileName);
    void onFileDeleted(const QString& fileName);
    void onOnlineStateChanged(bool isOnline);

protected:
    QDateTime mFileDateTime;
    RemoteSyncState mRemoteState = RemoteUnknown;
    QNetworkConfigurationManager* mNetworkConfigurationManager = nullptr;
    mutable KDirWatch* mDirWatch = nullptr;

private:
    Q_DECLARE_PUBLIC(AbstractModelFileSystemSynchronizer)
};

inline AbstractModelFileSystemSynchronizerPrivate::AbstractModelFileSystemSynchronizerPrivate(AbstractModelFileSystemSynchronizer* parent)
    : AbstractModelSynchronizerPrivate(parent)
{
}
inline RemoteSyncState AbstractModelFileSystemSynchronizerPrivate::remoteSyncState() const { return mRemoteState; }

inline void AbstractModelFileSystemSynchronizerPrivate::setRemoteState(RemoteSyncState remoteState)
{
    Q_Q(AbstractModelFileSystemSynchronizer);

    if (mRemoteState == remoteState) {
        return;
    }

    mRemoteState = remoteState;
    emit q->remoteSyncStateChanged(remoteState);
}
inline void AbstractModelFileSystemSynchronizerPrivate::setFileDateTimeOnSync(const QDateTime& fileDateTime)
{
    mFileDateTime = fileDateTime;
}

}

#endif
