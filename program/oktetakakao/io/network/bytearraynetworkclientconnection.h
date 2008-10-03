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

#ifndef BYTEARRAYNETWORKCLIENTCONNECTION_H
#define BYTEARRAYNETWORKCLIENTCONNECTION_H


// Kakao core
#include <abstractmodelnetworkclientconnection.h>

class KByteArrayDocument;
namespace KHECore {
class ByteArrayChange;
}
template <class T> class QList;

class ByteArrayNetworkClientConnection : public AbstractModelNetworkClientConnection
{
  Q_OBJECT

  public:
    ByteArrayNetworkClientConnection( KByteArrayDocument* document, NetworkClientConnection* clientConnection, QObject* parent = 0 );
    virtual ~ByteArrayNetworkClientConnection();

  public: //TODO: used to fetch start data, but perhaps the document construction should be done internal?
//     QList<Person> userList();

  protected: // AbstractModelNetworkClientConnection API
//     virtual void receive( const QByteArray& message );

  protected:
    void sendTitle() const;
    void sendVersionStatus() const;
    void sendBaseData() const;
    void sendChanges( int firstVersionIndex, int lastVersionIndex ) const;

  protected Q_SLOTS:
    void onChangesDone( const QList<KHECore::ByteArrayChange>& changes,
                        int oldVersionIndex, int newVersionIndex );
    void onRevertedToVersionIndex( int versionIndex );
    void onUsersAdded( const QList<Person>& newUserList );
    void onUsersRemoved( const QList<Person>& newUserList );

  protected:
    KByteArrayDocument* mByteArrayDocument;
//     bool mChangingUsers : 1;
};

#endif
