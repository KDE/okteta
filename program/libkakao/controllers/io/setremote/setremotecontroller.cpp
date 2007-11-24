/***************************************************************************
                          setremotecontroller.cpp  -  description
                             -------------------
    begin                : Sat Nov 17 2007
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


#include "setremotecontroller.h"

// kakao
#include <kviewmanager.h>
#include <kdocumentsyncmanager.h>
// KDE
#include <KActionCollection>
#include <KAction>
#include <KStandardAction>
#include <KXmlGuiWindow>


SetRemoteController::SetRemoteController( KDocumentSyncManager *syncManager, KXmlGuiWindow *window )
: mMainWindow( window ), mSyncManager( syncManager )
{
    KActionCollection *actionCollection = mMainWindow->actionCollection();

    mSaveAsAction = KStandardAction::saveAs( this, SLOT(saveAs()), actionCollection );

    setView( 0 );
}

void SetRemoteController::setView( KAbstractView *view )
{
    mDocument = ( view != 0 ) ? view->document() : 0;

    const bool canBeSaved = mDocument ?
                                ( mDocument->synchronizer() != 0 ||
                                  mSyncManager->hasSynchronizerForLocal(mDocument->mimeType()) ) :
                                false;

    mSaveAsAction->setEnabled( canBeSaved );
}

void SetRemoteController::saveAs()
{
    mSyncManager->setSynchronizer( mDocument );
}
