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

#ifndef BYTEARRAYPROCESSBYTEARRAYSYNCHRONIZER_H
#define BYTEARRAYPROCESSBYTEARRAYSYNCHRONIZER_H


// Kakao core
#include <abstractmodelfilesystemsynchronizer.h>

class ProcessByteArrayAdaptor;
class KByteArrayDocument;

class ByteArrayProcessByteArraySynchronizer : public AbstractModelFileSystemSynchronizer
{
  Q_OBJECT

  friend class ByteArrayProcessLoadJob;

  public:
    ByteArrayProcessByteArraySynchronizer();
    virtual ~ByteArrayProcessByteArraySynchronizer();

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
    void set( KByteArrayDocument* document, ProcessByteArrayAdaptor* adaptor );

  protected:
    KByteArrayDocument* mDocument;
    ProcessByteArrayAdaptor* mAdaptor;
};

#endif
