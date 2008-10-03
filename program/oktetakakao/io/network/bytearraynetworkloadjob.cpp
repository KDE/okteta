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

#include "bytearraynetworkloadjob.h"

// lib
#include "bytearraynetworksynchronizer.h"
#include "bytearraynetworkserverconnection.h"
#include "kbytearraydocument.h"
// Kakao core
#include <person.h>
#include <networkserverconnectionmanager.h>
// Okteta core
#include <kpiecetablebytearraymodel.h>
// KDE
#include <KLocale>
#include <KIcon>
// Qt
#include <QtGui/QApplication>
#include <QtCore/QTimer>

#include <KDebug>

// TODO: add properties to document: shared since, last activity, in display use relative date
ByteArrayNetworkLoadJob::ByteArrayNetworkLoadJob( ByteArrayNetworkSynchronizer* synchronizer, const KUrl& url )
 : mSynchronizer( synchronizer ), mUrl( url ), mConnector( 0 )
{}

void ByteArrayNetworkLoadJob::start()
{
    QTimer::singleShot( 0, this, SLOT(loadByteArrayObject()) );
}

void ByteArrayNetworkLoadJob::loadByteArrayObject()
{
kDebug();
    const Person ego = Person::createEgo();

    NetworkServerConnectionManager* serverConnectionManager = NetworkServerConnectionManager::self();
//     connect( serverConnectionManager, SIGNAL(connectionCreated( NetworkServerConnection* )),
//              SLOT(onConnectionCreated( NetworkServerConnection* )) );
//     serverConnectionManager->startCreateConnectionToServer( mUrl );
    serverConnectionManager->setPerson( ego );
    serverConnectionManager->startCreateConnectionToServer( mUrl, this, "onConnectionCreated" );
}

void ByteArrayNetworkLoadJob::onConnectionCreated( NetworkServerConnection* serverConnection )
{
    const int remoteModelHandle = 0;

    KByteArrayDocument* document = 0;

    if( serverConnection == 0 )
    {
        // TODO: error should be reported by serverconnectionmanager
//     setError( ConnectionFailed );
//     setErrorText( serverConnectionManager->errorString() );
    }
    else
    {
        mConnector = new ByteArrayNetworkServerConnection( serverConnection, remoteModelHandle, mSynchronizer );

//         connect( mConnector, SIGNAL(connected()), SLOT(onConnectedToServer()) );
//         connect( mConnector, SIGNAL(error(int)), SLOT(onError()) );

        const int currentVersionIndex = 0;//remoteConnector->versionIndex();
        const int lastVersionIndex = 0;//remoteConnector->versionCount() - 1;
        const QByteArray data;// = remoteConnector->baseData();
        KHECore::KPieceTableByteArrayModel* byteArrayModel =
            new KHECore::KPieceTableByteArrayModel( data.constData(), data.size(), false );

        const QList<KHECore::ByteArrayChange> changes;// =
//             remoteConnector->changes( 0, lastVersionIndex ); // TODO: 0, -1 as default values, -1 means till end
        byteArrayModel->doChanges( changes, 0, lastVersionIndex );
        byteArrayModel->revertToVersionByIndex( currentVersionIndex );

        byteArrayModel->setModified( false ); // TODO: set to modified state or remote

        document = new KByteArrayDocument( byteArrayModel, i18nc("destination of the byte array", "Connected to.") );
        // TODO: make KPieceTableByteArrayModel a child by constructor argument parent

        document->setTitle( "Remotedocument");//i18n("Remote: %1",remoteConnector->title()) );
        document->addUsers( QList<Person>() );// remoteConnector->userList() );

        // TODO: should be done by the connector to the remote document
//         QList<Person> egos;
//         egos.append( ego );
//         document->addUsers( egos );
        document->setSynchronizer( mSynchronizer );
    }

    mSynchronizer->set( document, mConnector );
    setDocument( document );
}


ByteArrayNetworkLoadJob::~ByteArrayNetworkLoadJob() {}
