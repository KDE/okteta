/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ABSTRACTCONNECTJOB_HPP
#define KASTEN_ABSTRACTCONNECTJOB_HPP

// lib
#include "kastencore_export.hpp"
// KF
#include <KJob>
// Std
#include <memory>

namespace Kasten {

class AbstractConnectJobPrivate;

class KASTENCORE_EXPORT AbstractConnectJob : public KJob
{
    Q_OBJECT

protected:
    KASTENCORE_NO_EXPORT explicit AbstractConnectJob(std::unique_ptr<AbstractConnectJobPrivate>&& dd);

public:
    AbstractConnectJob();

    ~AbstractConnectJob() override;

protected:
    // emits documentLoaded()
    // TODO: or better name property LoadedDocument?

protected:
    const std::unique_ptr<AbstractConnectJobPrivate> d_ptr;
};

}

#endif
