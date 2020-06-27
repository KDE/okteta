/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2010 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "structurestoolviewfactory.hpp"

// lib
#include "structurestoolview.hpp"
#include "structurestool.hpp"
// KF
#include <KLocalizedString>

namespace Kasten {

StructuresToolViewFactory::StructuresToolViewFactory() = default;

StructuresToolViewFactory::~StructuresToolViewFactory() = default;

QString StructuresToolViewFactory::iconName() const
{
    return QStringLiteral("okteta");
}
QString StructuresToolViewFactory::title() const
{
    return i18nc("@title:window", "Structures");
}
QString StructuresToolViewFactory::id() const
{
    return QStringLiteral("org.kde.okteta.StructuresToolView");
}
SidePosition StructuresToolViewFactory::defaultPosition() const
{
    return RightSidePosition;
}

AbstractToolView* StructuresToolViewFactory::create(AbstractTool* tool) const
{
    return new StructuresToolView(qobject_cast<StructuresTool*> (tool));
}

}
