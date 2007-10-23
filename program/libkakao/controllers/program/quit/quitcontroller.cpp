/***************************************************************************
                          quitcontroller.cpp  -  description
                             -------------------
    begin                : Fri Jun 2 2006
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


#include "quitcontroller.h"

// KDE
#include <KXmlGuiWindow>
#include <KActionCollection>
#include <KStandardAction>


QuitController::QuitController( KProgram *program, KXmlGuiWindow *window )
: mMainWindow( window )
{
Q_UNUSED( program )

    KActionCollection *actionCollection = mMainWindow->actionCollection();
    KStandardAction::quit( this, SLOT(quit()), actionCollection );
}

void QuitController::setView( KAbstractView *view )
{
Q_UNUSED( view )
}

void QuitController::quit()
{
    mMainWindow->close();
//   Program->quit(); // TODO: think about a base program
}
