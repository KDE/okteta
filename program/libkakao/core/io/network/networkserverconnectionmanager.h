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

#ifndef NETWORKCLIENTCONNECTIONMANAGER_H
#define NETWORKCLIENTCONNECTIONMANAGER_H

// Qt
#include <QtCore/QObject>

class NetworkServerConnection;
class Person;
class KUrl;

class NetworkServerConnectionManager : public QObject
{
    Q_OBJECT

  public:
    static NetworkServerConnectionManager* self();

  public:
    NetworkServerConnectionManager(); // TODO: make this protected
  public:
    virtual ~NetworkServerConnectionManager();

  public:
    // TODO: methodName should be SLOT() for consistency, but invokeMethod does not operate that way
    void startCreateConnectionToServer( const KUrl& url, QObject* receiver, const char* methodName );
    void setPerson( const Person& person );

//   Q_SIGNALS:
//     void connectionCreated( NetworkServerConnection* connection );
//     void connectionFailed(); // TODO: how to signal best which server?

  private:
    Q_PRIVATE_SLOT( d, void onConnectedToServer() )
    Q_PRIVATE_SLOT( d, void onError( int errorCode ) )
    Q_PRIVATE_SLOT( d, void onDisconnected() )
    Q_PRIVATE_SLOT( d, void notifyAboutExistingConnections() )

  protected:
    class Private;
    Private* const d;
};

#endif
