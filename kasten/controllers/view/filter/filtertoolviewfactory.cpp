/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2010 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "filtertoolviewfactory.hpp"

// lib
#include "filtertoolview.hpp"
#include "filtertool.hpp"
// KF
#include <KLocalizedString>

namespace Kasten {

FilterToolViewFactory::FilterToolViewFactory() = default;

FilterToolViewFactory::~FilterToolViewFactory() = default;

QString FilterToolViewFactory::iconName() const { return QStringLiteral("okteta"); }
QString FilterToolViewFactory::title()       const { return i18nc("@title:window", "Binary Filter"); }
QString FilterToolViewFactory:: id()      const { return QStringLiteral("org.kde.okteta.FilterToolView"); }
SidePosition FilterToolViewFactory::defaultPosition() const { return BottomSidePosition; }

AbstractToolView* FilterToolViewFactory::create(AbstractTool* tool) const
{
    return new FilterToolView(qobject_cast<FilterTool*>(tool));
}

}
