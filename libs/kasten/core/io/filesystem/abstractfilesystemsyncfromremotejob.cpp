/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008-2009, 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "abstractfilesystemsyncfromremotejob.hpp"
#include "abstractfilesystemsyncfromremotejob_p.hpp"

namespace Kasten {

AbstractFileSystemSyncFromRemoteJob::AbstractFileSystemSyncFromRemoteJob(AbstractModelFileSystemSynchronizer* synchronizer)
    : AbstractSyncFromRemoteJob(new AbstractFileSystemSyncFromRemoteJobPrivate(this, synchronizer))
{
}

AbstractFileSystemSyncFromRemoteJob::~AbstractFileSystemSyncFromRemoteJob() = default;

AbstractModelFileSystemSynchronizer* AbstractFileSystemSyncFromRemoteJob::synchronizer() const
{
    Q_D(const AbstractFileSystemSyncFromRemoteJob);

    return d->synchronizer();
}
QFile* AbstractFileSystemSyncFromRemoteJob::file() const
{
    Q_D(const AbstractFileSystemSyncFromRemoteJob);

    return d->file();
}

void AbstractFileSystemSyncFromRemoteJob::start()
{
    Q_D(AbstractFileSystemSyncFromRemoteJob);

    d->start();
}

void AbstractFileSystemSyncFromRemoteJob::completeRead(bool success)
{
    Q_D(AbstractFileSystemSyncFromRemoteJob);

    d->completeRead(success);
}

}

// have to include this because of Q_PRIVATE_SLOT
#include "moc_abstractfilesystemsyncfromremotejob.cpp"
