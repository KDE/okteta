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

#ifndef ABSTRACTNETWORKCONNECTION_P_H
#define ABSTRACTNETWORKCONNECTION_P_H

#include "abstractnetworkconnection.h"

// Qt
#include <QtNetwork/QAbstractSocket>

class QTcpSocket;


class AbstractNetworkConnection::Private
{
  public:
    enum State { Unconnected, WaitingForHandshake, HandshakeReceived };

  public:
    Private( AbstractNetworkConnection* parent );
    ~Private();

  public:
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
    AbstractNetworkConnection* const p;

    QTcpSocket* mSocket;
    State mState;

    QString mLastErrorString;
};

inline AbstractNetworkConnection::Private::Private( AbstractNetworkConnection* parent )
 : p( parent ), mSocket( 0 ), mState( Unconnected )
{
}

inline int AbstractNetworkConnection::Private::error() const { return 0; }
inline QString AbstractNetworkConnection::Private::errorString() const { return mLastErrorString; }
inline void AbstractNetworkConnection::Private::setErrorString( const QString& errorString )
{
    mLastErrorString = errorString;
}

inline AbstractNetworkConnection::Private::~Private() {}

#endif
