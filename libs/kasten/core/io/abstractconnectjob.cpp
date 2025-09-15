/*
    SPDX-FileCopyrightText: 2008-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "abstractconnectjob.hpp"
#include "abstractconnectjob_p.hpp"

namespace Kasten {

AbstractConnectJob::AbstractConnectJob(std::unique_ptr<AbstractConnectJobPrivate>&& dd)
    : d_ptr(std::move(dd))
{
}

AbstractConnectJob::AbstractConnectJob()
    : d_ptr(std::make_unique<AbstractConnectJobPrivate>(this))
{}

AbstractConnectJob::~AbstractConnectJob() = default;

}

#include "moc_abstractconnectjob.cpp"
