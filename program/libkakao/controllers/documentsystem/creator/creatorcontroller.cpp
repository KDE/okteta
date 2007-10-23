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
#include <KRecentFilesAction>
#include <KActionCollection>
#include <KStandardAction>
#include <KXmlGuiWindow>
#include <KFileDialog>
#include <KGlobal>

static const char AllFileNamesFilter[] = "*";
static const char CreatorConfigGroupId[] = "Recent Files";

CreatorController::CreatorController( KDocumentCreator *creator, KXmlGuiWindow *window )
: mCreator( creator ), mMainWindow( window )
{
    KActionCollection *actionCollection = mMainWindow->actionCollection();

    KStandardAction::openNew(    this, SLOT(createNew()), actionCollection );
    KStandardAction::open(       this, SLOT(open()),      actionCollection );
    mOpenRecentAction =
        KStandardAction::openRecent( this, SLOT(openRecent( const KUrl& )), actionCollection );

    KConfigGroup configGroup( KGlobal::config(), CreatorConfigGroupId );
    mOpenRecentAction->loadEntries( configGroup );
}

void CreatorController::setWorkingUrl( const KUrl &Url )
{
    mWorkingUrl = Url;
}

void CreatorController::setView( KAbstractView *view )
{
Q_UNUSED( view )
}

void CreatorController::createNew()
{
    mCreator->createNew();
}

void CreatorController::open()
{
    KUrl url = KFileDialog::getOpenUrl( mWorkingUrl.url(), AllFileNamesFilter, mMainWindow );

    if( !url.isEmpty() )
    {
        mCreator->open( url );
        // store path
        mWorkingUrl = url.upUrl();
        mOpenRecentAction->addUrl( url );
    }
}

void CreatorController::openRecent( const KUrl &url )
{
    mCreator->open( url );
}

CreatorController::~CreatorController()
{
    KConfigGroup configGroup( KGlobal::config(), CreatorConfigGroupId );
    mOpenRecentAction->saveEntries( configGroup );
}
