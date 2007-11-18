/***************************************************************************
                          loadercontroller.cpp  -  description
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


#include "loadercontroller.h"

// kakao
#include <kdocumentloadermanager.h>
// KDE
#include <KRecentFilesAction>
#include <KActionCollection>
#include <KStandardAction>
#include <KXmlGuiWindow>
#include <KFileDialog>
#include <KGlobal>

static const char AllFileNamesFilter[] = "*";
static const char CreatorConfigGroupId[] = "Recent Files";

LoaderController::LoaderController( KDocumentLoaderManager *loaderManager, KXmlGuiWindow *window )
: mLoaderManager( loaderManager ), mMainWindow( window )
{
    KActionCollection *actionCollection = mMainWindow->actionCollection();

    KStandardAction::open(       this, SLOT(load()),      actionCollection );
    mOpenRecentAction =
        KStandardAction::openRecent( this, SLOT(loadRecent( const KUrl& )), actionCollection );

    KConfigGroup configGroup( KGlobal::config(), CreatorConfigGroupId );
    mOpenRecentAction->loadEntries( configGroup );

    connect( mLoaderManager, SIGNAL(urlUsed( const KUrl& )), SLOT(onUrlUsed( const KUrl& )) );
}


void LoaderController::setView( KAbstractView *view )
{
Q_UNUSED( view )
}

void LoaderController::load()
{
    mLoaderManager->load();

//     KUrl url = KFileDialog::getOpenUrl( QString()/*mWorkingUrl.url()*/, AllFileNamesFilter, mMainWindow );

//     if( !url.isEmpty() )
//     {
//         mLoaderManager->load( url );
        // store path
//         mWorkingUrl = url.upUrl();
//         mOpenRecentAction->addUrl( url );
//     }
}

void LoaderController::loadRecent( const KUrl &url )
{
    // TODO: 
    mLoaderManager->load( url );
}

void LoaderController::onUrlUsed( const KUrl &url )
{
    mOpenRecentAction->addUrl( url );
}

LoaderController::~LoaderController()
{
    KConfigGroup configGroup( KGlobal::config(), CreatorConfigGroupId );
    mOpenRecentAction->saveEntries( configGroup );
}
