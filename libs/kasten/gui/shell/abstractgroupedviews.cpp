/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2007, 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "abstractgroupedviews.hpp"
#include "abstractgroupedviews_p.hpp"

namespace Kasten {

AbstractGroupedViews::AbstractGroupedViews()
    : AbstractViewArea(new AbstractGroupedViewsPrivate(this))
{
}

AbstractGroupedViews::AbstractGroupedViews(AbstractGroupedViewsPrivate* d)
    : AbstractViewArea(d)
{
}

AbstractGroupedViews::~AbstractGroupedViews() = default;

}
