/*
    This file is part of the Kakao Framework, part of the KDE project.

    Copyright 2007-2008 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "synchronizecontroller.h"

// lib
#include <kabstractview.h>
// Kakao core
#include <abstractsynctoremotejob.h>
#include <abstractsyncfromremotejob.h>
#include <kabstractdocumentfilesystemsynchronizer.h>
#include <jobmanager.h>
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
    if( mDocument ) mDocument->disconnect( this );

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
    AbstractSyncToRemoteJob *syncJob = mDocument->synchronizer()->startSyncToRemote();
    JobManager::executeJob( syncJob, mMainWindow );
}

void SynchronizeController::reload()
{
    AbstractSyncFromRemoteJob *syncJob = mDocument->synchronizer()->startSyncFromRemote();
    JobManager::executeJob( syncJob, mMainWindow );
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
