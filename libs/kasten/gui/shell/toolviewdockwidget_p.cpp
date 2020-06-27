/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008, 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "toolviewdockwidget_p.hpp"

// lib
#include <abstracttoolview.hpp>
// Kasten core
#include <Kasten/AbstractTool>

namespace Kasten {

ToolViewDockWidgetPrivate::ToolViewDockWidgetPrivate(AbstractToolView* toolView)
    : mToolView(toolView)
{
}

ToolViewDockWidgetPrivate::~ToolViewDockWidgetPrivate()
{
    delete mToolView;
}

void ToolViewDockWidgetPrivate::init(ToolViewDockWidget* q)
{
    q->setObjectName(mToolView->tool()->objectName());
    q->setWidget(mToolView->widget());

    QObject::connect(q, &QDockWidget::visibilityChanged,
                     q, [this](bool isVisible) { onVisibilityChanged(isVisible); });
}

void ToolViewDockWidgetPrivate::onVisibilityChanged(bool isVisible)
{
    mIsShown = isVisible;
}

}
