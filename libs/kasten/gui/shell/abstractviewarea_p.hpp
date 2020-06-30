/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ABSTRACTVIEWAREA_P_HPP
#define KASTEN_ABSTRACTVIEWAREA_P_HPP

// lib
#include "abstractviewarea.hpp"
// Qt
#include <Qt>

namespace Kasten {

class AbstractViewAreaPrivate
{
public:
    explicit AbstractViewAreaPrivate(AbstractViewArea* viewArea);

    virtual ~AbstractViewAreaPrivate();

protected:
    AbstractViewArea* const q_ptr;
};

inline AbstractViewAreaPrivate::AbstractViewAreaPrivate(AbstractViewArea* parent)
    : q_ptr(parent)
{
}

inline AbstractViewAreaPrivate::~AbstractViewAreaPrivate() = default;

}

#endif
