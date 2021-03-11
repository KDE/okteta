/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ABSTRACTSYNCWITHREMOTEJOB_HPP
#define KASTEN_ABSTRACTSYNCWITHREMOTEJOB_HPP

// lib
#include "kastencore_export.hpp"
// KF
#include <KJob>

namespace Kasten {

class AbstractSyncWithRemoteJobPrivate;

class KASTENCORE_EXPORT AbstractSyncWithRemoteJob : public KJob
{
    Q_OBJECT

protected:
    explicit AbstractSyncWithRemoteJob(AbstractSyncWithRemoteJobPrivate* d);

public:
    AbstractSyncWithRemoteJob();

    ~AbstractSyncWithRemoteJob() override;

protected:
    const QScopedPointer<AbstractSyncWithRemoteJobPrivate> d_ptr;
};

}

#endif
