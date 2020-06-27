/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2010 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "versionviewtoolviewfactory.hpp"

// lib
#include "versionviewtoolview.hpp"
#include "versionviewtool.hpp"
// KF
#include <KLocalizedString>

namespace Kasten {

VersionViewToolViewFactory::VersionViewToolViewFactory() = default;

VersionViewToolViewFactory::~VersionViewToolViewFactory() = default;

QString VersionViewToolViewFactory::iconName() const { return QStringLiteral("view-history"); }
QString VersionViewToolViewFactory::title()    const { return i18nc("@title:window", "Versions"); }
QString VersionViewToolViewFactory::id()       const { return QStringLiteral("org.kde.kasten.VersionViewToolView"); }
SidePosition VersionViewToolViewFactory::defaultPosition() const { return RightSidePosition; }

AbstractToolView* VersionViewToolViewFactory::create(AbstractTool* tool) const
{
    return new VersionViewToolView(qobject_cast<VersionViewTool*>(tool));
}

}
