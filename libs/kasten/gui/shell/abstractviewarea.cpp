/*
    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "abstractviewarea.hpp"
#include "abstractviewarea_p.hpp"

// Std
#include <utility>

namespace Kasten {

AbstractViewArea::AbstractViewArea()
    : d_ptr(std::make_unique<AbstractViewAreaPrivate>(this))
{}

AbstractViewArea::AbstractViewArea(std::unique_ptr<AbstractViewAreaPrivate>&& dd)
    : d_ptr(std::move(dd))
{
}

AbstractViewArea::~AbstractViewArea() = default;

}

#include "moc_abstractviewarea.cpp"
