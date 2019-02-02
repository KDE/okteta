/*
    This file is part of the Kasten Framework, made within the KDE community.

    Copyright 2008-2009,2011 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef ABSTRACTFILESYSTEMSYNCFROMREMOTEJOB_P_HPP
#define ABSTRACTFILESYSTEMSYNCFROMREMOTEJOB_P_HPP

// library
#include "abstractfilesystemsyncfromremotejob.hpp"
#include <abstractsyncfromremotejob_p.hpp>
// KF5
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
    Q_DECLARE_PUBLIC(AbstractFileSystemSyncFromRemoteJob)

protected:
    AbstractModelFileSystemSynchronizer* mSynchronizer;
    QString mWorkFilePath;
    QString mTempFilePath;
    QFile* mFile = nullptr;
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
