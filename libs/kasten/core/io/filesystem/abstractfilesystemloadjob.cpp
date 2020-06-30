/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008-2009, 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "abstractfilesystemloadjob.hpp"
#include "abstractfilesystemloadjob_p.hpp"

namespace Kasten {

AbstractFileSystemLoadJob::AbstractFileSystemLoadJob(AbstractModelFileSystemSynchronizer* synchronizer,
                                                     const QUrl& url)
    : AbstractLoadJob(new AbstractFileSystemLoadJobPrivate(this, synchronizer, url))
{
}

AbstractFileSystemLoadJob::~AbstractFileSystemLoadJob() = default;

AbstractModelFileSystemSynchronizer* AbstractFileSystemLoadJob::synchronizer() const
{
    Q_D(const AbstractFileSystemLoadJob);

    return d->synchronizer();
}

QUrl AbstractFileSystemLoadJob::url() const
{
    Q_D(const AbstractFileSystemLoadJob);

    return d->url();
}

QFile* AbstractFileSystemLoadJob::file() const
{
    Q_D(const AbstractFileSystemLoadJob);

    return d->file();
}

void AbstractFileSystemLoadJob::start()
{
    Q_D(AbstractFileSystemLoadJob);

    d->start();
}

void AbstractFileSystemLoadJob::setDocument(AbstractDocument* document)
{
    Q_D(AbstractFileSystemLoadJob);

    d->setDocument(document);
}

}

// have to include this because of Q_PRIVATE_SLOT
#include "moc_abstractfilesystemloadjob.cpp"
