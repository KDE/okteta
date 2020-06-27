/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ABSTRACTGROUPEDVIEWS_P_HPP
#define KASTEN_ABSTRACTGROUPEDVIEWS_P_HPP

// lib
#include "abstractgroupedviews.hpp"
#include "abstractviewarea_p.hpp"

namespace Kasten {

class AbstractGroupedViewsPrivate : public AbstractViewAreaPrivate
{
public:
    explicit AbstractGroupedViewsPrivate(AbstractGroupedViews* parent);

    ~AbstractGroupedViewsPrivate() override;
};

inline AbstractGroupedViewsPrivate::AbstractGroupedViewsPrivate(AbstractGroupedViews* parent)
    : AbstractViewAreaPrivate(parent)
{
}

inline AbstractGroupedViewsPrivate::~AbstractGroupedViewsPrivate() = default;

}

#endif
