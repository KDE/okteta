/***************************************************************************
                          fullscreencontroller.cpp  -  description
                             -------------------
    begin                : Mon Oct 15 2007
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


#include "fullscreencontroller.h"

// KDE
#include <KToggleFullScreenAction>
#include <KXmlGuiWindow>
#include <KActionCollection>


FullScreenController::FullScreenController( KXmlGuiWindow *window )
: mMainWindow( window )
{
    KActionCollection *actionCollection = mMainWindow->actionCollection();

    KToggleFullScreenAction *fullScreenAction = new KToggleFullScreenAction( actionCollection );
    fullScreenAction->setWindow( window );
    fullScreenAction->setShortcut( Qt::CTRL + Qt::SHIFT + Qt::Key_F11 );
    actionCollection->addAction( "view_fullscreen", fullScreenAction );
    connect( fullScreenAction , SIGNAL(toggled(bool)) , SLOT(switchFullScreen(bool)) ); 
}

void FullScreenController::setView( KAbstractView *view )
{
Q_UNUSED( view )
}

void FullScreenController::switchFullScreen( bool toFullScreen )
{
    if( toFullScreen )
        mMainWindow->showFullScreen();
    else
        mMainWindow->showNormal();
}
