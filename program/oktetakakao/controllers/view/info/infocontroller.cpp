/***************************************************************************
                          infocontroller.cpp  -  description
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


#include "infocontroller.h"

// controller
#include "infoview.h"
#include "infotool.h"
// KDE
#include <KLocale>
#include <KActionCollection>
#include <KXmlGuiWindow>
// Qt
#include <QtGui/QAction>
#include <QtGui/QDockWidget>


InfoController::InfoController( KXmlGuiWindow *window )
 : mTool( new InfoTool() )
{
    KActionCollection *actionCollection = window->actionCollection();

    mView = new InfoView( mTool );

    QDockWidget *dockWidget = new QDockWidget( i18nc("@title:window", "Info"), window );
    dockWidget->setObjectName( "Info" );
    dockWidget->setWidget( mView );
    window->addDockWidget( Qt::BottomDockWidgetArea, dockWidget );

    QAction *action = dockWidget->toggleViewAction();
    action->setText( i18nc("@title:window", "Info") ); //TODO: better name needed!
    actionCollection->addAction( "show_info", action );

    setView( 0 );
}

void InfoController::setView( KAbstractView *view )
{
    mTool->setView( view );
}

InfoController::~InfoController()
{
    delete mTool;
}
