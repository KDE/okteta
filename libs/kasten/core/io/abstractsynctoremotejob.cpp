/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "abstractsynctoremotejob.hpp"
#include "abstractsynctoremotejob_p.hpp"

namespace Kasten {

AbstractSyncToRemoteJob::AbstractSyncToRemoteJob(AbstractSyncToRemoteJobPrivate* d)
    : d_ptr(d)
{
}

AbstractSyncToRemoteJob::AbstractSyncToRemoteJob()
    : d_ptr(new AbstractSyncToRemoteJobPrivate(this))
{
}

AbstractSyncToRemoteJob::~AbstractSyncToRemoteJob() = default;

}
