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

#include "bytearrayrowviewdesignerfactory.h"

// plugin
#include "fillerbytearraymodel.h"
// Okteta gui
#include <bytearrayrowview.h>


// TODO: add translations for visible names
ByteArrayRowViewDesignerFactory::ByteArrayRowViewDesignerFactory( QObject* parent )
  : QObject( parent )
{
}

QWidget* ByteArrayRowViewDesignerFactory::createWidget( QWidget* parent )
{
    Okteta::ByteArrayRowView* widget = new Okteta::ByteArrayRowView( parent );
    widget->setByteArrayModel( createFillerByteArrayModel(widget) );
    return widget;
}

QString ByteArrayRowViewDesignerFactory::group() const
{
    return QString::fromLatin1("Okteta (KDE)");
}

QIcon ByteArrayRowViewDesignerFactory::icon() const
{
    return QIcon();
}

QString ByteArrayRowViewDesignerFactory::includeFile() const
{
    return QString::fromLatin1("okteta/bytearrayrowview.h");
}

QString ByteArrayRowViewDesignerFactory::toolTip() const
{
    return QString::fromLatin1("Row oriented Viewer/Editor for Byte Arrays");
}

QString ByteArrayRowViewDesignerFactory::whatsThis() const
{
    return QString::fromLatin1("A widget to show and edit byte arrays provided by subclasses of Okteta::AbstractByteArrayModel");
}

bool ByteArrayRowViewDesignerFactory::isContainer() const
{
    return false;
}

QString ByteArrayRowViewDesignerFactory::name() const
{
    return QString::fromLatin1("Okteta::ByteArrayRowView");
}
