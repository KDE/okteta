/*
    SPDX-FileCopyrightText: 2008-2009, 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "abstractfilesystemloadjob.hpp"
#include "abstractfilesystemloadjob_p.hpp"

// Std
#include <utility>

namespace Kasten {

AbstractFileSystemLoadJob::AbstractFileSystemLoadJob(std::unique_ptr<AbstractModelFileSystemSynchronizer>&& synchronizer,
                                                     const QUrl& url)
    : AbstractLoadJob(std::make_unique<AbstractFileSystemLoadJobPrivate>(this, std::move(synchronizer), url))
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

void AbstractFileSystemLoadJob::setDocument(std::unique_ptr<AbstractDocument>&& document)
{
    Q_D(AbstractFileSystemLoadJob);

    d->setDocument(std::move(document));
}

}

// have to include this because of Q_PRIVATE_SLOT
#include "moc_abstractfilesystemloadjob.cpp"
