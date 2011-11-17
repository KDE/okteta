/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2009-2010 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "modifiedbarcontroller.h"

// Kasten ui
#include <statusbar.h>
// KDE
#include <KLocale>
#include <KIcon>
// Qt
#include <QtGui/QLabel>


namespace Kasten1
{
static const int modifiedPixmapWidth = 16;


ModifiedBarController::ModifiedBarController( StatusBar* statusBar )
  : mDocument( 0 )
{
    // TODO: depend an statusbar height
    const QSize modifiedPixmapSize = QSize(modifiedPixmapWidth, modifiedPixmapWidth);

    mLocalStateLabel = new QLabel( statusBar );
    mLocalStateLabel->setAlignment( Qt::AlignCenter );
    mLocalStateLabel->setFixedSize( modifiedPixmapSize );
    statusBar->addWidget( mLocalStateLabel );

    mRemoteStateLabel = new QLabel( statusBar );
    mRemoteStateLabel->setAlignment( Qt::AlignCenter );
    mRemoteStateLabel->setFixedSize( modifiedPixmapSize );
    statusBar->addWidget( mRemoteStateLabel );

    setTargetModel( 0 );
}


void ModifiedBarController::setTargetModel( AbstractModel* model )
{
    if( mDocument ) mDocument->disconnect( this );

    mDocument = model ? model->findBaseModel<AbstractDocument*>() : 0;

    LocalSyncState localState;
    RemoteSyncState remoteState;
    if( mDocument )
    {
        localState = mDocument->localSyncState();
        remoteState = mDocument->remoteSyncState();
        connect( mDocument, SIGNAL(localSyncStateChanged(Kasten1::LocalSyncState)),
                 SLOT(onLocalSyncStateChanged(Kasten1::LocalSyncState)) );
        connect( mDocument, SIGNAL(remoteSyncStateChanged(Kasten1::RemoteSyncState)),
                 SLOT(onRemoteSyncStateChanged(Kasten1::RemoteSyncState)) );
    }
    else
    {
        localState = LocalInSync;
        remoteState = RemoteInSync;
    }
    onLocalSyncStateChanged( localState );
    onRemoteSyncStateChanged( remoteState );
    mLocalStateLabel->setEnabled( mDocument );
    mRemoteStateLabel->setEnabled( mDocument );
}


void ModifiedBarController::onLocalSyncStateChanged( Kasten1::LocalSyncState localSyncState )
{
    const bool isModified = (localSyncState == LocalHasChanges );

    // TODO: depend an statusbar height
    const QPixmap pixmap = isModified ?
        KIcon( QLatin1String("document-save") ).pixmap(modifiedPixmapWidth) :
        QPixmap();
    mLocalStateLabel->setPixmap( pixmap );

    mLocalStateLabel->setToolTip( isModified ?
        i18nc( "@tooltip the document is modified", "Modified." ) :
        i18nc( "@tooltip the document is not modified", "Not modified." ) );

}

void ModifiedBarController::onRemoteSyncStateChanged( Kasten1::RemoteSyncState remoteSyncState )
{
    const char* const iconName =
        ( remoteSyncState == RemoteHasChanges ) ?  "document-save" :
        ( remoteSyncState == RemoteNotSet ) ?      "document-new" :
        ( remoteSyncState == RemoteDeleted ) ?     "edit-delete" :
        ( remoteSyncState == RemoteUnknown ) ?     "flag-yellow" :
        ( remoteSyncState == RemoteUnreachable ) ? "network-disconnect" :
        /* else */                                 0;

    // TODO: depend an statusbar height
    const QPixmap pixmap = iconName ?
        KIcon( QLatin1String(iconName) ).pixmap(modifiedPixmapWidth) :
        QPixmap();
    mRemoteStateLabel->setPixmap( pixmap );

    // TODO: tooltips
}

}
