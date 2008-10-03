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

#ifndef ABSTRACTMODELNETWORKSERVERCONNECTION_P_H
#define ABSTRACTMODELNETWORKSERVERCONNECTION_P_H

#include "abstractmodelnetworkserverconnection.h"

// lib
#include <person.h>
// Qt
#include <QtNetwork/QAbstractSocket>

class QTcpSocket;


class AbstractModelNetworkServerConnection::Private
{
  public:
    enum State { Unconnected, WaitingForHandshake, HandshakeReceived };

  public:
    Private( AbstractModelNetworkServerConnection* parent );
    ~Private();

  public:
    void startConnectToServer( const KUrl& url, const Person& person );
    void startDisconnectFromServer();
    void setErrorString( const QString& errorString );

  public:
    int error() const;
    QString errorString() const;

  public: // Q_SLOTS
    void onSocketConnected();
    void onSocketReadyRead();
    void onSocketError( QAbstractSocket::SocketError socketError );
    void onSocketDisconnected();

  protected:
    void sendHandshake();
    void tryReceiveHandshake();

    void sendUserDetails();

  protected:
    AbstractModelNetworkServerConnection* const p;

    QTcpSocket* mSocket;
    State mState;

    Person mPerson;
    QString mLastErrorString;
};

inline AbstractModelNetworkServerConnection::Private::Private( AbstractModelNetworkServerConnection* parent )
 : p( parent ), mSocket( 0 ), mState( Unconnected )
{
}

inline int AbstractModelNetworkServerConnection::Private::error() const { return 0; }
inline QString AbstractModelNetworkServerConnection::Private::errorString() const { return mLastErrorString; }
inline void AbstractModelNetworkServerConnection::Private::setErrorString( const QString& errorString )
{
    mLastErrorString = errorString;
}

inline AbstractModelNetworkServerConnection::Private::~Private() {}

#endif
