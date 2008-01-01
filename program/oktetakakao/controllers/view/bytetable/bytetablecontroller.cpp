/***************************************************************************
                          bytetablecontroller.cpp  -  description
                             -------------------
    begin                : Mon Dec 31 2007
    copyright            : 2007 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU General Public License           *
 *   version 2 as published by the Free Software Foundation.               *
 *                                                                         *
 ***************************************************************************/


#include "bytetablecontroller.h"

// controller
#include "bytetableview.h"
#include "bytetabletool.h"
//
#include <kbytearraydocument.h>
// kakao
#include <kabstractview.h>
// Okteta gui
#include <kbytearrayview.h>
// Okteta core
#include <kbytearraymodel.h>
// KDE
#include <KLocale>
#include <KXmlGuiWindow>
#include <KActionCollection>
// Qt
#include <QtGui/QAction>
#include <QtGui/QDockWidget>


ByteTableController::ByteTableController( KXmlGuiWindow *window )
 : mTool( new ByteTableTool() )
{
    KActionCollection *actionCollection = window->actionCollection();

    mView = new ByteTableView( mTool );

    QDockWidget *dockWidget = new QDockWidget( i18nc("@title:window", "Byte Table"), window );
    dockWidget->setObjectName( "ByteTable" );
    dockWidget->setWidget( mView );
    window->addDockWidget( Qt::BottomDockWidgetArea, dockWidget );

    QAction *action = dockWidget->toggleViewAction();
    action->setText( i18nc("@title:window", "Byte Table") ); //TODO: better name needed!
    actionCollection->addAction( "show_byte_table", action );

    setView( 0 );
}

void ByteTableController::setView( KAbstractView *view )
{
    mTool->setView( view );
}

ByteTableController::~ByteTableController()
{
    delete mTool;
}
