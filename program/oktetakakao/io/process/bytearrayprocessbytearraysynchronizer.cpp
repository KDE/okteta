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

#include "bytearrayprocessbytearraysynchronizer.h"

// lib
#include "bytearrayprocessloadjob.h"
#include "processbytearrayadaptor.h"
#include "kbytearraydocument.h"
// DocumentAccess
#include <processdocumentmanager.h>
// Okteta core
#include <kpiecetablebytearraymodel.h>
// KDE
#include <KUrl>
#include <KLocale>
// Qt
#include <QtGui/QApplication>


#include <KDebug>

ByteArrayProcessByteArraySynchronizer::ByteArrayProcessByteArraySynchronizer()
 : mDocument( 0 ), mAdaptor( 0 )
{
}

void ByteArrayProcessByteArraySynchronizer::set( KByteArrayDocument* document, ProcessByteArrayAdaptor* adaptor )
{
    mDocument = document;
    mAdaptor = adaptor;
}


KAbstractDocument* ByteArrayProcessByteArraySynchronizer::document() const { return mDocument; }

void ByteArrayProcessByteArraySynchronizer::startOffering( KAbstractDocument* document )
{
    mDocument = qobject_cast<KByteArrayDocument*>( document );
    mAdaptor = new ProcessByteArrayAdaptor( mDocument );
    ProcessDocumentManager::self()->addProcessDocument( mAdaptor );
    mDocument->setLiveSynchronizer( this );
}

AbstractLoadJob* ByteArrayProcessByteArraySynchronizer::startLoad( const KUrl& url )
{
    return new ByteArrayProcessLoadJob( this, url );
}

AbstractSyncToRemoteJob* ByteArrayProcessByteArraySynchronizer::startSyncToRemote()
{
    return 0; //new ByteArrayRawFileWriteJob( this );
}

AbstractSyncFromRemoteJob* ByteArrayProcessByteArraySynchronizer::startSyncFromRemote()
{
    return 0; //new ByteArrayRawFileReloadJob( this );
}

AbstractSyncWithRemoteJob* ByteArrayProcessByteArraySynchronizer::startSyncWithRemote( const KUrl& url, AbstractModelSynchronizer::ConnectOption option  )
{
    Q_UNUSED( url )
    Q_UNUSED( option )
    return 0; //new ByteArrayRawFileWriteToJob( this, url, option );
}

AbstractConnectJob* ByteArrayProcessByteArraySynchronizer::startConnect( KAbstractDocument* document,
                                              const KUrl& url, AbstractModelSynchronizer::ConnectOption option )
{
    Q_UNUSED( document )
    Q_UNUSED( url )
    Q_UNUSED( option )
    return 0; //new ByteArrayRawFileConnectJob( this, document, url, option );
}

ByteArrayProcessByteArraySynchronizer::~ByteArrayProcessByteArraySynchronizer()
{
    if( mAdaptor )
        ProcessDocumentManager::self()->closeProcessDocument( mAdaptor );
}
