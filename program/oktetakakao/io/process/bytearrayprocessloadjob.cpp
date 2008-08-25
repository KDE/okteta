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

#include "bytearrayprocessloadjob.h"

// lib
#include "bytearrayprocessbytearraysynchronizer.h"
#include "processbytearrayadaptor.h"
#include "kbytearraydocument.h"
//
#include <processdocumentmanager.h>
// Okteta core
#include <kpiecetablebytearraymodel.h>
// KDE
#include <KLocale>
// Qt
#include <QtGui/QApplication>
#include <QtCore/QTimer>

#include <KDebug>

ByteArrayProcessLoadJob::ByteArrayProcessLoadJob( ByteArrayProcessByteArraySynchronizer* synchronizer, const KUrl& url )
 : mSynchronizer( synchronizer ), mUrl( url )
{}

void ByteArrayProcessLoadJob::start()
{
    QTimer::singleShot( 0, this, SLOT(loadProcessByteArrayObject()) );
}

void ByteArrayProcessLoadJob::loadProcessByteArrayObject()
{
    KByteArrayDocument* document;
    ProcessByteArrayAdaptor* adaptor;

    ProcessDocumentManager* documentAccessManager = ProcessDocumentManager::self();
    const QString id = mUrl.fileName();
    AbstractProcessDocument* processDocumentById = documentAccessManager->processDocumentById( id );
    ProcessByteArrayAdaptor* remoteAdaptor = qobject_cast<ProcessByteArrayAdaptor*>( processDocumentById );

    if( remoteAdaptor )
    {
        const int currentVersionIndex = remoteAdaptor->versionIndex();
        const int lastVersionIndex = remoteAdaptor->versionCount() - 1;
        const QByteArray data = remoteAdaptor->baseData();
        KHECore::KPieceTableByteArrayModel* byteArrayModel =
            new KHECore::KPieceTableByteArrayModel( data.constData(), data.size(), false );

        const QList<KHECore::ByteArrayChange> changes =
            remoteAdaptor->changes( 0, lastVersionIndex ); // TODO: 0, -1 as default values, -1 means till end
        byteArrayModel->doChanges( changes, 0, lastVersionIndex );
        byteArrayModel->revertToVersionByIndex( currentVersionIndex );

        byteArrayModel->setModified( false ); // TODO: set to modified state or remote

        document = new KByteArrayDocument( byteArrayModel, i18nc("destination of the byte array", "Connected to.") );
        // TODO: make KPieceTableByteArrayModel a child by constructor argument parent

        document->setTitle( i18n("Remote: %1",remoteAdaptor->title()) );

        adaptor = new ProcessByteArrayAdaptor( document );
        connect( remoteAdaptor, SIGNAL(changesDone( const QList<KHECore::ByteArrayChange>&, int, int )),
                 adaptor, SLOT(onChangesDone( const QList<KHECore::ByteArrayChange>&, int, int )) );
        connect( remoteAdaptor, SIGNAL(revertedToVersionIndex( int )),
                 adaptor, SLOT(onRevertedToVersionIndex( int )) );
        connect( adaptor, SIGNAL(changesDone( const QList<KHECore::ByteArrayChange>&, int, int )),
                 remoteAdaptor, SLOT(onChangesDone( const QList<KHECore::ByteArrayChange>&, int, int )) );
        connect( adaptor, SIGNAL(revertedToVersionIndex( int )),
                 remoteAdaptor, SLOT(onRevertedToVersionIndex( int )) );
    }
    else
    {
        document = 0;
    }

    mSynchronizer->set( document, adaptor );
    setDocument( document );
}

ByteArrayProcessLoadJob::~ByteArrayProcessLoadJob() {}
