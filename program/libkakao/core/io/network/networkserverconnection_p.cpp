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

#include "networkserverconnection.h"
#include "networkserverconnection_p.h"

// lib
#include "personimpulsivestreamwriter.h"
// KDE
#include <KUrl>
#include <KLocale>

static const char MessageId = 'M';
static const char QuitMessageText[] = "QUIT";
static const char ProtocolId[] = "KAKAOSYNC/0.1";
static const char ProtocolIdSize = sizeof( ProtocolId );


bool NetworkServerConnection::Private::matchesServer( const KUrl& url ) const
{
    return mSocket && ( url.host() == mSocket->peerName() && url.port() == mSocket->peerPort() );
}

void NetworkServerConnection::Private::startConnectToServer( const KUrl& url, const Person& person )
{
    mPerson = person;

    const QString hostName = url.host();
    const quint16 port = url.port();

    mSocket = new QTcpSocket( p );

    connect( mSocket, SIGNAL(connected()),
             p, SLOT(onSocketConnected()) );
    connect( mSocket, SIGNAL(readyRead()),
             p, SLOT(onSocketReadyRead()) );
    connect( mSocket, SIGNAL(error( QAbstractSocket::SocketError )),
             p, SLOT(onSocketError( QAbstractSocket::SocketError )) );
    connect( mSocket, SIGNAL(disconnected()),
             p, SLOT(onSocketDisconnected()) );

    mSocket->connectToHost( hostName, port );
//     mAwaitingHeader = true;
//     mNextBlockLength = 0;
//     isRecursive = false;
}

void NetworkServerConnection::Private::startDisconnectFromServer()
{
//     command( QuitMessageText );

    mSocket->disconnectFromHost();
}


void NetworkServerConnection::Private::sendHandshake()
{
//     mSocket->write( &ProtocolIdSize, 1 );
    mSocket->write( ProtocolId, ProtocolIdSize );
//     mSocket->write( QByteArray(8, '\0'));
//     mSocket->write( infoHash);
//     mSocket->write( peerIdString);
//     mSocket->flush();
}


void NetworkServerConnection::Private::sendUserDetails()
{
    PersonImpulsiveStreamWriter personWriter( mPerson );
    personWriter.writeTo( mSocket );
}


void NetworkServerConnection::Private::tryReceiveHandshake()
{
    if( mSocket->bytesAvailable() < ProtocolIdSize ) // TODO: what if sent handshake is smaller? Timeout?
        return;

    // Sanity check the protocol ID
    QByteArray protocolId = mSocket->read( ProtocolIdSize );
    if( !protocolId.startsWith(ProtocolId) )
    {
        setErrorString( i18n("Server does not understand the KakaoSync protocol.") );
        mSocket->abort();
    }
    else
    {
        mState = HandshakeReceived;
        sendUserDetails();
        emit p->connected();
    }
}


void NetworkServerConnection::Private::onSocketConnected()
{
    sendHandshake();
    mState = WaitingForHandshake;
}

void NetworkServerConnection::Private::onSocketReadyRead()
{
    if( mState == WaitingForHandshake )
        tryReceiveHandshake();
}

void NetworkServerConnection::Private::onSocketError( QAbstractSocket::SocketError socketError )
{
    QString errorString;

    switch( socketError )
    {
    case QAbstractSocket::ConnectionRefusedError:
        errorString = i18n( "Connection was refused by the server at %1.", mSocket->peerName() );
        break;
    case QAbstractSocket::RemoteHostClosedError:
        errorString = i18n( "Connection was closed by the server." );
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

void NetworkServerConnection::Private::onSocketDisconnected()
{
    mState = Unconnected;
    emit p->disconnected();
}
