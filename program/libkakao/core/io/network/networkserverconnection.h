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

#ifndef NETWORKSERVERCONNECTION_H
#define NETWORKSERVERCONNECTION_H

// Qt
#include <QtCore/QObject>

class AbstractModelNetworkServerConnection;
class Person;
class KUrl;
class QIODevice;
template<class T> class QList;


class NetworkServerConnection : public QObject
{
    Q_OBJECT

  public:
    explicit NetworkServerConnection( QObject* parent = 0 );
    virtual ~NetworkServerConnection();

  public: //
    void startConnectToServer( const KUrl& url, const Person& person );
    void startDisconnectFromServer();

    void addModelConnection( AbstractModelNetworkServerConnection* modelConnection );
    void removeModelConnection( AbstractModelNetworkServerConnection* modelConnection );

  public:
    QIODevice* ioDevice() const;
    bool matchesServer( const KUrl& url ) const;
    int error() const;
    QString errorString() const;
    QList<Person> userList() const;

  Q_SIGNALS:
    void connected();
    void disconnected();
    void error( int errorCode );

  protected:
    void setErrorString( const QString& errorString );

  private:
    Q_PRIVATE_SLOT( d, void onSocketConnected() )
    Q_PRIVATE_SLOT( d, void onSocketReadyRead() )
    Q_PRIVATE_SLOT( d, void onSocketError(QAbstractSocket::SocketError socketError) )
    Q_PRIVATE_SLOT( d, void onSocketDisconnected() )

  protected:
    class Private;
    friend class Private;
    Private* const d;
};

#endif
