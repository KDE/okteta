/*
    This file is part of the Kasten Framework, made within the KDE community.

    Copyright 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "singleviewwindow.h"
#include "singleviewwindow_p.h"
#include "singleviewwindow.moc"


namespace Kasten
{

SingleViewWindow::SingleViewWindow( DocumentManager* documentManager, AbstractView* view )
  : KXmlGuiWindow(),
    d_ptr( new SingleViewWindowPrivate(this, documentManager, view) )
{
}

DocumentManager* SingleViewWindow::documentManager() const
{
    Q_D( const SingleViewWindow );

    return d->documentManager();
}

AbstractView* SingleViewWindow::view() const
{
    Q_D( const SingleViewWindow );

    return d->view();
}

SingleViewArea* SingleViewWindow::viewArea() const
{
    Q_D( const SingleViewWindow );

    return d->viewArea();
}

QList<ToolViewDockWidget*> SingleViewWindow::dockWidgets() const
{
    Q_D( const SingleViewWindow );

    return d->dockWidgets();
}

void SingleViewWindow::setView( AbstractView* view )
{
    Q_D( SingleViewWindow );

    d->setView( view );
}


void SingleViewWindow::addXmlGuiController( AbstractXmlGuiController* controller )
{
    Q_D( SingleViewWindow );

    d->addXmlGuiController( controller );
}

void SingleViewWindow::addTool( AbstractToolView* toolView )
{
    Q_D( SingleViewWindow );

    d->addTool( toolView );
}

bool SingleViewWindow::queryClose()
{
    Q_D( SingleViewWindow );

    return d->queryClose();
}


SingleViewWindow::~SingleViewWindow()
{
    delete d_ptr;
}

}
