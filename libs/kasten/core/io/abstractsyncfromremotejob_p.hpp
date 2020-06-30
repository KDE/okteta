/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ABSTRACTSYNCFROMREMOTEJOB_P_HPP
#define KASTEN_ABSTRACTSYNCFROMREMOTEJOB_P_HPP

#include "abstractsyncfromremotejob.hpp"

namespace Kasten {

class AbstractSyncFromRemoteJobPrivate
{
public:
    explicit AbstractSyncFromRemoteJobPrivate(AbstractSyncFromRemoteJob* parent);

    virtual ~AbstractSyncFromRemoteJobPrivate();

protected:
    AbstractSyncFromRemoteJob* const q_ptr;
};

inline AbstractSyncFromRemoteJobPrivate::AbstractSyncFromRemoteJobPrivate(AbstractSyncFromRemoteJob* parent)
    : q_ptr(parent)
{}

inline AbstractSyncFromRemoteJobPrivate::~AbstractSyncFromRemoteJobPrivate() = default;

}

#endif
