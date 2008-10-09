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

#ifndef NETWORKCLIENTCONNECTION_P_H
#define NETWORKCLIENTCONNECTION_P_H

#include "networkclientconnection.h"

// lib
#include <person.h>
// Qt
#include <QtNetwork/QTcpSocket>
#include <QtCore/QHash>

class AbstractModelNetworkClientConnection;
class AbstractImpulsiveStreamReader;


class NetworkClientConnection::Private
{
  public:
    enum State { Unconnected, WaitingForHandshake, WaitingForUserDetails };

  public:
    Private( NetworkClientConnection* parent );
    ~Private();

  public:
    void startConnectToClient( QTcpSocket* tcpSocket );
    void startDisconnectFromClient();
    void setErrorString( const QString& errorString );

  public:
    QIODevice* ioDevice() const;
    int error() const;
    QString errorString() const;
    Person person() const;

  public: // Q_SLOTS
    void onSocketReadyRead();
    void onSocketError( QAbstractSocket::SocketError socketError );
    void onSocketDisconnected();

  protected:
    void sendHandshake();

    void handleSetupReply();
    void handleUserDetails();

  protected:
    NetworkClientConnection* const p;

    quint32 mId;
    QTcpSocket* mSocket;
    State mState;
    AbstractImpulsiveStreamReader* mCurrentStreamReader;

    Person mPerson;
    QString mLastErrorString;

    QHash<int,AbstractModelNetworkClientConnection*> mModelClientConnectionList;
};

inline NetworkClientConnection::Private::Private( NetworkClientConnection* parent )
 : p( parent ), mSocket( 0 ), mState( Unconnected ), mCurrentStreamReader( 0 )
{
}

inline QIODevice* NetworkClientConnection::Private::ioDevice() const { return mSocket; }
inline Person NetworkClientConnection::Private::person()       const { return mPerson; }
inline int NetworkClientConnection::Private::error()           const { return -1; }
inline QString NetworkClientConnection::Private::errorString() const { return mLastErrorString; }

inline void NetworkClientConnection::Private::setErrorString( const QString& errorString )
{
    mLastErrorString = errorString;
}

#endif
