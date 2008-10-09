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

#ifndef NETWORKSERVERCONNECTION_P_H
#define NETWORKSERVERCONNECTION_P_H

#include "networkserverconnection.h"

// lib
#include <person.h>
// Qt
#include <QtNetwork/QTcpSocket>
#include <QtCore/QList>

class AbstractImpulsiveStreamReader;


class NetworkServerConnection::Private
{
  public:
    enum State { Unconnected, WaitingForHandshake, WaitingForUserList };

  public:
    Private( NetworkServerConnection* parent );
    ~Private();

  public:
    void startConnectToServer( const KUrl& url, const Person& person );
    void startDisconnectFromServer();

    void addModelConnection( AbstractModelNetworkServerConnection* modelConnection );
    void removeModelConnection( AbstractModelNetworkServerConnection* modelConnection );

    void setErrorString( const QString& errorString );

  public:
    QIODevice* ioDevice() const;
    bool matchesServer( const KUrl& url ) const;
    int error() const;
    QString errorString() const;
    QList<Person> userList() const;


  public: // Q_SLOTS
    void onSocketConnected();
    void onSocketReadyRead();
    void onSocketError( QAbstractSocket::SocketError socketError );
    void onSocketDisconnected();

  protected:
    void sendHandshake();
    void sendUserDetails();

    void handleSetupReply();
    void handleUserListMessage();

  protected:
    NetworkServerConnection* const p;

    QTcpSocket* mSocket;
    State mState;
    AbstractImpulsiveStreamReader* mCurrentStreamReader;

    Person mPerson;
    QString mLastErrorString;

    QList<Person> mServerUserList;

    QList<AbstractModelNetworkServerConnection*> mModelConnectionList;
};

inline NetworkServerConnection::Private::Private( NetworkServerConnection* parent )
 : p( parent ), mSocket( 0 ), mState( Unconnected ), mCurrentStreamReader( 0 )
{
}

inline QIODevice* NetworkServerConnection::Private::ioDevice() const { return mSocket; }
inline int NetworkServerConnection::Private::error() const { return 0; }
inline QString NetworkServerConnection::Private::errorString() const { return mLastErrorString; }
inline QList<Person> NetworkServerConnection::Private::userList() const { return mServerUserList; }

inline void NetworkServerConnection::Private::setErrorString( const QString& errorString )
{
    mLastErrorString = errorString;
}

#endif
