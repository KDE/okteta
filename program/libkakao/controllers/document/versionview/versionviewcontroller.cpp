/***************************************************************************
                          versionviewcontroller.cpp  -  description
                             -------------------
    begin                : Wed Jan 2 2008
    copyright            : 2008 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU General Public License           *
 *   version 2 as published by the Free Software Foundation.               *
 *                                                                         *
 ***************************************************************************/


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
    window->addDockWidget( Qt::BottomDockWidgetArea, dockWidget );

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
