/*
    This file is part of the Kasten Framework, made within the KDE community.

    Copyright 2007-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

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

// Kasten core
#include <documentsyncmanager.h>
#include <abstractmodelfilesystemsynchronizer.h>
// KDE
#include <KUrl>
#include <KActionCollection>
#include <KAction>
#include <KStandardAction>
#include <KXMLGUIClient>
#include <KLocale>


namespace Kasten2
{

SynchronizeController::SynchronizeController( DocumentSyncManager* syncManager, KXMLGUIClient* guiClient )
  : mSyncManager( syncManager ),
    mDocument( 0 ),
    mSynchronizer( 0 )
{
    KActionCollection* actionCollection = guiClient->actionCollection();

    mSaveAction = KStandardAction::save( this, SLOT(save()), actionCollection );

    mReloadAction = actionCollection->addAction( QLatin1String("file_reload") );
    mReloadAction->setText( i18nc("@title:menu","Reloa&d") );
    mReloadAction->setIcon( KIcon( QLatin1String("view-refresh") ) );
    mReloadAction->setShortcuts( KStandardShortcut::reload() );
    connect( mReloadAction, SIGNAL(triggered(bool)), SLOT(reload()) );

    setTargetModel( 0 );
}

void SynchronizeController::setTargetModel( AbstractModel* model )
{
    if( mDocument ) mDocument->disconnect( this );

    mDocument = model ? model->findBaseModel<AbstractDocument*>() : 0;

    if( mDocument )
    {
        connect( mDocument, SIGNAL(synchronizerChanged(Kasten2::AbstractModelSynchronizer*)),
                            SLOT(onSynchronizerChange(Kasten2::AbstractModelSynchronizer*)) );
    }
    onSynchronizerChange( mDocument ? mDocument->synchronizer() : 0 );
}

void SynchronizeController::save()
{
    mSyncManager->save( mDocument );
}

void SynchronizeController::reload()
{
    mSyncManager->reload( mDocument );
}

void SynchronizeController::onSynchronizerChange( AbstractModelSynchronizer* newSynchronizer )
{
    mSynchronizer = qobject_cast<AbstractModelFileSystemSynchronizer*>( newSynchronizer );
    // TODO: Storable interface should be used by Synchronizer 
    // synchronizer should report about possible activities
    // TODO: check for access rights, may not write
    bool canSync = false;
    if( mSynchronizer )
    {
        const LocalSyncState localSyncState = mDocument->localSyncState();
        const RemoteSyncState remoteSyncState = mDocument->remoteSyncState();
        canSync = ( localSyncState == LocalHasChanges )
                  || ( remoteSyncState == RemoteHasChanges )
                  || ( remoteSyncState == RemoteUnknown );

        connect( mDocument, SIGNAL(localSyncStateChanged(Kasten2::LocalSyncState)),
                            SLOT(onSyncStateChanged()) );
        connect( mDocument, SIGNAL(remoteSyncStateChanged(Kasten2::RemoteSyncState)),
                            SLOT(onSyncStateChanged()) );
    }

    mSaveAction->setEnabled( canSync );
    mReloadAction->setEnabled( canSync );
}

void SynchronizeController::onSyncStateChanged()
{
    const LocalSyncState localSyncState = mDocument->localSyncState();
    const RemoteSyncState remoteSyncState = mDocument->remoteSyncState();
    const bool canSync = ( localSyncState == LocalHasChanges )
                  || ( remoteSyncState == RemoteHasChanges )
                  || ( remoteSyncState == RemoteUnknown );

    mSaveAction->setEnabled( canSync );
    mReloadAction->setEnabled( canSync );
}

}
