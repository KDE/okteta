/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ABSTRACTCONNECTJOB_P_HPP
#define KASTEN_ABSTRACTCONNECTJOB_P_HPP

#include "abstractconnectjob.hpp"

namespace Kasten {

class AbstractConnectJobPrivate
{
public:
    explicit AbstractConnectJobPrivate(AbstractConnectJob* parent);

    virtual ~AbstractConnectJobPrivate();

protected:
    AbstractConnectJob* const q_ptr;
};

inline AbstractConnectJobPrivate::AbstractConnectJobPrivate(AbstractConnectJob* parent)
    : q_ptr(parent)
{}

inline AbstractConnectJobPrivate::~AbstractConnectJobPrivate() = default;

}

#endif
