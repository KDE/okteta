/*
    This file is part of the Okteta Kakao module, part of the KDE project.

    Copyright 2006-2007 Friedrich W. H. Kossebau <kossebau@kde.org>

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
