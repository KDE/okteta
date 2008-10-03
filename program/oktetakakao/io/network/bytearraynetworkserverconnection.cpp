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

#include "bytearraynetworkserverconnection.h"

// lib
#include "kbytearraydocument.h"
// Kakao core
#include <person.h>
// Okteta core
#include <kpiecetablebytearraymodel.h>


#include <KDebug>

ByteArrayNetworkServerConnection::ByteArrayNetworkServerConnection( NetworkServerConnection* serverConnection, int remoteModelHandle, QObject* parent )
 : AbstractModelNetworkServerConnection( serverConnection, remoteModelHandle, parent )
{
}
#if 0
QString ByteArrayNetworkServerConnection::title()
{
    return QString();//mByteArrayDocument->title();
}
int ByteArrayNetworkServerConnection::versionIndex()
{
    return 0;//mByteArrayDocument->versionIndex();
}
QList<Person> ByteArrayNetworkServerConnection::userList()
{
    return QList<Person>();//mByteArrayDocument->userList();
}
int ByteArrayNetworkServerConnection::versionCount()
{
    return 0;//mByteArrayDocument->versionCount();
}

QByteArray ByteArrayNetworkServerConnection::baseData()
{
    KHECore::KAbstractByteArrayModel* byteArrayModel = mByteArrayDocument->content();
    KHECore::ChangeHistory* changeHistory =
        byteArrayModel ? qobject_cast<KHECore::ChangeHistory*>( byteArrayModel ) : 0;

    return changeHistory ? changeHistory->initialData() : QByteArray();
}

QList<KHECore::ByteArrayChange> ByteArrayNetworkServerConnection::changes( int firstVersionIndex, int lastVersionIndex ) const
{
    KHECore::KAbstractByteArrayModel* byteArrayModel = mByteArrayDocument ? mByteArrayDocument->content() : 0;
    KHECore::ChangeHistory* changeHistory =
        byteArrayModel ? qobject_cast<KHECore::ChangeHistory*>( byteArrayModel ) : 0;

    return changeHistory ? changeHistory->changes(firstVersionIndex,lastVersionIndex) : QList<KHECore::ByteArrayChange>();
}

void ByteArrayNetworkServerConnection::connectTo( ByteArrayNetworkServerConnection* otherConnector )
{
    if( mOtherConnector == otherConnector )
        return;

    mOtherConnector = otherConnector;

    KHECore::KAbstractByteArrayModel* byteArrayModel = mByteArrayDocument ? mByteArrayDocument->content() : 0;
    if( byteArrayModel )
    {
        connect( byteArrayModel, SIGNAL(changesDone( const QList<KHECore::ByteArrayChange>&, int, int )),
                 SLOT(onChangesDone( const QList<KHECore::ByteArrayChange>&, int, int )) );
        connect( byteArrayModel, SIGNAL(revertedToVersionIndex( int )),
                 SLOT(onRevertedToVersionIndex( int )) );
        connect( mByteArrayDocument, SIGNAL(usersAdded( const QList<Person>& )),
                 SLOT(onUsersAdded( const QList<Person>& )) );
        connect( mByteArrayDocument, SIGNAL(usersRemoved( const QList<Person>& )),
                 SLOT(onUsersRemoved( const QList<Person>& )) );
    }

    mOtherConnector->connectTo( this );
}

void ByteArrayNetworkServerConnection::disconnectFrom()
{
    mOtherConnector = 0;
    KHECore::KAbstractByteArrayModel* byteArrayModel = mByteArrayDocument ? mByteArrayDocument->content() : 0;
    if( byteArrayModel )
        byteArrayModel->disconnect( this );
}

void ByteArrayNetworkServerConnection::doChanges( const QList<KHECore::ByteArrayChange>& changes,
                                                    int oldVersionIndex, int newVersionIndex )
{
    KHECore::KAbstractByteArrayModel* byteArrayModel = mByteArrayDocument ? mByteArrayDocument->content() : 0;
    KHECore::ChangeHistory* changeHistory =
        byteArrayModel ? qobject_cast<KHECore::ChangeHistory*>( byteArrayModel ) : 0;
    if( changeHistory )
        changeHistory->doChanges( changes, oldVersionIndex, newVersionIndex );
}

void ByteArrayNetworkServerConnection::revertToVersionByIndex( int versionIndex )
{
    KHECore::KAbstractByteArrayModel* byteArrayModel = mByteArrayDocument ? mByteArrayDocument->content() : 0;
    KHECore::Versionable* versionable =
        byteArrayModel ? qobject_cast<KHECore::Versionable*>( byteArrayModel ) : 0;
    if( versionable )
        versionable->revertToVersionByIndex( versionIndex );
}

void ByteArrayNetworkServerConnection::addUsers( const QList<Person>& users )
{
    mChangingUsers = true;
    mByteArrayDocument->addUsers( users );
    mChangingUsers = false;
}

void ByteArrayNetworkServerConnection::removeUsers( const QList<Person>& users )
{
    mChangingUsers = true;
    mByteArrayDocument->removeUsers( users );
    mChangingUsers = false;
}

void ByteArrayNetworkServerConnection::onChangesDone( const QList<KHECore::ByteArrayChange>& changes,
                                                        int oldVersionIndex, int newVersionIndex )
{
    if( mOtherConnector )
        mOtherConnector->doChanges( changes, oldVersionIndex, newVersionIndex );
}

void ByteArrayNetworkServerConnection::onRevertedToVersionIndex( int versionIndex )
{
    if( mOtherConnector )
        mOtherConnector->revertToVersionByIndex( versionIndex );
}

void ByteArrayNetworkServerConnection::onUsersAdded( const QList<Person>& users )
{
    if( mOtherConnector && !mChangingUsers )
        mOtherConnector->addUsers( users );
}

void ByteArrayNetworkServerConnection::onUsersRemoved( const QList<Person>& users )
{
    if( mOtherConnector && !mChangingUsers )
        mOtherConnector->removeUsers( users );
}
#endif

ByteArrayNetworkServerConnection::~ByteArrayNetworkServerConnection()
{
}
