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

#include "modelservermanager.h"
#include "modelservermanager_p.h"

// lib
#include "abstractmodelnetworkserver.h"
#include "networkclientconnection.h"
// KDE
#include <DNSSD/PublicService>
// Qt
#include <QtNetwork/QTcpServer>

static const bool DefaultIsPublished = true;
static const int DefaultClientsCountLimit = 50;
// TODO: make this configurable
static const quint16 ModelServerPort = 32100;
static const quint16 ModelServerMinPort = 32100;
static const quint16 ModelServerMaxPort = 32200;
static const char DNSSDProtocolType[] = "_kakaosync._tcp";


ModelServerManager::Private::Private( ModelServerManager* parent )
 : p( parent ), mTcpServer( 0 ), mPublicService( 0 ),
   mIsPublished( DefaultIsPublished ), mClientsCountLimit( DefaultClientsCountLimit )
{}


bool ModelServerManager::Private::startListening()
{
    mTcpServer = new QTcpServer( p );
    mTcpServer->listen( QHostAddress::Any, ModelServerPort );

    const bool tcpServerStarted = ( mTcpServer && mTcpServer->isListening() );
    if( tcpServerStarted )
    {
// kDebug() << "Listening to ModelServerPort " << mTcpServer->serverPort();
        connect( mTcpServer, SIGNAL(newConnection()),
                 p, SLOT(onNewConnection()) );
        if( mIsPublished )
            startPublishing();
    }
    else
    {
// kError() << "Tcp server failed";
        delete mTcpServer;
        mTcpServer = 0;
    }

    return tcpServerStarted;
}

void ModelServerManager::Private::startPublishing()
{
    if( !mPublicService )
    {
        mPublicService = new DNSSD::PublicService( "Okteta server", DNSSDProtocolType, ModelServerPort );
        connect( mPublicService, SIGNAL(published( bool )),
                 p, SLOT(onPublished( bool )) );
    }

    mPublicService->publishAsync();
}


void ModelServerManager::Private::stopListening()
{
    delete mTcpServer;
    mTcpServer = 0;

    if( mIsPublished )
        stopPublishing();
}

void ModelServerManager::Private::stopPublishing()
{
    if( mPublicService )
        mPublicService->stop();
}

void ModelServerManager::Private::setIsPublished( bool isPublished )
{
    if( mIsPublished == isPublished )
        return;

    mIsPublished = isPublished;

    const bool tcpServerStarted = ( mTcpServer && mTcpServer->isListening() );
    if( tcpServerStarted )
    {
        if( mIsPublished )
            startPublishing();
        else
            stopPublishing();
    }
}


void ModelServerManager::Private::onNewConnection()
{
    // TODO: count clients!
//     if( mClientConnectionList.count() == mClientsCountLimit )
//         return;

    QTcpSocket* tcpSocket = mTcpServer->nextPendingConnection();
    NetworkClientConnection* clientConnection = new NetworkClientConnection();
    p->connect( clientConnection, SIGNAL(connected()), SLOT(onConnected()) );
    p->connect( clientConnection, SIGNAL(disconnected()), SLOT(onDisconnected()) );
    clientConnection->startConnectToClient( tcpSocket );
    mClientConnectionList.append( clientConnection );
}

void ModelServerManager::Private::onConnected()
{
}

void ModelServerManager::Private::onDisconnected()
{
    NetworkClientConnection* clientConnection = qobject_cast<NetworkClientConnection*>( p->sender() );
    mClientConnectionList.removeOne( clientConnection );
    clientConnection->deleteLater();
}

void ModelServerManager::Private::onPublished( bool successful )
{
Q_UNUSED( successful )
// TODO: push feedback into info pipe
}


ModelServerManager::Private::~Private()
{
    delete mPublicService;

    qDeleteAll( mClientConnectionList );
}