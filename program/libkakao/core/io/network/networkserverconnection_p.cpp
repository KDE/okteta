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
#include <setuprequestimpulsivestreamwriter.h>
#include <setupreplyimpulsivestreamreader.h>
#include <personimpulsivestreamwriter.h>
#include <userlistimpulsivestreamreader.h>
// KDE
#include <KUrl>
#include <KLocale>

static const char MessageId = 'M';
static const char QuitMessageText[] = "QUIT";


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

void NetworkServerConnection::Private::addModelConnection( AbstractModelNetworkServerConnection* modelConnection )
{
    mModelConnectionList.append( modelConnection );
}

void NetworkServerConnection::Private::removeModelConnection( AbstractModelNetworkServerConnection* modelConnection )
{
    mModelConnectionList.removeOne( modelConnection );

    // TODO: make close-on-last-removed configurable
    if( mModelConnectionList.count() == 0 )
        startDisconnectFromServer();
}

void NetworkServerConnection::Private::sendHandshake()
{
    SetupRequestImpulsiveStreamWriter setupRequestWriter;
    setupRequestWriter.writeTo( mSocket );
}

void NetworkServerConnection::Private::sendUserDetails()
{
    PersonImpulsiveStreamWriter personWriter( mPerson );
    personWriter.writeTo( mSocket );
}

void NetworkServerConnection::Private::handleSetupReply()
{
    SetupReplyImpulsiveStreamReader* setupReplyReader =
        (SetupReplyImpulsiveStreamReader*)mCurrentStreamReader;

    AbstractImpulsiveStreamReader* nextReader = 0;
    if( setupReplyReader->replyOkay() )
    {
        sendUserDetails();

        mState = WaitingForUserList;
        nextReader = new UserListImpulsiveStreamReader();
    }
    else
    {
        setErrorString( i18n("Server does not understand the KakaoSync protocol.") );
        mSocket->abort();
    }

    delete mCurrentStreamReader;
    mCurrentStreamReader = nextReader;
}

void NetworkServerConnection::Private::handleUserListMessage()
{
    UserListImpulsiveStreamReader* userListReader =
        (UserListImpulsiveStreamReader*)mCurrentStreamReader;
    mServerUserList = userListReader->userList();

    emit p->connected();

    delete mCurrentStreamReader;
    mCurrentStreamReader = 0;
}

void NetworkServerConnection::Private::onSocketConnected()
{
    sendHandshake();
    mState = WaitingForHandshake;
    mCurrentStreamReader = new SetupReplyImpulsiveStreamReader();
}

void NetworkServerConnection::Private::onSocketReadyRead()
{
    while( mCurrentStreamReader && mCurrentStreamReader->nextBytesNeeded() <= mSocket->bytesAvailable() )
    {
        mCurrentStreamReader->readFrom( mSocket );
        if( mCurrentStreamReader->isDone() )
        {
            switch( mState )
            {
            case WaitingForHandshake:
                handleSetupReply();
                break;
            case WaitingForUserList:
                handleUserListMessage();
                break;
            }
            break;
        }
    }
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

NetworkServerConnection::Private::~Private()
{
    delete mCurrentStreamReader;
}
