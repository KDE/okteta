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
#include "bytearrayprocessbytearrayconnector.h"
#include "bytearrayprocessdocument.h"
#include "kbytearraydocument.h"
//
#include <processdocumentmanager.h>
#include <processdocumentperson.h>
// Kakao core
#include <person.h>
// Okteta core
#include <kpiecetablebytearraymodel.h>
// KDE
#include <KLocale>
#include <KIcon>
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
    ByteArrayProcessByteArrayConnector* connector;

    const QString id = mUrl.fileName();

    ProcessDocumentManager* documentAccessManager = ProcessDocumentManager::self();
    AbstractProcessDocument* remoteProcessDocument = documentAccessManager->processDocumentById( id );

    const Person ego = Person::createEgo();
    const ProcessDocumentPerson processEgo( ego.name(), ego.faceIcon() );

    AbstractProcessDocumentConnector* abstractRemoteConnector = remoteProcessDocument->createConnector( processEgo );

    if( abstractRemoteConnector )
    {
        ByteArrayProcessByteArrayConnector* remoteConnector =
            qobject_cast<ByteArrayProcessByteArrayConnector*>( abstractRemoteConnector );

        const int currentVersionIndex = remoteConnector->versionIndex();
        const int lastVersionIndex = remoteConnector->versionCount() - 1;
        const QByteArray data = remoteConnector->baseData();
        KHECore::KPieceTableByteArrayModel* byteArrayModel =
            new KHECore::KPieceTableByteArrayModel( data.constData(), data.size(), false );

        const QList<KHECore::ByteArrayChange> changes =
            remoteConnector->changes( 0, lastVersionIndex ); // TODO: 0, -1 as default values, -1 means till end
        byteArrayModel->doChanges( changes, 0, lastVersionIndex );
        byteArrayModel->revertToVersionByIndex( currentVersionIndex );

        byteArrayModel->setModified( false ); // TODO: set to modified state or remote

        document = new KByteArrayDocument( byteArrayModel, i18nc("destination of the byte array", "Connected to.") );
        // TODO: make KPieceTableByteArrayModel a child by constructor argument parent

        document->setTitle( i18n("Remote: %1",remoteConnector->title()) );
        document->addUsers( remoteConnector->userList() );

        connector = new ByteArrayProcessByteArrayConnector( document );
        connector->connectTo( remoteConnector );

        // TODO: should be done by the connector to the remote document
        QList<Person> egos;
        egos.append( ego );
        document->addUsers( egos );
    }
    else
    {
        document = 0;
    }

    mSynchronizer->set( document, connector );
    setDocument( document );
}

ByteArrayProcessLoadJob::~ByteArrayProcessLoadJob() {}
