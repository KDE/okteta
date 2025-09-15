/*
    SPDX-FileCopyrightText: 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "singleviewwindow.hpp"
#include "singleviewwindow_p.hpp"

namespace Kasten {

SingleViewWindow::SingleViewWindow(AbstractView* view)
    : d_ptr(std::make_unique<SingleViewWindowPrivate>(this, view))
{
}

SingleViewWindow::~SingleViewWindow() = default;

AbstractView* SingleViewWindow::view() const
{
    Q_D(const SingleViewWindow);

    return d->view();
}

SingleViewArea* SingleViewWindow::viewArea() const
{
    Q_D(const SingleViewWindow);

    return d->viewArea();
}

QVector<ToolViewDockWidget*> SingleViewWindow::dockWidgets() const
{
    Q_D(const SingleViewWindow);

    return d->dockWidgets();
}

void SingleViewWindow::setView(AbstractView* view)
{
    Q_D(SingleViewWindow);

    d->setView(view);
}

void SingleViewWindow::addXmlGuiController(std::unique_ptr<AbstractXmlGuiController>&& controller)
{
    Q_D(SingleViewWindow);

    d->addXmlGuiController(std::move(controller));
}

void SingleViewWindow::addTool(std::unique_ptr<AbstractTool>&& tool, std::unique_ptr<AbstractToolView>&& toolView)
{
    Q_D(SingleViewWindow);

    d->addTool(std::move(tool), std::move(toolView));
}

}

#include "moc_singleviewwindow.cpp"
