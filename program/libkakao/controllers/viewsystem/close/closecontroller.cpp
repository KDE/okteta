/***************************************************************************
                          closecontroller.cpp  -  description
                             -------------------
    begin                : Tue Nov 7 2006
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


#include "closecontroller.h"

// kakao
#include <kviewmanager.h>
// KDE
#include <KActionCollection>
#include <KAction>
#include <KStandardAction>
#include <KXmlGuiWindow>


CloseController::CloseController( KViewManager *viewManager, KXmlGuiWindow *window )
: mViewManager( viewManager ), mMainWindow( window )
{
    KActionCollection* actionCollection = window->actionCollection();

    mCloseAction  = KStandardAction::close(  this, SLOT(close()),  actionCollection );
    setView( 0 );
}

void CloseController::setView( KAbstractView *view )
{
    mView = view;
    const bool hasView = ( mView != 0 );

    mCloseAction->setEnabled( hasView );
}


void CloseController::close()
{
    mViewManager->closeView( mView );
}
