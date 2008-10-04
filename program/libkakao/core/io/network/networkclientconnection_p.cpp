/*
    This file is part of the Kakao Framework, part of the KDE project.

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

#include "networkclientconnection_p.h"

// lib
#include "modelservermanager.h"
#include "abstractmodelnetworkserver.h"
#include "abstractmodelnetworkclientconnection.h"
#include <userlistimpulsivestreamwriter.h>
#include <personimpulsivestreamreader.h>
// KDE
#include <KUrl>
#include <KLocale>

#include <KDebug>

// Protocol: Header{ char 1: MessageId, char 2-5: length of load } Load {}  // writes the length (as quint32) and the data
static const char MessageId = 'M';
static const int HeaderSize = sizeof(quint8) + sizeof( quint32 );
static const int PendingRequestTimeout = 60 * 1000;
static const char ProtocolId[] = "KAKAOSYNC/0.1";
static const char ProtocolIdSize = sizeof( ProtocolId );


void NetworkClientConnection::Private::startConnectToClient( QTcpSocket* tcpSocket )
{
    mSocket = tcpSocket;

kDebug()<<mSocket->peerName()<<mSocket->peerPort()<<mSocket->state();
//     connect( mSocket, SIGNAL(connected()),
//              p, SLOT(onSocketConnected()) );
    connect( mSocket, SIGNAL(readyRead()),
             p, SLOT(onSocketReadyRead()) );
    connect( mSocket, SIGNAL(error( QAbstractSocket::SocketError )),
             p, SLOT(onSocketError( QAbstractSocket::SocketError )) );
    connect( mSocket, SIGNAL(disconnected()),
             p, SLOT(onSocketDisconnected()) );

    mState = WaitingForHandshake;
}

void NetworkClientConnection::Private::startDisconnectFromClient()
{
    mSocket->disconnectFromHost();
}


void NetworkClientConnection::Private::sendHandshake()
{
//     mSocket->write( &ProtocolIdSize, 1 );
    mSocket->write( ProtocolId, ProtocolIdSize );
//     mSocket->write( QByteArray(8, '\0'));
//     mSocket->write( infoHash);
//     mSocket->write( peerIdString);
//     mSocket->flush();
}

void NetworkClientConnection::Private::tryReceiveHandshake()
{
    if( mSocket->bytesAvailable() < ProtocolIdSize )
        return;

    // Sanity check the protocol ID
    QByteArray protocolId = mSocket->read( ProtocolIdSize );
    if( !protocolId.startsWith(ProtocolId) )
    {
        mSocket->abort();
        return;
    }
    sendHandshake();

    mState = WaitingForUserDetails;
    mCurrentStreamReader = new PersonImpulsiveStreamReader();
}


void NetworkClientConnection::Private::onSocketReadyRead()
{
    if( mState == WaitingForHandshake )
        tryReceiveHandshake();
    else
    while( mCurrentStreamReader && mCurrentStreamReader->nextBytesNeeded() <= mSocket->bytesAvailable() )
    {
        mCurrentStreamReader->readFrom( mSocket );
        if( mCurrentStreamReader->isDone() )
        {
            switch( mState )
            {
            case WaitingForUserDetails:
            {
                PersonImpulsiveStreamReader* personReader = (PersonImpulsiveStreamReader*)mCurrentStreamReader;
                mPerson = personReader->person();

                const int modelId = 0;

                QList<AbstractModelNetworkServer*> modelServerList = ModelServerManager::self()->serverList();
                AbstractModelNetworkServer* modelServer = modelServerList.at( modelId );

                AbstractModelNetworkClientConnection* modelClientConnection =
                    modelServer->createModelClientConnection( p );

                mModelClientConnectionList[modelId] = modelClientConnection;

                UserListImpulsiveStreamWriter userListWriter( modelServer->users() );
                userListWriter.writeTo( mSocket );

                emit p->connected();
                break;
            }
            }
            delete mCurrentStreamReader;
            mCurrentStreamReader = 0;
            break;
        }
#if 0
        {
            const bool loadNotCompletelyReadYet = ( mSocket->bytesAvailable() < (qint64) mLoadLength );
            if( loadNotCompletelyReadYet )
                break;

            QByteArray message( mLoadLength, 0 );
            stream.readRawData( message.data(), mLoadLength );

            p->receive( message );

            mHeaderRead = false;
        }
#endif
    }
}

void NetworkClientConnection::Private::onSocketError( QAbstractSocket::SocketError socketError )
{
    QString errorString;

    switch( socketError )
    {
    case QAbstractSocket::RemoteHostClosedError:
        errorString = i18n( "Connection was closed by the client." );
        break;
    case QAbstractSocket::NetworkError:
        errorString = i18n( "An error occurred with the network (e.g. cable unplugged)." );
        break;
    default:
        errorString = i18n( "Unspecified error in connection to server (code %1).", socketError );
    }
    setErrorString( errorString );

    emit p->error( error() );
}

void NetworkClientConnection::Private::onSocketDisconnected()
{
    // TODO: the documentserver should control the connections, how to signal him? By QObject::isDeletedSignal?
    qDeleteAll( mModelClientConnectionList );
    mModelClientConnectionList.clear();
//     foreach( AbstractModelNetworkClientConnection* modelClientConnection, mModelClientConnectionList )
//         
    emit p->disconnected();
}

NetworkClientConnection::Private::~Private()
{
    // TODO: see above
    qDeleteAll( mModelClientConnectionList );
    delete mCurrentStreamReader;
}
