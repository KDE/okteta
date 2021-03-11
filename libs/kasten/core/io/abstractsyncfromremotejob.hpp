/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ABSTRACTSYNCFROMREMOTEJOB_HPP
#define KASTEN_ABSTRACTSYNCFROMREMOTEJOB_HPP

// lib
#include "kastencore_export.hpp"
// KF
#include <KJob>

namespace Kasten {

class AbstractSyncFromRemoteJobPrivate;

class KASTENCORE_EXPORT AbstractSyncFromRemoteJob : public KJob
{
    Q_OBJECT

protected:
    explicit AbstractSyncFromRemoteJob(AbstractSyncFromRemoteJobPrivate* parent);

public:
    AbstractSyncFromRemoteJob();

    ~AbstractSyncFromRemoteJob() override;

protected:
    const QScopedPointer<AbstractSyncFromRemoteJobPrivate> d_ptr;
};

}

#endif
