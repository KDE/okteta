/*
    This file is part of the Okteta Kakao module, part of the KDE project.

    Copyright 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "bytearraynetworksynchronizer.h"

// lib
#include "bytearraynetworkserverconnection.h"
#include "bytearraynetworkloadjob.h"
#include "bytearraynetworkserver.h"
#include <kbytearraydocument.h>
// Kakao core
#include <modelservermanager.h>
// Okteta core
#include <kpiecetablebytearraymodel.h>
// KDE
#include <KUrl>
#include <KLocale>
// Qt
#include <QtGui/QApplication>


ByteArrayNetworkSynchronizer::ByteArrayNetworkSynchronizer()
 : mDocument( 0 ), mNetworkServer( 0 ), mConnection( 0 )
{
}

void ByteArrayNetworkSynchronizer::set( KByteArrayDocument* document, ByteArrayNetworkServer* networkServer )
{
    mDocument = document;
    mNetworkServer = networkServer;
}

void ByteArrayNetworkSynchronizer::set( KByteArrayDocument* document, ByteArrayNetworkServerConnection* connection )
{
    mDocument = document;
    mConnection = connection;
}


KAbstractDocument* ByteArrayNetworkSynchronizer::document() const { return mDocument; }

void ByteArrayNetworkSynchronizer::startOffering( KAbstractDocument* document )
{
    mDocument = qobject_cast<KByteArrayDocument*>( document );
    mNetworkServer = new ByteArrayNetworkServer( mDocument, this );
    ModelServerManager::self()->addServer( mNetworkServer );
    mDocument->setLiveSynchronizer( this );
}

AbstractLoadJob* ByteArrayNetworkSynchronizer::startLoad( const KUrl& url )
{
    return new ByteArrayNetworkLoadJob( this, url );
}

AbstractSyncToRemoteJob* ByteArrayNetworkSynchronizer::startSyncToRemote()
{
    return 0; //new ByteArrayRawFileWriteJob( this );
}

AbstractSyncFromRemoteJob* ByteArrayNetworkSynchronizer::startSyncFromRemote()
{
    return 0; //new ByteArrayRawFileReloadJob( this );
}

AbstractSyncWithRemoteJob* ByteArrayNetworkSynchronizer::startSyncWithRemote( const KUrl& url, AbstractModelSynchronizer::ConnectOption option  )
{
    Q_UNUSED( url )
    Q_UNUSED( option )
    return 0; //new ByteArrayRawFileWriteToJob( this, url, option );
}

AbstractConnectJob* ByteArrayNetworkSynchronizer::startConnect( KAbstractDocument* document,
                                              const KUrl& url, AbstractModelSynchronizer::ConnectOption option )
{
    Q_UNUSED( document )
    Q_UNUSED( url )
    Q_UNUSED( option )
    return 0; //new ByteArrayRawFileConnectJob( this, document, url, option );
}


ByteArrayNetworkSynchronizer::~ByteArrayNetworkSynchronizer()
{
    if( mNetworkServer )
        // TODO: make this happen automatically on destruction of mNetworkServer
        ModelServerManager::self()->removeServer( mNetworkServer );
}
