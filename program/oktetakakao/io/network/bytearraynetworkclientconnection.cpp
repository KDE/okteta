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

#include "bytearraynetworkclientconnection.h"

// lib
#include <kbytearraydocument.h>
#include <changelistimpulsivestreamwriter.h>
// Kakao core
#include <networkclientconnection.h>
#include <person.h>
#include <qbytearrayimpulsivestreamwriter.h>
// Okteta core
#include <kpiecetablebytearraymodel.h>
// Qt
#include <QtNetwork/QTcpSocket>


#include <KDebug>

ByteArrayNetworkClientConnection::ByteArrayNetworkClientConnection( KByteArrayDocument* document, NetworkClientConnection* clientConnection, QObject* parent )
 : AbstractModelNetworkClientConnection( clientConnection, parent ),
   mByteArrayDocument( document )
{
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
    QList<Person> newUserList;
    newUserList.append( clientConnection->person() );
    mByteArrayDocument->addUsers( newUserList );
}


#if 0

void ByteArrayNetworkClientConnection::disconnectFrom()
{
    mOtherConnector = 0;
    KHECore::KAbstractByteArrayModel* byteArrayModel = mByteArrayDocument ? mByteArrayDocument->content() : 0;
    if( byteArrayModel )
        byteArrayModel->disconnect( this );
}

void ByteArrayNetworkClientConnection::doChanges( const QList<KHECore::ByteArrayChange>& changes,
                                                    int oldVersionIndex, int newVersionIndex )
{
    KHECore::KAbstractByteArrayModel* byteArrayModel = mByteArrayDocument ? mByteArrayDocument->content() : 0;
    KHECore::ChangeHistory* changeHistory =
        byteArrayModel ? qobject_cast<KHECore::ChangeHistory*>( byteArrayModel ) : 0;
    if( changeHistory )
        changeHistory->doChanges( changes, oldVersionIndex, newVersionIndex );
}

void ByteArrayNetworkClientConnection::revertToVersionByIndex( int versionIndex )
{
    KHECore::KAbstractByteArrayModel* byteArrayModel = mByteArrayDocument ? mByteArrayDocument->content() : 0;
    KHECore::Versionable* versionable =
        byteArrayModel ? qobject_cast<KHECore::Versionable*>( byteArrayModel ) : 0;
    if( versionable )
        versionable->revertToVersionByIndex( versionIndex );
}

void ByteArrayNetworkClientConnection::addUsers( const QList<Person>& users )
{
    mChangingUsers = true;
    mByteArrayDocument->addUsers( users );
    mChangingUsers = false;
}

void ByteArrayNetworkClientConnection::removeUsers( const QList<Person>& users )
{
    mChangingUsers = true;
    mByteArrayDocument->removeUsers( users );
    mChangingUsers = false;
}

#endif

void ByteArrayNetworkClientConnection::sendTitle() const
{
//     *dataStream << mByteArrayDocument->title();
}

#if 0
QList<Person> ByteArrayNetworkClientConnection::sendUserList() const
{
    *dataStream << QList<Person>();//mByteArrayDocument->userList();
}
#endif
void ByteArrayNetworkClientConnection::sendVersionStatus() const
{
//     *dataStream << mByteArrayDocument->versionCount();
//     *dataStream << mByteArrayDocument->versionIndex();
}

void ByteArrayNetworkClientConnection::sendBaseData() const
{
    KHECore::KAbstractByteArrayModel* byteArrayModel = mByteArrayDocument->content();
    KHECore::ChangeHistory* changeHistory =
        byteArrayModel ? qobject_cast<KHECore::ChangeHistory*>( byteArrayModel ) : 0;

    QByteArrayImpulsiveStreamWriter byteArrayWriter( changeHistory ? changeHistory->initialData() : QByteArray() );
//     byteArrayWriter.writeTo( socket() );
}

void ByteArrayNetworkClientConnection::sendChanges( int firstVersionIndex, int lastVersionIndex ) const
{
    KHECore::KAbstractByteArrayModel* byteArrayModel = mByteArrayDocument ? mByteArrayDocument->content() : 0;
    KHECore::ChangeHistory* changeHistory =
        byteArrayModel ? qobject_cast<KHECore::ChangeHistory*>( byteArrayModel ) : 0;

    ChangeListImpulsiveStreamWriter changeListWriter( changeHistory ? changeHistory->changes(firstVersionIndex,lastVersionIndex) :
                     QList<KHECore::ByteArrayChange>() );
}

void ByteArrayNetworkClientConnection::onChangesDone( const QList<KHECore::ByteArrayChange>& changes,
                                                      int oldVersionIndex, int newVersionIndex )
{
//         out.setVersion(QDataStream::Qt_4_4);
//     send( QString( "Changed version from %1 to %2\r\n").arg(oldVersionIndex).arg(newVersionIndex).toLatin1() );
//     if( mOtherConnector )
//         mOtherConnector->doChanges( changes, oldVersionIndex, newVersionIndex );
}

void ByteArrayNetworkClientConnection::onRevertedToVersionIndex( int versionIndex )
{
//     if( mOtherConnector )
//         mOtherConnector->revertToVersionByIndex( versionIndex );
}

void ByteArrayNetworkClientConnection::onUsersAdded( const QList<Person>& users )
{
//     if( mOtherConnector && !mChangingUsers )
//         mOtherConnector->addUsers( users );
}

void ByteArrayNetworkClientConnection::onUsersRemoved( const QList<Person>& users )
{
//     if( mOtherConnector && !mChangingUsers )
//         mOtherConnector->removeUsers( users );
}

ByteArrayNetworkClientConnection::~ByteArrayNetworkClientConnection()
{
    QList<Person> goneUserList;
    goneUserList.append( clientConnection()->person() );
    mByteArrayDocument->removeUsers( goneUserList );
}
