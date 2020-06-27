/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "toollistmenucontroller.hpp"

// Kasten gui
#include <Kasten/WidgetsDockable>
#include <Kasten/ToolViewDockWidget>
// KF
#include <KXMLGUIClient>
#include <KXMLGUIFactory>
// Qt
#include <QAction>

namespace Kasten {

static constexpr char ToolListActionListId[] = "tools_list";

ToolListMenuController::ToolListMenuController(If::WidgetsDockable* widgetsDockable,
                                               KXMLGUIClient* guiClient)
    : mWidgetsDockable(widgetsDockable)
    , mGuiClient(guiClient)
{
    // TODO: for now this is only called on start, so first create all tools/views before this controller
    updateActions();
}

void ToolListMenuController::setTargetModel(AbstractModel* model)
{
    Q_UNUSED(model)
}

void ToolListMenuController::updateActions()
{
    mGuiClient->unplugActionList(QLatin1String(ToolListActionListId));

    qDeleteAll(mToolActionList);
    mToolActionList.clear();

    const QVector<ToolViewDockWidget*> dockWidgets = mWidgetsDockable->dockWidgets();

    mToolActionList.reserve(dockWidgets.size());
    for (const ToolViewDockWidget* dockWidget : dockWidgets) {
        QAction* action = dockWidget->toggleViewAction();
        action->setText(dockWidget->windowTitle());
//         action->setText( mToolView->title() );
        mToolActionList.append(action);
    }

    mGuiClient->plugActionList(QLatin1String(ToolListActionListId), mToolActionList);
}

}
