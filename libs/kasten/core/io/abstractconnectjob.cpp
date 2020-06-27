/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "abstractconnectjob.hpp"
#include "abstractconnectjob_p.hpp"

namespace Kasten {

AbstractConnectJob::AbstractConnectJob(AbstractConnectJobPrivate* d)
    : d_ptr(d)
{
}

AbstractConnectJob::AbstractConnectJob()
    : d_ptr(new AbstractConnectJobPrivate(this))
{}

AbstractConnectJob::~AbstractConnectJob() = default;

}
