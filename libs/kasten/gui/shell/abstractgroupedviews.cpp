/*
    SPDX-FileCopyrightText: 2007, 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "abstractgroupedviews.hpp"
#include "abstractgroupedviews_p.hpp"

namespace Kasten {

AbstractGroupedViews::AbstractGroupedViews()
    : AbstractViewArea(std::make_unique<AbstractGroupedViewsPrivate>(this))
{
}

AbstractGroupedViews::AbstractGroupedViews(std::unique_ptr<AbstractGroupedViewsPrivate>&& dd)
    : AbstractViewArea(std::move(dd))
{
}

AbstractGroupedViews::~AbstractGroupedViews() = default;

}

#include "moc_abstractgroupedviews.cpp"
