/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ABSTRACTSYNCTOREMOTEJOB_P_HPP
#define KASTEN_ABSTRACTSYNCTOREMOTEJOB_P_HPP

#include "abstractsynctoremotejob.hpp"

namespace Kasten {

class AbstractSyncToRemoteJobPrivate
{
public:
    explicit AbstractSyncToRemoteJobPrivate(AbstractSyncToRemoteJob* parent);

    virtual ~AbstractSyncToRemoteJobPrivate();

protected:
    AbstractSyncToRemoteJob* const q_ptr;
};

inline AbstractSyncToRemoteJobPrivate::AbstractSyncToRemoteJobPrivate(AbstractSyncToRemoteJob* parent)
    : q_ptr(parent)
{}

inline AbstractSyncToRemoteJobPrivate::~AbstractSyncToRemoteJobPrivate() = default;

}

#endif
