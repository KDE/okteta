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
#include "networkserverconnection.h"


class AbstractModelNetworkServerConnection::Private
{
  public:
    Private( NetworkServerConnection* serverConnection, int remoteModelHandle,
             AbstractModelNetworkServerConnection* parent );
    ~Private();

  public:
    NetworkServerConnection* serverConnection() const;
    int remoteModelHandle() const;

  protected:
    NetworkServerConnection* mServerConnection;
    int mRemoteModelHandle;
    AbstractModelNetworkServerConnection* const p;

};

inline AbstractModelNetworkServerConnection::Private::Private( NetworkServerConnection* serverConnection,
                                                               int remoteModelHandle,
                                                               AbstractModelNetworkServerConnection* parent )
 : mServerConnection( serverConnection ), mRemoteModelHandle( remoteModelHandle ), p( parent )
{
    mServerConnection->addModelConnection( p );
}

inline NetworkServerConnection* AbstractModelNetworkServerConnection::Private::serverConnection() const
{
    return mServerConnection;
}
inline int AbstractModelNetworkServerConnection::Private::remoteModelHandle() const
{
    return mRemoteModelHandle;
}

inline AbstractModelNetworkServerConnection::Private::~Private()
{
    mServerConnection->removeModelConnection( p );
}

#endif
