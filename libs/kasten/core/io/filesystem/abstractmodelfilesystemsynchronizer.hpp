/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2007-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ABSTRACTMODELFILESYSTEMSYNCHRONIZER_HPP
#define KASTEN_ABSTRACTMODELFILESYSTEMSYNCHRONIZER_HPP

// lib
#include "abstractmodelsynchronizer.hpp"

class KDirWatch;
class QDateTime;

namespace Kasten {
class AbstractModelFileSystemSynchronizerPrivate;

class KASTENCORE_EXPORT AbstractModelFileSystemSynchronizer : public AbstractModelSynchronizer
{
    Q_OBJECT

    friend class AbstractFileSystemLoadJobPrivate;
    friend class AbstractFileSystemConnectJobPrivate;
    friend class AbstractFileSystemSyncFromRemoteJobPrivate;
    friend class AbstractFileSystemSyncWithRemoteJobPrivate;
    friend class AbstractFileSystemSyncToRemoteJobPrivate;

protected:
    explicit AbstractModelFileSystemSynchronizer(AbstractModelFileSystemSynchronizerPrivate* d);

public:
    AbstractModelFileSystemSynchronizer();

    ~AbstractModelFileSystemSynchronizer() override;

public: // AbstractModelSynchronizer API
    RemoteSyncState remoteSyncState() const override;

protected:
//     QDataTime lastModifed() const;

protected:
    void setRemoteState(RemoteSyncState remoteState);
    void setFileDateTimeOnSync(const QDateTime& fileDateTime);

    void startFileWatching();
    void stopFileWatching();
    void pauseFileWatching();
    void unpauseFileWatching();

    void startNetworkWatching();
    void stopNetworkWatching();

private:
    Q_DECLARE_PRIVATE(AbstractModelFileSystemSynchronizer)
};

}

#endif
