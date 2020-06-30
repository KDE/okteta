/*
    This file is part of the Okteta Gui library, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "widgetcolumnstylist.hpp"
#include "widgetcolumnstylist_p.hpp"

// Qt
#include <QWidget>

namespace Okteta {

WidgetColumnStylist::WidgetColumnStylist(QWidget* parent)
    : AbstractColumnStylist(new WidgetColumnStylistPrivate(parent))
{
}

WidgetColumnStylist::~WidgetColumnStylist() = default;

const QPalette& WidgetColumnStylist::palette() const
{
    Q_D(const WidgetColumnStylist);

    return d->widget()->palette();
}

}
