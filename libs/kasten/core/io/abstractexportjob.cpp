/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "abstractexportjob.hpp"
#include "abstractexportjob_p.hpp"

namespace Kasten {

AbstractExportJob::AbstractExportJob(AbstractExportJobPrivate* d)
    : d_ptr(d)
{}

AbstractExportJob::AbstractExportJob()
    : d_ptr(new AbstractExportJobPrivate(this))
{}

AbstractExportJob::~AbstractExportJob() = default;

AbstractDocument* AbstractExportJob::document() const
{
    Q_D(const AbstractExportJob);

    return d->document();
}

void AbstractExportJob::setDocument(AbstractDocument* document)
{
    Q_D(AbstractExportJob);

    d->setDocument(document);
}

}
