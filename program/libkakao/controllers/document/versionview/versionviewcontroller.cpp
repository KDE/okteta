/*
    This file is part of the Kakao Framework, part of the KDE project.

    Copyright 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "versionviewcontroller.h"

// controller
#include "versionview.h"
// Kakao gui
#include <kabstractview.h>
// KDE
#include <KLocale>
#include <KActionCollection>
#include <KXmlGuiWindow>
// Qt
#include <QtGui/QAction>
#include <QtGui/QDockWidget>


VersionViewController::VersionViewController( KXmlGuiWindow *window )
{
    KActionCollection *actionCollection = window->actionCollection();

    mView = new VersionView();

    QDockWidget *dockWidget = new QDockWidget( i18nc("@title:window", "Versions"), window );
    dockWidget->setObjectName( "Versions" );
    dockWidget->setWidget( mView );
    window->addDockWidget( Qt::RightDockWidgetArea, dockWidget );

    QAction *action = dockWidget->toggleViewAction();
    action->setText( i18nc("@title:window", "Versions") );
    actionCollection->addAction( "show_versions", action );

    setView( 0 );
}

void VersionViewController::setView( KAbstractView *view )
{
    KAbstractDocument *document = view ? view->document() : 0;

    mView->setDocument( document );
}

VersionViewController::~VersionViewController() {}

#include "versionviewcontroller.moc"
