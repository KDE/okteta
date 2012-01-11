/*
    This file is part of the Kasten Framework, made within the KDE community.

    Copyright 2010 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "mobileshellwindow.h"

// Kasten core
#include <abstractview.h>
#include <documentmanager.h>
// Qt
#include <QtGui/QToolBar>


namespace Kasten2
{

MobileShellWindow::MobileShellWindow( DocumentManager* documentManager/*, ViewManager *viewManager*/ )
  : KMainWindow(),
    mDocumentManager( documentManager ),
    mView( 0 )
{
    QToolBar* toolBar = new QToolBar( this );
    toolBar->setMovable( false );
    addToolBar( Qt::BottomToolBarArea, toolBar );
}

void MobileShellWindow::setView( AbstractView* view )
{
    // TODO: what to do with the old one if existing?
    mView = view;
    QWidget* widget = mView->widget();
    setCentralWidget( widget );
}


#if 0
void ShellWindow::updateControllers( AbstractView* view )
{
    foreach( AbstractXmlGuiController* controller, mControllers )
        controller->setTargetModel( view );

    foreach( ToolViewDockWidget* dockWidget, mDockWidgets )
    {
        if( dockWidget->isShown() )
            dockWidget->toolView()->tool()->setTargetModel( view );
    }
}
#endif

void MobileShellWindow::addActionController( AbstractActionController* controller )
{
    mControllers.append( controller );
}

bool MobileShellWindow::queryClose()
{
    AbstractDocument* document = mView ? mView->findBaseModel<AbstractDocument*>() : 0;
    // TODO: query the document manager or query the view manager?
    return (! document) || mDocumentManager->canClose( document );
}


MobileShellWindow::~MobileShellWindow() {}

}
