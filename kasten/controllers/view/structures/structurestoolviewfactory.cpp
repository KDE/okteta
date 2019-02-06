/*
 *   This file is part of the Okteta Kasten Framework, made within the KDE community.
 *
 *   Copyright 2010 Friedrich W. H. Kossebau <kossebau@kde.org>
 *
 *   This library is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU Lesser General Public
 *   License as published by the Free Software Foundation; either
 *   version 2.1 of the License, or (at your option) version 3, or any
 *   later version accepted by the membership of KDE e.V. (or its
 *   successor approved by the membership of KDE e.V.), which shall
 *   act as a proxy defined in Section 6 of version 3 of the license.
 *
 *   This library is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *   Lesser General Public License for more details.
 *
 *   You should have received a copy of the GNU Lesser General Public
 *   License along with this library. If not, see <http://www.gnu.org/licenses/>.
 */

#include "structurestoolviewfactory.hpp"

// lib
#include "structurestoolview.hpp"
#include "structurestool.hpp"
// KF5
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
