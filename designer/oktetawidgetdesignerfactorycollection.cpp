/*
    This file is part of the Okteta Designer plugin, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "oktetawidgetdesignerfactorycollection.hpp"

// plugin
#include "bytearraycolumnviewdesignerfactory.hpp"
#include "bytearrayrowviewdesignerfactory.hpp"

OktetaWidgetDesignerFactoryCollection::OktetaWidgetDesignerFactoryCollection(QObject* parent)
    : QObject(parent)
{
    mWidgetFactories.append(new ByteArrayColumnViewDesignerFactory(this));
    mWidgetFactories.append(new ByteArrayRowViewDesignerFactory(this));
}

QList<QDesignerCustomWidgetInterface*> OktetaWidgetDesignerFactoryCollection::customWidgets() const
{
    return mWidgetFactories;
}
