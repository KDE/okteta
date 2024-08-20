/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "abstractsyncfromremotejob.hpp"
#include "abstractsyncfromremotejob_p.hpp"

namespace Kasten {

AbstractSyncFromRemoteJob::AbstractSyncFromRemoteJob(std::unique_ptr<AbstractSyncFromRemoteJobPrivate>&& dd)
    : d_ptr(std::move(dd))
{
}

AbstractSyncFromRemoteJob::AbstractSyncFromRemoteJob()
    : d_ptr(std::make_unique<AbstractSyncFromRemoteJobPrivate>(this))
{
}

AbstractSyncFromRemoteJob::~AbstractSyncFromRemoteJob() = default;

}

#include "moc_abstractsyncfromremotejob.cpp"
