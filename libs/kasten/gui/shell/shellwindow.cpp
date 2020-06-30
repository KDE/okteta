/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2007-2009, 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "shellwindow.hpp"
#include "shellwindow_p.hpp"

namespace Kasten {

ShellWindow::ShellWindow(ViewManager* viewManager)
    : d_ptr(new ShellWindowPrivate(this, viewManager))
{
}

ShellWindow::~ShellWindow() = default;

ViewManager* ShellWindow::viewManager() const
{
    Q_D(const ShellWindow);

    return d->viewManager();
}

MultiViewAreas* ShellWindow::viewArea() const
{
    Q_D(const ShellWindow);

    return d->viewArea();
}

QVector<ToolViewDockWidget*> ShellWindow::dockWidgets() const
{
    Q_D(const ShellWindow);

    return d->dockWidgets();
}

void ShellWindow::addXmlGuiController(AbstractXmlGuiController* controller)
{
    Q_D(ShellWindow);

    d->addXmlGuiController(controller);
}

void ShellWindow::addTool(AbstractToolView* toolView)
{
    Q_D(ShellWindow);

    d->addTool(toolView);
}

void ShellWindow::showDocument(AbstractDocument* document)
{
    Q_D(ShellWindow);

    d->showDocument(document);
}

void ShellWindow::updateControllers(AbstractView* view)
{
    Q_D(ShellWindow);

    d->updateControllers(view);
}

}
