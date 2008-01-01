/***************************************************************************
                          synchronizecontroller.cpp  -  description
                             -------------------
    begin                : Sun Sep 2 2007
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


#include "synchronizecontroller.h"

// kakao
#include <kabstractdocumentfilesystemsynchronizer.h>
#include <kviewmanager.h>
// KDE
#include <KUrl>
#include <KActionCollection>
#include <KAction>
#include <KStandardAction>
#include <KXmlGuiWindow>


SynchronizeController::SynchronizeController( KXmlGuiWindow *window )
: mMainWindow( window ), mDocument( 0 ), mSynchronizer( 0 )
{
    KActionCollection *actionCollection = mMainWindow->actionCollection();

    mSaveAction   = KStandardAction::save(   this, SLOT(save()),   actionCollection );
    mReloadAction = KStandardAction::revert( this, SLOT(reload()), actionCollection );

    setView( 0 );
}

void SynchronizeController::setView( KAbstractView *view )
{
    mDocument->disconnect( this );

    mDocument = ( view != 0 ) ? view->document() : 0;

    if( mDocument )
    {
        connect( mDocument, SIGNAL(synchronizerChanged( KAbstractDocumentSynchronizer * )),
                            SLOT(onSynchronizerChange( KAbstractDocumentSynchronizer * )) );
    }
    onSynchronizerChange( mDocument ? mDocument->synchronizer() : 0 );
}

void SynchronizeController::save()
{
    mSynchronizer->syncToRemote();
}

void SynchronizeController::reload()
{
    mSynchronizer->syncFromRemote();
}

void SynchronizeController::onSynchronizerChange( KAbstractDocumentSynchronizer *newSynchronizer )
{
    mSynchronizer = qobject_cast<KAbstractDocumentFileSystemSynchronizer *>( newSynchronizer );
    // TODO: Storable interface should be used by Synchronizer 
    // synchronizer should report about possible activities
    bool outOfSync = true;
    bool hasUrl = false;
    if( mSynchronizer )
    {
        hasUrl = !mSynchronizer->url().isEmpty();
        outOfSync = hasUrl && ( mDocument->synchronizationStates() != KAbstractDocument::InSync );
        connect( mDocument, SIGNAL(modified( KAbstractDocument::SynchronizationStates )),
                            SLOT(onSynchronizationStateChange( KAbstractDocument::SynchronizationStates )) );
    }

    mSaveAction->setEnabled( mSynchronizer && outOfSync );
    mReloadAction->setEnabled( mSynchronizer && outOfSync );
}

void SynchronizeController::onSynchronizationStateChange( KAbstractDocument::SynchronizationStates newStates )
{
    const bool hasUrl = !mSynchronizer->url().isEmpty();
    const bool outOfSync = hasUrl && ( newStates != KAbstractDocument::InSync );
    mSaveAction->setEnabled( outOfSync );
    mReloadAction->setEnabled( outOfSync );
}
