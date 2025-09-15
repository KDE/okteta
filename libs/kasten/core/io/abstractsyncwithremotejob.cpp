/*
    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "abstractsyncwithremotejob.hpp"
#include "abstractsyncwithremotejob_p.hpp"

namespace Kasten {

AbstractSyncWithRemoteJob::AbstractSyncWithRemoteJob(std::unique_ptr<AbstractSyncWithRemoteJobPrivate>&& dd)
    : d_ptr(std::move(dd))
{
}

AbstractSyncWithRemoteJob::AbstractSyncWithRemoteJob()
    : d_ptr(std::make_unique<AbstractSyncWithRemoteJobPrivate>(this))
{}

AbstractSyncWithRemoteJob::~AbstractSyncWithRemoteJob() = default;

}

#include "moc_abstractsyncwithremotejob.cpp"
