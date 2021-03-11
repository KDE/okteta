/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ABSTRACTSYNCTOREMOTEJOB_HPP
#define KASTEN_ABSTRACTSYNCTOREMOTEJOB_HPP

// lib
#include "kastencore_export.hpp"
// KF
#include <KJob>

namespace Kasten {
class AbstractSyncToRemoteJobPrivate;

class KASTENCORE_EXPORT AbstractSyncToRemoteJob : public KJob
{
    Q_OBJECT

protected:
    explicit AbstractSyncToRemoteJob(AbstractSyncToRemoteJobPrivate* d);

public:
    AbstractSyncToRemoteJob();

    ~AbstractSyncToRemoteJob() override;

protected:
    const QScopedPointer<AbstractSyncToRemoteJobPrivate> d_ptr;
};

}

#endif
