/***************************************************************************
                          poddecodercontroller.cpp  -  description
                             -------------------
    begin                : Fri Nov 3 2006
    copyright            : 2006 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU General Public License           *
 *   version 2 as published by the Free Software Foundation.               *
 *                                                                         *
 ***************************************************************************/


#include "poddecodercontroller.h"

// controller
#include "kprimitivetypesview.h"
#include "poddecodertool.h"
// KDE
#include <KXmlGuiWindow>
#include <KActionCollection>
#include <KLocale>
// Qt
#include <QtGui/QDockWidget>
#include <QtGui/QAction>


PODDecoderController::PODDecoderController( KXmlGuiWindow *window )
 : mWindow( window ), mTool( new PODDecoderTool() )

{
    KActionCollection *actionCollection = mWindow->actionCollection();

    mPrimitiveTypesView = new KPrimitiveTypesView( mTool );

    QDockWidget *dockWidget = new QDockWidget( i18nc("@title:window", "Decoding Table"), mWindow );
    dockWidget->setObjectName( "PODDecoder" );
    dockWidget->setWidget( mPrimitiveTypesView );
    mWindow->addDockWidget( Qt::BottomDockWidgetArea, dockWidget );
    // TODO: create container view interface, which could be view, view group, etc...
    //mContainer->addToolWidget( mPrimitiveTypesView );

    QAction *action = dockWidget->toggleViewAction();
    action->setText( i18nc("@title:window", "Decoding Table") ); //TODO: better name needed!
//     action->setShortcut( Qt::Key_F7 );
    actionCollection->addAction( "show_pod_decoder", action );

    mPrimitiveTypesView->onDataChange();
}

void PODDecoderController::setView( KAbstractView *view )
{
    mTool->setView( view );
}

PODDecoderController::~PODDecoderController()
{
    delete mTool;
}
