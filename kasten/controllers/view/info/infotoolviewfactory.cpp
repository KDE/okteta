/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2010 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "infotoolviewfactory.hpp"

// lib
#include "infotoolview.hpp"
#include "infotool.hpp"
// KF
#include <KLocalizedString>

namespace Kasten {

InfoToolViewFactory::InfoToolViewFactory() = default;

InfoToolViewFactory::~InfoToolViewFactory() = default;

QString InfoToolViewFactory::iconName() const { return QStringLiteral("okteta"); }
QString InfoToolViewFactory::title()       const { return i18nc("@title:window", "Statistics"); }
QString InfoToolViewFactory::id()       const { return QStringLiteral("org.kde.okteta.StatisticsToolView"); }
SidePosition InfoToolViewFactory::defaultPosition() const { return RightSidePosition; }

AbstractToolView* InfoToolViewFactory::create(AbstractTool* tool) const
{
    return new InfoToolView(qobject_cast<InfoTool*>(tool));
}

}
