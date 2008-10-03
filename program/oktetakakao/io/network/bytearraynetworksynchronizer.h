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

#ifndef BYTEARRAYNETWORKSYNCHRONIZER_H
#define BYTEARRAYNETWORKSYNCHRONIZER_H


// Kakao core
#include <abstractmodelnetworksynchronizer.h>

class ByteArrayNetworkServerConnection;
class ByteArrayNetworkServer;
class KByteArrayDocument;

class ByteArrayNetworkSynchronizer : public AbstractModelNetworkSynchronizer
{
  Q_OBJECT

  friend class ByteArrayNetworkLoadJob;

  public:
    ByteArrayNetworkSynchronizer();
    virtual ~ByteArrayNetworkSynchronizer();

  public: // AbstractModelSynchronizer API
    virtual void startOffering( KAbstractDocument* document );
    virtual AbstractLoadJob* startLoad( const KUrl& url );
    virtual AbstractSyncToRemoteJob* startSyncToRemote();
    virtual AbstractSyncFromRemoteJob* startSyncFromRemote();
    virtual AbstractSyncWithRemoteJob* startSyncWithRemote( const KUrl& url, AbstractModelSynchronizer::ConnectOption option );
    virtual AbstractConnectJob* startConnect( KAbstractDocument* document,
                                              const KUrl& url, AbstractModelSynchronizer::ConnectOption option );

    virtual KAbstractDocument* document() const;

  protected:
    void set( KByteArrayDocument* document, ByteArrayNetworkServer* processDocument );
    void set( KByteArrayDocument* document, ByteArrayNetworkServerConnection* connection );

  protected:
    KByteArrayDocument* mDocument;
    ByteArrayNetworkServer* mNetworkServer;
    ByteArrayNetworkServerConnection* mConnection;
};

#endif
