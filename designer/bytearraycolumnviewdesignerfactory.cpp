/*
    This file is part of the Okteta Designer plugin, made within the KDE community.

    Copyright 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "bytearraycolumnviewdesignerfactory.h"

// plugin
#include "fillerbytearraymodel.h"
// Okteta gui
#include <bytearraycolumnview.h>


// TODO: add translations for visible names
ByteArrayColumnViewDesignerFactory::ByteArrayColumnViewDesignerFactory( QObject* parent )
  : QObject( parent )
{
}

QWidget* ByteArrayColumnViewDesignerFactory::createWidget( QWidget* parent )
{
    Okteta::ByteArrayColumnView* widget = new Okteta::ByteArrayColumnView( parent );
    widget->setByteArrayModel( createFillerByteArrayModel(widget) );
    return widget;
}

QString ByteArrayColumnViewDesignerFactory::group() const
{
    return QString::fromLatin1("Okteta (KDE)");
}

QIcon ByteArrayColumnViewDesignerFactory::icon() const
{
    return QIcon();
}

QString ByteArrayColumnViewDesignerFactory::includeFile() const
{
    return QString::fromLatin1("okteta/bytearraycolumnview.h");
}

QString ByteArrayColumnViewDesignerFactory::toolTip() const
{
    return QString::fromLatin1("Column oriented Viewer/Editor for Byte Arrays");
}

QString ByteArrayColumnViewDesignerFactory::whatsThis() const
{
    return QString::fromLatin1("A widget to show and edit byte arrays provided by subclasses of Okteta::AbstractByteArrayModel");
}

bool ByteArrayColumnViewDesignerFactory::isContainer() const
{
    return false;
}

QString ByteArrayColumnViewDesignerFactory::name() const
{
    return QString::fromLatin1("Okteta::ByteArrayColumnView");
}
