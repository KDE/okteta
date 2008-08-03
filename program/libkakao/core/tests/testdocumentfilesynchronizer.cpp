/*
    This file is part of the Kakao Framework, part of the KDE project.

    Copyright 2007-2008 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "testdocumentfilesynchronizer.h"

// lib
#include "testdocumentfileloadjob.h"
#include "testdocumentfileconnectjob.h"
#include "testdocumentfilewritejob.h"
#include "testdocumentfilereloadjob.h"
#include "testdocumentfilewritetojob.h"
#include "testdocument.h"
// KDE
#include <KUrl>
// Qt
#include <QtCore/QByteArray>
#include <QtCore/QDataStream>
#include <QtCore/QFile>


TestDocumentFileSynchronizer::TestDocumentFileSynchronizer( const QByteArray &header )
 : mHeader( header )
{
    // TODO: where to catch this? who decides about this?
//     mDocument->setTitle( url.fileName() );
}

KAbstractDocument *TestDocumentFileSynchronizer::document() const { return mDocument; }


AbstractLoadJob *TestDocumentFileSynchronizer::startLoad( const KUrl &url )
{
    return new TestDocumentFileLoadJob( this, url );
}

AbstractSyncToRemoteJob *TestDocumentFileSynchronizer::startSyncToRemote()
{
    return new TestDocumentFileWriteJob( this );
}

AbstractSyncFromRemoteJob *TestDocumentFileSynchronizer::startSyncFromRemote()
{
    return new TestDocumentFileReloadJob( this );
}

AbstractSyncWithRemoteJob *TestDocumentFileSynchronizer::startSyncWithRemote( const KUrl &url, AbstractModelSynchronizer::ConnectOption option  )
{
    return new TestDocumentFileWriteToJob( this, url, option );
}

AbstractConnectJob *TestDocumentFileSynchronizer::startConnect( KAbstractDocument *document,
                                              const KUrl &url, AbstractModelSynchronizer::ConnectOption option )
{
    return new TestDocumentFileConnectJob( this, document, url, option );
}
