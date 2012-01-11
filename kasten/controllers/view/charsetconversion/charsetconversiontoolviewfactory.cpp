/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "charsetconversiontoolviewfactory.h"

// lib
#include "charsetconversiontoolview.h"
#include "charsetconversiontool.h"
// KDE
#include <KLocale>


namespace Kasten2
{

CharsetConversionToolViewFactory::CharsetConversionToolViewFactory() {}

QString CharsetConversionToolViewFactory::iconName() const { return QString::fromLatin1("okteta"); }
QString CharsetConversionToolViewFactory::title()    const { return i18nc("@title:window", "Charset Conversion"); }
QString CharsetConversionToolViewFactory::id()       const { return QString::fromLatin1("org.kde.okteta.CharsetConversionToolView"); }
SidePosition CharsetConversionToolViewFactory::defaultPosition() const { return RightSidePosition; }

AbstractToolView* CharsetConversionToolViewFactory::create( AbstractTool* tool )
{
    return new CharsetConversionToolView( qobject_cast<CharsetConversionTool*>(tool) );
}

CharsetConversionToolViewFactory::~CharsetConversionToolViewFactory() {}

}
