/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008, 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "toolviewdockwidget.hpp"
#include "toolviewdockwidget_p.hpp"

// lib
#include <abstracttoolview.hpp>

namespace Kasten {

ToolViewDockWidget::ToolViewDockWidget(AbstractToolView* toolView, QWidget* parent)
    : QDockWidget(toolView->title(), parent)
    , d_ptr(new ToolViewDockWidgetPrivate(toolView))
{
    Q_D(ToolViewDockWidget);

    d->init(this);
}

ToolViewDockWidget::~ToolViewDockWidget() = default;

AbstractToolView* ToolViewDockWidget::toolView() const
{
    Q_D(const ToolViewDockWidget);

    return d->toolView();
}

bool ToolViewDockWidget::isShown() const
{
    Q_D(const ToolViewDockWidget);

    return d->isShown();
}

}
