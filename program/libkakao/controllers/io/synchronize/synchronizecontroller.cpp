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

// Kakao core
#include <abstractsynctoremotejob.h>
#include <abstractsyncfromremotejob.h>
#include <abstractmodelfilesystemsynchronizer.h>
#include <jobmanager.h>
// KDE
#include <KUrl>
#include <KActionCollection>
#include <KAction>
#include <KStandardAction>
#include <KXMLGUIClient>


SynchronizeController::SynchronizeController( KXMLGUIClient* guiClient )
: mDocument( 0 ), mSynchronizer( 0 )
{
    KActionCollection* actionCollection = guiClient->actionCollection();

    mSaveAction   = KStandardAction::save(   this, SLOT(save()),   actionCollection );
    mReloadAction = KStandardAction::revert( this, SLOT(reload()), actionCollection );

    setTargetModel( 0 );
}

void SynchronizeController::setTargetModel( AbstractModel* model )
{
    if( mDocument ) mDocument->disconnect( this );

    mDocument = model ? model->findBaseModel<KAbstractDocument*>() : 0;

    if( mDocument )
    {
        connect( mDocument, SIGNAL(synchronizerChanged( AbstractModelSynchronizer* )),
                            SLOT(onSynchronizerChange( AbstractModelSynchronizer* )) );
    }
    onSynchronizerChange( mDocument ? mDocument->synchronizer() : 0 );
}

void SynchronizeController::save()
{
    AbstractSyncToRemoteJob *syncJob = mDocument->synchronizer()->startSyncToRemote();
    JobManager::executeJob( syncJob );
}

void SynchronizeController::reload()
{
    AbstractSyncFromRemoteJob *syncJob = mDocument->synchronizer()->startSyncFromRemote();
    JobManager::executeJob( syncJob );
}

void SynchronizeController::onSynchronizerChange( AbstractModelSynchronizer* newSynchronizer )
{
    mSynchronizer = qobject_cast<AbstractModelFileSystemSynchronizer*>( newSynchronizer );
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
