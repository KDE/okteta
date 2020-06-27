/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008-2009, 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "abstractfilesystemexportjob.hpp"
#include "abstractfilesystemexportjob_p.hpp"

namespace Kasten {

AbstractFileSystemExportJob::AbstractFileSystemExportJob(AbstractModel* model, const AbstractModelSelection* selection,
                                                         const QUrl& url)
    : AbstractExportJob(new AbstractFileSystemExportJobPrivate(this, model, selection, url))
{
}

AbstractFileSystemExportJob::AbstractFileSystemExportJob(AbstractFileSystemExportJobPrivate* d)
    : AbstractExportJob(d)
{
}

AbstractFileSystemExportJob::~AbstractFileSystemExportJob() = default;

AbstractModel* AbstractFileSystemExportJob::model() const
{
    Q_D(const AbstractFileSystemExportJob);

    return d->model();
}
const AbstractModelSelection* AbstractFileSystemExportJob::selection() const
{
    Q_D(const AbstractFileSystemExportJob);

    return d->selection();
}
QFile* AbstractFileSystemExportJob::file() const
{
    Q_D(const AbstractFileSystemExportJob);

    return d->file();
}

void AbstractFileSystemExportJob::start()
{
    Q_D(AbstractFileSystemExportJob);

    d->start();
}

void AbstractFileSystemExportJob::completeExport(bool success)
{
    Q_D(AbstractFileSystemExportJob);

    d->completeExport(success);
}

}

// have to include this because of Q_PRIVATE_SLOT
#include "moc_abstractfilesystemexportjob.cpp"
