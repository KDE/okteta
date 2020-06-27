/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ABSTRACTSYNCWITHREMOTEJOB_P_HPP
#define KASTEN_ABSTRACTSYNCWITHREMOTEJOB_P_HPP

#include "abstractsyncwithremotejob.hpp"

namespace Kasten {

class AbstractSyncWithRemoteJobPrivate
{
public:
    explicit AbstractSyncWithRemoteJobPrivate(AbstractSyncWithRemoteJob* parent);

    virtual ~AbstractSyncWithRemoteJobPrivate();

protected:
    AbstractSyncWithRemoteJob* const q_ptr;
};

inline AbstractSyncWithRemoteJobPrivate::AbstractSyncWithRemoteJobPrivate(AbstractSyncWithRemoteJob* parent)
    : q_ptr(parent)
{}

inline AbstractSyncWithRemoteJobPrivate::~AbstractSyncWithRemoteJobPrivate() = default;

}

#endif
