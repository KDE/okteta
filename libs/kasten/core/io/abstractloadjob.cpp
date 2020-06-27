/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "abstractloadjob.hpp"
#include "abstractloadjob_p.hpp"

namespace Kasten {

AbstractLoadJob::AbstractLoadJob(AbstractLoadJobPrivate* d)
    : d_ptr(d)
{
}

AbstractLoadJob::AbstractLoadJob()
    : d_ptr(new AbstractLoadJobPrivate(this))
{
}

AbstractLoadJob::~AbstractLoadJob() = default;

AbstractDocument* AbstractLoadJob::document() const
{
    Q_D(const AbstractLoadJob);

    return d->document();
}

void AbstractLoadJob::setDocument(AbstractDocument* document)
{
    Q_D(AbstractLoadJob);

    d->setDocument(document);
}

}
