/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008-2009, 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "abstractfilesystemsynctoremotejob.hpp"
#include "abstractfilesystemsynctoremotejob_p.hpp"

namespace Kasten {

AbstractFileSystemSyncToRemoteJob::AbstractFileSystemSyncToRemoteJob(AbstractModelFileSystemSynchronizer* synchronizer)
    : AbstractSyncToRemoteJob(new AbstractFileSystemSyncToRemoteJobPrivate(this, synchronizer))
{
}

AbstractFileSystemSyncToRemoteJob::~AbstractFileSystemSyncToRemoteJob() = default;

AbstractModelFileSystemSynchronizer* AbstractFileSystemSyncToRemoteJob::synchronizer() const
{
    Q_D(const AbstractFileSystemSyncToRemoteJob);

    return d->synchronizer();
}

QFile* AbstractFileSystemSyncToRemoteJob::file() const
{
    Q_D(const AbstractFileSystemSyncToRemoteJob);

    return d->file();
}

void AbstractFileSystemSyncToRemoteJob::start()
{
    Q_D(AbstractFileSystemSyncToRemoteJob);

    d->start();
}

void AbstractFileSystemSyncToRemoteJob::completeWrite(bool success)
{
    Q_D(AbstractFileSystemSyncToRemoteJob);

    d->completeWrite(success);
}

}

// have to include this because of Q_PRIVATE_SLOT
#include "moc_abstractfilesystemsynctoremotejob.cpp"
