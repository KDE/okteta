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

#include "abstractnetworkconnection.h"
#include "abstractnetworkconnection_p.h"

// lib
#include "personimpulsivestreamwriter.h"
// KDE
#include <KUrl>
#include <KLocale>
// Qt
#include <QtNetwork/QTcpSocket>

static const char MessageId = 'M';
static const char QuitMessageText[] = "QUIT";
static const char ProtocolId[] = "KAKAOSYNC/0.1";
static const char ProtocolIdSize = sizeof( ProtocolId );


void AbstractNetworkConnection::Private::startConnectToServer( const KUrl& url, const Person& person )
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


void AbstractNetworkConnection::Private::onSocketConnected()
{
    sendHandshake();
    mState = WaitingForHandshake;
}

void AbstractNetworkConnection::Private::onSocketReadyRead()
{
    if( mState == WaitingForHandshake )
        tryReceiveHandshake();
}

void AbstractNetworkConnection::Private::onSocketError( QAbstractSocket::SocketError socketError )
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

void AbstractNetworkConnection::Private::onSocketDisconnected()
{
    mState = Unconnected;
    emit p->disconnected();
}
