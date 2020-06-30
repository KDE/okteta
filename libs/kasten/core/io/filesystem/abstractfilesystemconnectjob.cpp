/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008-2009, 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "abstractfilesystemconnectjob.hpp"
#include "abstractfilesystemconnectjob_p.hpp"

namespace Kasten {

AbstractFileSystemConnectJob::AbstractFileSystemConnectJob(AbstractModelFileSystemSynchronizer* synchronizer,
                                                           AbstractDocument* document,
                                                           const QUrl& url, AbstractModelSynchronizer::ConnectOption option)
    : AbstractConnectJob(new AbstractFileSystemConnectJobPrivate(this, synchronizer, document, url, option))
{
}

AbstractFileSystemConnectJob::~AbstractFileSystemConnectJob() = default;

AbstractModelFileSystemSynchronizer* AbstractFileSystemConnectJob::synchronizer() const
{
    Q_D(const AbstractFileSystemConnectJob);

    return d->synchronizer();
}
AbstractDocument* AbstractFileSystemConnectJob::document() const
{
    Q_D(const AbstractFileSystemConnectJob);

    return d->document();
}
QFile* AbstractFileSystemConnectJob::file() const
{
    Q_D(const AbstractFileSystemConnectJob);

    return d->file();
}

void AbstractFileSystemConnectJob::start()
{
    Q_D(AbstractFileSystemConnectJob);

    d->start();
}

void AbstractFileSystemConnectJob::complete(bool success)
{
    Q_D(AbstractFileSystemConnectJob);

    d->complete(success);
}

}

// have to include this because of Q_PRIVATE_SLOT
#include "moc_abstractfilesystemconnectjob.cpp"
