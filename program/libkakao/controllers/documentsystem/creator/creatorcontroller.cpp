/***************************************************************************
                          creatorcontroller.cpp  -  description
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


#include "creatorcontroller.h"

// kakao
#include <kdocumentcreator.h>
// KDE
#include <KActionCollection>
#include <KStandardAction>
#include <KXmlGuiWindow>

CreatorController::CreatorController( KDocumentCreator *creator, KXmlGuiWindow *window )
: mCreator( creator ), mMainWindow( window )
{
    KActionCollection *actionCollection = mMainWindow->actionCollection();

    KStandardAction::openNew( this, SLOT(createNew()), actionCollection );
}

void CreatorController::setView( KAbstractView *view )
{
Q_UNUSED( view )
}

void CreatorController::createNew()
{
    mCreator->createNew();
}

CreatorController::~CreatorController() {}
