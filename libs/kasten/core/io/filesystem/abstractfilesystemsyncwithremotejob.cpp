/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008-2009, 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "abstractfilesystemsyncwithremotejob.hpp"
#include "abstractfilesystemsyncwithremotejob_p.hpp"

namespace Kasten {

AbstractFileSystemSyncWithRemoteJob::AbstractFileSystemSyncWithRemoteJob(AbstractModelFileSystemSynchronizer* synchronizer,
                                                                         const QUrl& url, AbstractModelSynchronizer::ConnectOption option)
    : AbstractSyncWithRemoteJob(new AbstractFileSystemSyncWithRemoteJobPrivate(this, synchronizer, url, option))
{}

AbstractFileSystemSyncWithRemoteJob::~AbstractFileSystemSyncWithRemoteJob() = default;

AbstractModelFileSystemSynchronizer* AbstractFileSystemSyncWithRemoteJob::synchronizer() const
{
    Q_D(const AbstractFileSystemSyncWithRemoteJob);

    return d->synchronizer();
}
QFile* AbstractFileSystemSyncWithRemoteJob::file() const
{
    Q_D(const AbstractFileSystemSyncWithRemoteJob);

    return d->file();
}

void AbstractFileSystemSyncWithRemoteJob::start()
{
    Q_D(AbstractFileSystemSyncWithRemoteJob);

    d->start();
}

void AbstractFileSystemSyncWithRemoteJob::completeSync(bool success)
{
    Q_D(AbstractFileSystemSyncWithRemoteJob);

    d->completeSync(success);
}

}

// have to include this because of Q_PRIVATE_SLOT
#include "moc_abstractfilesystemsyncwithremotejob.cpp"
