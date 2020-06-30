/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2010 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "stringsextracttoolviewfactory.hpp"

// lib
#include "stringsextracttoolview.hpp"
#include "stringsextracttool.hpp"
// KF
#include <KLocalizedString>

namespace Kasten {

StringsExtractToolViewFactory::StringsExtractToolViewFactory() = default;

StringsExtractToolViewFactory::~StringsExtractToolViewFactory() = default;

QString StringsExtractToolViewFactory::iconName() const { return QStringLiteral("text-plain"); }
QString StringsExtractToolViewFactory::title()       const { return i18nc("@title:window", "Strings"); }
QString StringsExtractToolViewFactory::id()       const { return QStringLiteral("org.kde.okteta.StringsToolView"); }
SidePosition StringsExtractToolViewFactory::defaultPosition() const { return RightSidePosition; }

AbstractToolView* StringsExtractToolViewFactory::create(AbstractTool* tool) const
{
    return new StringsExtractToolView(qobject_cast<StringsExtractTool*>(tool));
}

}
