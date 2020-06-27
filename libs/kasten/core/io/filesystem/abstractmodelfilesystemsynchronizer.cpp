/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2007-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "abstractmodelfilesystemsynchronizer.hpp"
#include "abstractmodelfilesystemsynchronizer_p.hpp"

// lib
#include <abstractdocument.hpp>

namespace Kasten {

AbstractModelFileSystemSynchronizer::AbstractModelFileSystemSynchronizer(AbstractModelFileSystemSynchronizerPrivate* d)
    : AbstractModelSynchronizer(d)
{
}

AbstractModelFileSystemSynchronizer::AbstractModelFileSystemSynchronizer()
    : AbstractModelSynchronizer(new AbstractModelFileSystemSynchronizerPrivate(this))
{}

AbstractModelFileSystemSynchronizer::~AbstractModelFileSystemSynchronizer() = default;

RemoteSyncState AbstractModelFileSystemSynchronizer::remoteSyncState() const
{
    Q_D(const AbstractModelFileSystemSynchronizer);

    return d->remoteSyncState();
}

void AbstractModelFileSystemSynchronizer::setRemoteState(RemoteSyncState remoteState)
{
    Q_D(AbstractModelFileSystemSynchronizer);

    d->setRemoteState(remoteState);
}

void AbstractModelFileSystemSynchronizer::setFileDateTimeOnSync(const QDateTime& fileDateTime)
{
    Q_D(AbstractModelFileSystemSynchronizer);

    d->setFileDateTimeOnSync(fileDateTime);
}

void AbstractModelFileSystemSynchronizer::startFileWatching()
{
    Q_D(AbstractModelFileSystemSynchronizer);

    d->startFileWatching();
}

void AbstractModelFileSystemSynchronizer::stopFileWatching()
{
    Q_D(AbstractModelFileSystemSynchronizer);

    d->stopFileWatching();
}

void AbstractModelFileSystemSynchronizer::pauseFileWatching()
{
    Q_D(AbstractModelFileSystemSynchronizer);

    d->pauseFileWatching();
}

void AbstractModelFileSystemSynchronizer::unpauseFileWatching()
{
    Q_D(AbstractModelFileSystemSynchronizer);

    d->unpauseFileWatching();
}

void AbstractModelFileSystemSynchronizer::startNetworkWatching()
{
    Q_D(AbstractModelFileSystemSynchronizer);

    d->startNetworkWatching();
}

void AbstractModelFileSystemSynchronizer::stopNetworkWatching()
{
    Q_D(AbstractModelFileSystemSynchronizer);

    d->stopNetworkWatching();
}

}
