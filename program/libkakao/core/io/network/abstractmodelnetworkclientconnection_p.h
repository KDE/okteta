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

#ifndef ABSTRACTMODELNETWORKCLIENTCONNECTION_P_H
#define ABSTRACTMODELNETWORKCLIENTCONNECTION_P_H

#include "abstractmodelnetworkclientconnection.h"


class AbstractModelNetworkClientConnection::Private
{
  public:
    Private( NetworkClientConnection* clientConnection, AbstractModelNetworkClientConnection* parent );
    ~Private();

  public:
    NetworkClientConnection* clientConnection() const;

  protected:
    AbstractModelNetworkClientConnection* const p;
    NetworkClientConnection* mClientConnection;
};

inline AbstractModelNetworkClientConnection::Private::Private( NetworkClientConnection* clientConnection, AbstractModelNetworkClientConnection* parent )
 : p( parent ), mClientConnection( clientConnection )
{
}

inline NetworkClientConnection* AbstractModelNetworkClientConnection::Private::clientConnection()  const
{
    return mClientConnection;
}

inline AbstractModelNetworkClientConnection::Private::~Private() {}

#endif
