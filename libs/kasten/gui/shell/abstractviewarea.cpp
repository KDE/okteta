/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "abstractviewarea.hpp"
#include "abstractviewarea_p.hpp"

// Qt
#include <Qt>

namespace Kasten {

AbstractViewArea::AbstractViewArea()
    : d_ptr(new AbstractViewAreaPrivate(this))
{}

AbstractViewArea::AbstractViewArea(AbstractViewAreaPrivate* d)
    : d_ptr(d)
{
}

AbstractViewArea::~AbstractViewArea() = default;

}
