/*
    This file is part of the Okteta Designer plugin, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearraycolumnviewdesignerfactory.hpp"

// plugin
#include "fillerbytearraymodel.hpp"
// Okteta gui
#include <Okteta/ByteArrayColumnView>

// TODO: add translations for visible names
ByteArrayColumnViewDesignerFactory::ByteArrayColumnViewDesignerFactory(QObject* parent)
    : QObject(parent)
{
}

QWidget* ByteArrayColumnViewDesignerFactory::createWidget(QWidget* parent)
{
    auto* widget = new Okteta::ByteArrayColumnView(parent);
    widget->setByteArrayModel(createFillerByteArrayModel(widget));
    return widget;
}

QString ByteArrayColumnViewDesignerFactory::group() const
{
    return QStringLiteral("Okteta");
}

QIcon ByteArrayColumnViewDesignerFactory::icon() const
{
    return {};
}

QString ByteArrayColumnViewDesignerFactory::includeFile() const
{
    return QStringLiteral("okteta/bytearraycolumnview.hpp");
}

QString ByteArrayColumnViewDesignerFactory::toolTip() const
{
    return QStringLiteral("Column oriented Viewer/Editor for Byte Arrays");
}

QString ByteArrayColumnViewDesignerFactory::whatsThis() const
{
    return QStringLiteral("A widget to show and edit byte arrays provided by subclasses of Okteta::AbstractByteArrayModel");
}

bool ByteArrayColumnViewDesignerFactory::isContainer() const
{
    return false;
}

QString ByteArrayColumnViewDesignerFactory::name() const
{
    return QStringLiteral("Okteta::ByteArrayColumnView");
}
