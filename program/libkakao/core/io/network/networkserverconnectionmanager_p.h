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

#ifndef NETWORKCLIENTCONNECTIONMANAGER_P_H
#define NETWORKCLIENTCONNECTIONMANAGER_P_H

#include "networkserverconnectionmanager.h"

// lib
#include <person.h>
// Qt
#include <QtCore/QMultiHash>


struct CallBackData {
    CallBackData( QObject* receiver, const char* methodName );

    QObject * const mReceiver;
    const char * const mMethodName;
};

inline CallBackData::CallBackData( QObject* receiver, const char* methodName )
 : mReceiver( receiver ), mMethodName( methodName )
{}


class NetworkServerConnectionManager::Private
{
  public:
    explicit Private( NetworkServerConnectionManager* parent );
    ~Private();

  public:
    void setPerson( const Person& person );
    void startCreateConnectionToServer( const KUrl& url, QObject* receiver, const char* methodName );

  public: // slots
    void onConnectedToServer();
    void onError( int errorCode );
    void onDisconnected();
    void notifyAboutExistingConnections();

  protected:
    void notifyRequesters( NetworkServerConnection* serverConnection );

  protected:
    NetworkServerConnectionManager* const p;

    Person mPerson;

    QList<NetworkServerConnection*> mServerConnectionList;

  protected:
    QMultiHash<NetworkServerConnection*,CallBackData> mWaitingCallBackList;
    QList<NetworkServerConnection*> mExistingRequestedServerConnections;
};



inline void NetworkServerConnectionManager::Private::setPerson( const Person& person )
{
    mPerson = person;
}

#endif
