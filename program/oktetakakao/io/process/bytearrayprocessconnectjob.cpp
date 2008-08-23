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


#include "bytearrayprocessconnectjob.h"

// lib
#include "bytearrayprocessbytearraysynchronizer.h"
#include "bytearrayrawfilewritethread.h"
#include "kbytearraydocument.h"
// KDE
#include <KUrl>
// Qt
#include <QtGui/QApplication>
#include <QtCore/QDataStream>
#include <QtCore/QFile>
#include <QtCore/QTimer>

class ByteArrayProcessConnectJob::Private
{
  public:
    Private( ByteArrayProcessByteArraySynchronizer* synchronizer, KByteArrayDocument* document,
             const KUrl &url, AbstractModelSynchronizer::ConnectOption option );

  public:
    KByteArrayDocument* document() const;
    KUrl url() const;
    AbstractModelSynchronizer::ConnectOption option() const;
    ByteArrayProcessByteArraySynchronizer* synchronizer() const;

  protected:
    ByteArrayProcessByteArraySynchronizer* mSynchronizer;
    KByteArrayDocument* mDocument;
    const KUrl mUrl;
    const AbstractModelSynchronizer::ConnectOption mOption;
};

ByteArrayProcessConnectJob::Private::Private( ByteArrayProcessByteArraySynchronizer* synchronizer,
                                           KByteArrayDocument *document,
                                           const KUrl& url,
                                           AbstractModelSynchronizer::ConnectOption option )
 : mSynchronizer( synchronizer ), mDocument( document ), mUrl( url ), mOption( option )
{}

ByteArrayProcessByteArraySynchronizer* ByteArrayProcessConnectJob::Private::synchronizer() const
{
    return mSynchronizer;
}
inline KByteArrayDocument* ByteArrayProcessConnectJob::Private::document()   const { return mDocument; }
inline KUrl ByteArrayProcessConnectJob::Private::url()                       const { return mUrl; }
inline AbstractModelSynchronizer::ConnectOption ByteArrayProcessConnectJob::Private::option() const
{
    return mOption;
}



ByteArrayProcessConnectJob::ByteArrayProcessConnectJob( ByteArrayProcessByteArraySynchronizer* synchronizer,
                                                  KByteArrayDocument* document,
                                                  const KUrl& url,
                                                  AbstractModelSynchronizer::ConnectOption option )
 : d( new Private(synchronizer,document,url,option) )
{}

ByteArrayProcessByteArraySynchronizer* ByteArrayProcessConnectJob::synchronizer() const
{
    return d->synchronizer();
}
KByteArrayDocument* ByteArrayProcessConnectJob::document()   const { return d->document(); }

void ByteArrayProcessConnectJob::start()
{
    QTimer::singleShot( 0, this, SLOT(connectToProcessByteArrayObject()) );
}

void ByteArrayProcessConnectJob::connectToProcessByteArrayObject()
{
    ProcessByteArrayAdaptor* adaptor = new ProcessByteArrayAdaptor( mDocument );
    QProcessConnection process = QProcessConnection::sessionBus();
    process.registerObject( "/ByteArray", mDocument );

    qobject_cast<KByteArrayRawFileSynchronizer*>(synchronizer())->setDocument( byteArrayDocument );
}

ByteArrayProcessConnectJob::~ByteArrayProcessConnectJob() {}
