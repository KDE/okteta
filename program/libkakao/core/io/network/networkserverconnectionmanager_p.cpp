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

#include "networkserverconnectionmanager_p.h"

// lib
#include "networkserverconnection.h"
// Qt
#include <QtCore/QMetaObject>
#include <QtCore/QTimer>

#include <KDebug>

NetworkServerConnectionManager::Private::Private( NetworkServerConnectionManager* parent )
 : p( parent ), mPerson( Person::createEgo() )
{}

void NetworkServerConnectionManager::Private::startCreateConnectionToServer( const KUrl& url,
                                                                             QObject* receiver, const char* methodName )
{
    NetworkServerConnection* requestedServerConnection = 0;

    foreach( NetworkServerConnection* serverConnection, mServerConnectionList )
    {
        if( serverConnection->matchesServer(url) )
        {
kDebug() << serverConnection;
            requestedServerConnection = serverConnection;
            mExistingRequestedServerConnections.append( requestedServerConnection );
            QTimer::singleShot( 0, p, SLOT(notifyAboutExistingConnections()) );
            break;
        }
    }

    if( requestedServerConnection == 0 )
    {
        requestedServerConnection = new NetworkServerConnection( p );
        mServerConnectionList.append( requestedServerConnection );
        connect( requestedServerConnection, SIGNAL(connected()), p, SLOT(onConnectedToServer()) );
        connect( requestedServerConnection, SIGNAL(disconnected()), p, SLOT(onDisconnected()) );
        connect( requestedServerConnection, SIGNAL(error(int)), p, SLOT(onError(int)) );

        requestedServerConnection->startConnectToServer( url, mPerson );
kDebug() << "started connect to server"<<url;
    }

    mWaitingCallBackList.insert( requestedServerConnection, CallBackData(receiver,methodName) );
}


void NetworkServerConnectionManager::Private::notifyRequesters( NetworkServerConnection* serverConnection )
{
kDebug()<<"Callback list count: "<<mWaitingCallBackList.count();
    // TODO: bug Qt to add a "QList<T> take()" to QMultiHash
    QMultiHash<NetworkServerConnection*,CallBackData>::ConstIterator it =
        mWaitingCallBackList.find( serverConnection );
    QMultiHash<NetworkServerConnection*,CallBackData>::ConstIterator end =
        mWaitingCallBackList.end();
    while( it != end && it.key() == serverConnection )
    {
        const CallBackData& callBackData = it.value();
        QMetaObject::invokeMethod( callBackData.mReceiver, callBackData.mMethodName,
                                   Q_ARG(NetworkServerConnection*, serverConnection) );
kDebug()<<"invoking: "<<callBackData.mReceiver<<" with "<<callBackData.mMethodName;
        ++it;
    }

    mWaitingCallBackList.remove( serverConnection );
}

void NetworkServerConnectionManager::Private::notifyAboutExistingConnections()
{
    foreach( NetworkServerConnection* serverConnection, mExistingRequestedServerConnections )
    {
        notifyRequesters( serverConnection );
    }

    mExistingRequestedServerConnections.clear();
}


void NetworkServerConnectionManager::Private::onConnectedToServer()
{
    NetworkServerConnection* serverConnection = qobject_cast<NetworkServerConnection*>( p->sender() );

    notifyRequesters( serverConnection );
}

void NetworkServerConnectionManager::Private::onError( int errorCode )
{
Q_UNUSED( errorCode )
}

void NetworkServerConnectionManager::Private::onDisconnected()
{
    NetworkServerConnection* serverConnection = qobject_cast<NetworkServerConnection*>( p->sender() );
    mServerConnectionList.removeOne( serverConnection );
    serverConnection->deleteLater();
}


NetworkServerConnectionManager::Private::~Private() {}
