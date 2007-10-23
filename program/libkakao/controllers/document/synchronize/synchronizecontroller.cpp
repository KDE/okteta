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
#include <kdocumentsynchronizer.h>
#include <kviewmanager.h>
#include <kistorable.h>
// KDE
#include <KActionCollection>
#include <KAction>
#include <KStandardAction>
#include <KXmlGuiWindow>


SynchronizeController::SynchronizeController( KDocumentSynchronizer *documentSynchronizer, KXmlGuiWindow *window )
: mDocumentSynchronizer( documentSynchronizer ), mMainWindow( window )
{
    KActionCollection *actionCollection = mMainWindow->actionCollection();

    mSaveAction   = KStandardAction::save(   this, SLOT(save()),   actionCollection );
    mSaveAsAction = KStandardAction::saveAs( this, SLOT(saveAs()), actionCollection );
//     mReloadAction = KStandardAction::revert( this, SLOT(reload()), actionCollection ); TODO

    setView( 0 );
}

void SynchronizeController::setView( KAbstractView *view )
{
    disconnect( mDocument );

    mDocument = ( view != 0 ) ? view->document() : 0;
    //TODO: reuse Factory here, it can load, so needs to know about decode, so should know about encode, too?
    mStoreControl = mDocument ? qobject_cast<KDE::If::Storable *>( mDocument ) : 0;
    // TODO: Storable interface should be used by Synchronizer 
    // synchronizer should report about possible activities
    bool inSync = true;
    bool hasUrl = false;
    if( mStoreControl )
    {
        inSync = ( mDocument->synchronizationStates() == KAbstractDocument::InSync );
        hasUrl = !mStoreControl->url().isEmpty();
        connect( mDocument, SIGNAL(modified( KAbstractDocument::SynchronizationStates )),
                            SLOT(onSynchronizationStateChange( KAbstractDocument::SynchronizationStates )) );
    }

    mSaveAction->setEnabled( mStoreControl && !inSync && hasUrl );
    mSaveAsAction->setEnabled( mStoreControl );
//     mReloadAction->setEnabled( mStoreControl && !inSync );// TODO: only if has url
}

void SynchronizeController::save()
{
    mDocumentSynchronizer->save( mDocument );
}

void SynchronizeController::saveAs()
{
    mDocumentSynchronizer->saveAs( mDocument );
}

void SynchronizeController::reload()
{
    mDocumentSynchronizer->reload( mDocument );
}

void SynchronizeController::onSynchronizationStateChange( KAbstractDocument::SynchronizationStates newStates )
{
    const bool hasUrl = !mStoreControl->url().isEmpty();
    mSaveAction->setEnabled( newStates!=KAbstractDocument::InSync && hasUrl );
//     mReloadAction->setEnabled( newStates!=KAbstractDocument::InSync && hasUrl ); // TODO: only if document has url
}
