/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2010 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytetabletoolviewfactory.hpp"

// lib
#include "bytetabletoolview.hpp"
#include "bytetabletool.hpp"
// KF
#include <KLocalizedString>

namespace Kasten {

ByteTableToolViewFactory::ByteTableToolViewFactory() = default;

ByteTableToolViewFactory::~ByteTableToolViewFactory() = default;

QString ByteTableToolViewFactory::iconName() const { return QStringLiteral("table"); }
QString ByteTableToolViewFactory::title()    const { return i18nc("@title:window", "Value/Char Table"); }
QString ByteTableToolViewFactory::id()       const { return QStringLiteral("org.kde.okteta.ByteTableToolView"); }
SidePosition ByteTableToolViewFactory::defaultPosition() const { return RightSidePosition; }

AbstractToolView* ByteTableToolViewFactory::create(AbstractTool* tool) const
{
    return new ByteTableToolView(qobject_cast<ByteTableTool*>(tool));
}

}
