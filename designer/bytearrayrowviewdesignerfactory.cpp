/*
    This file is part of the Okteta Designer plugin, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearrayrowviewdesignerfactory.hpp"

// plugin
#include "fillerbytearraymodel.hpp"
// Okteta gui
#include <Okteta/ByteArrayRowView>

// TODO: add translations for visible names
ByteArrayRowViewDesignerFactory::ByteArrayRowViewDesignerFactory(QObject* parent)
    : QObject(parent)
{
}

QWidget* ByteArrayRowViewDesignerFactory::createWidget(QWidget* parent)
{
    auto* widget = new Okteta::ByteArrayRowView(parent);
    widget->setByteArrayModel(createFillerByteArrayModel(widget));
    return widget;
}

QString ByteArrayRowViewDesignerFactory::group() const
{
    return QStringLiteral("Okteta");
}

QIcon ByteArrayRowViewDesignerFactory::icon() const
{
    return {};
}

QString ByteArrayRowViewDesignerFactory::includeFile() const
{
    return QStringLiteral("okteta/bytearrayrowview.hpp");
}

QString ByteArrayRowViewDesignerFactory::toolTip() const
{
    return QStringLiteral("Row oriented Viewer/Editor for Byte Arrays");
}

QString ByteArrayRowViewDesignerFactory::whatsThis() const
{
    return QStringLiteral("A widget to show and edit byte arrays provided by subclasses of Okteta::AbstractByteArrayModel");
}

bool ByteArrayRowViewDesignerFactory::isContainer() const
{
    return false;
}

QString ByteArrayRowViewDesignerFactory::name() const
{
    return QStringLiteral("Okteta::ByteArrayRowView");
}
