/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2010 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "terminaltoolviewfactory.hpp"

// lib
#include "terminaltoolview.hpp"
#include "terminaltool.hpp"
// KF
#include <KLocalizedString>

namespace Kasten {

TerminalToolViewFactory::TerminalToolViewFactory() = default;

TerminalToolViewFactory::~TerminalToolViewFactory() = default;

QString TerminalToolViewFactory::iconName() const { return QStringLiteral("utilities-terminal"); }
QString TerminalToolViewFactory::title()    const { return i18nc("@title:window", "Terminal"); }
QString TerminalToolViewFactory::id()       const { return QStringLiteral("org.kde.kasten.TerminalToolView"); }
SidePosition TerminalToolViewFactory::defaultPosition() const { return BottomSidePosition; }

AbstractToolView* TerminalToolViewFactory::create(AbstractTool* tool) const
{
    return new TerminalToolView(qobject_cast<TerminalTool*>(tool));
}

}
