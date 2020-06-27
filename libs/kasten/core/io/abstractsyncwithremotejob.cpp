/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "abstractsyncwithremotejob.hpp"
#include "abstractsyncwithremotejob_p.hpp"

namespace Kasten {

AbstractSyncWithRemoteJob::AbstractSyncWithRemoteJob(AbstractSyncWithRemoteJobPrivate* d)
    : d_ptr(d)
{
}

AbstractSyncWithRemoteJob::AbstractSyncWithRemoteJob()
    : d_ptr(new AbstractSyncWithRemoteJobPrivate(this))
{}

AbstractSyncWithRemoteJob::~AbstractSyncWithRemoteJob() = default;

}
