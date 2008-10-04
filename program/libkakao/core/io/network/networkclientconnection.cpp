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


#include "networkclientconnection.h"
#include "networkclientconnection_p.h"
#include "networkclientconnection.moc" // due to Q_PRIVATE_SLOT


NetworkClientConnection::NetworkClientConnection( QObject* parent )
 : QObject( parent ), d( new Private(this) )
{}


void NetworkClientConnection::startConnectToClient( QTcpSocket* tcpSocket )
{
    d->startConnectToClient( tcpSocket );
}

void NetworkClientConnection::startDisconnectFromClient()
{
    d->startDisconnectFromClient();
}

Person NetworkClientConnection::person()       const { return d->person(); }
QIODevice* NetworkClientConnection::ioDevice() const { return d->ioDevice(); }

int NetworkClientConnection::error()           const { return d->error(); }
QString NetworkClientConnection::errorString() const { return d->errorString(); }
void NetworkClientConnection::setErrorString( const QString& errorString )
{
    d->setErrorString( errorString );
}

NetworkClientConnection::~NetworkClientConnection()
{
    delete d;
}
