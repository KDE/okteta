/*
    This file is part of the Kasten Framework, made within the KDE community.

    Copyright 2010 Friedrich W. H. Kossebau <kossebau@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library. If not, see <http://www.gnu.org/licenses/>.
*/

#include "bytetabletoolviewfactory.h"

// lib
#include "bytetabletoolview.h"
#include "bytetabletool.h"
// KDE
#include <KLocale>


namespace Kasten2
{

ByteTableToolViewFactory::ByteTableToolViewFactory() {}

QString ByteTableToolViewFactory::iconName() const { return QLatin1String("table"); }
QString ByteTableToolViewFactory::title()    const { return i18nc("@title:window", "Value/Char Table"); }
QString ByteTableToolViewFactory::id()       const { return QLatin1String("org.kde.okteta.ByteTableToolView"); }
SidePosition ByteTableToolViewFactory::defaultPosition() const { return RightSidePosition; }

AbstractToolView* ByteTableToolViewFactory::create( AbstractTool* tool )
{
    return new ByteTableToolView( qobject_cast<ByteTableTool*>(tool) );
}

ByteTableToolViewFactory::~ByteTableToolViewFactory() {}

}
