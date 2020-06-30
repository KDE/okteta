/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008-2009, 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ABSTRACTFILESYSTEMSYNCFROMREMOTEJOB_P_HPP
#define KASTEN_ABSTRACTFILESYSTEMSYNCFROMREMOTEJOB_P_HPP

// library
#include "abstractfilesystemsyncfromremotejob.hpp"
#include <abstractsyncfromremotejob_p.hpp>
// KF
#include <QTemporaryFile>

namespace Kasten {

class AbstractModelFileSystemSynchronizer;

class AbstractFileSystemSyncFromRemoteJobPrivate : public AbstractSyncFromRemoteJobPrivate
{
public:
    AbstractFileSystemSyncFromRemoteJobPrivate(AbstractFileSystemSyncFromRemoteJob* parent,
                                               AbstractModelFileSystemSynchronizer* synchronizer);
    AbstractFileSystemSyncFromRemoteJobPrivate() = delete;

    ~AbstractFileSystemSyncFromRemoteJobPrivate() override;

public: // KJob API
    void start();

public:
    AbstractModelFileSystemSynchronizer* synchronizer() const;
    QFile* file() const;

public:
    void completeRead(bool success);

public:  // slots
    void syncFromRemote();

protected:
    AbstractModelFileSystemSynchronizer* mSynchronizer;
    QString mWorkFilePath;
    QString mTempFilePath;
    QFile* mFile = nullptr;

private:
    Q_DECLARE_PUBLIC(AbstractFileSystemSyncFromRemoteJob)
};

inline AbstractFileSystemSyncFromRemoteJobPrivate::AbstractFileSystemSyncFromRemoteJobPrivate(AbstractFileSystemSyncFromRemoteJob* parent,
                                                                                              AbstractModelFileSystemSynchronizer* synchronizer)
    : AbstractSyncFromRemoteJobPrivate(parent)
    , mSynchronizer(synchronizer)
{
}

inline AbstractFileSystemSyncFromRemoteJobPrivate::~AbstractFileSystemSyncFromRemoteJobPrivate() = default;

inline QFile* AbstractFileSystemSyncFromRemoteJobPrivate::file()     const { return mFile; }
// TODO: setup a notification system
inline AbstractModelFileSystemSynchronizer* AbstractFileSystemSyncFromRemoteJobPrivate::synchronizer() const
{
    return mSynchronizer;
}

inline void AbstractFileSystemSyncFromRemoteJobPrivate::start()
{
    Q_Q(AbstractFileSystemSyncFromRemoteJob);

    QMetaObject::invokeMethod(q, "syncFromRemote", Qt::QueuedConnection);
}

}

#endif
