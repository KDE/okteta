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
// Qt
#include <QtGui/QDockWidget>


PODDecoderController::PODDecoderController( KXmlGuiWindow *window )
 : mWindow( window ), mTool( new PODDecoderTool() )

{
    QDockWidget *dockWidget = new QDockWidget( mWindow );
    mPrimitiveTypesView = new KPrimitiveTypesView( mTool, dockWidget );
    dockWidget->setWidget( mPrimitiveTypesView );
    mWindow->addDockWidget( Qt::BottomDockWidgetArea, dockWidget );
    //Window->addToolWidget( mPrimitiveTypesView );

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
