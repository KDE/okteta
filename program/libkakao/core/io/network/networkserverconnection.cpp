/*
    This file is part of the Okteta Kakao module, part of the KDE project.

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
#include "networkserverconnection.moc" // due to Q_PRIVATE_SLOT


NetworkServerConnection::NetworkServerConnection( QObject* parent )
 : QObject( parent ), d( new Private(this) )
{}


QIODevice* NetworkServerConnection::ioDevice() const { return d->ioDevice(); }
bool NetworkServerConnection::matchesServer( const KUrl& url ) const
{
    return d->matchesServer( url );
}

int NetworkServerConnection::error()           const { return d->error(); }
QString NetworkServerConnection::errorString() const { return d->errorString(); }
QList<Person> NetworkServerConnection::userList() const { return d->userList(); }


void NetworkServerConnection::startConnectToServer( const KUrl& url, const Person& person )
{
    d->startConnectToServer( url, person );
}

void NetworkServerConnection::startDisconnectFromServer()
{
    d->startDisconnectFromServer();
}

void NetworkServerConnection::addModelConnection( AbstractModelNetworkServerConnection* modelConnection )
{
    d->addModelConnection( modelConnection );
}

void NetworkServerConnection::removeModelConnection( AbstractModelNetworkServerConnection* modelConnection )
{
    d->removeModelConnection( modelConnection );
}

void NetworkServerConnection::setErrorString( const QString& errorString )
{
    d->setErrorString( errorString );
}

NetworkServerConnection::~NetworkServerConnection()
{
    delete d;
}
