/*
    This file is part of the Kakao Framework, part of the KDE project.

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


#include "modelservermanager.h"
#include "modelservermanager_p.h"
#include "modelservermanager.moc" // due to Q_PRIVATE_SLOT

// KDE
#include <KGlobal>


K_GLOBAL_STATIC( ModelServerManager, managerInstance )

ModelServerManager* ModelServerManager::self()
{
    return managerInstance;
}


ModelServerManager::ModelServerManager()
 : QObject( 0 ), d( new Private(this) )
{}

bool ModelServerManager::isPublished() const { return d->isPublished(); }
int ModelServerManager::clientsCountLimit() const { return d->clientsCountLimit(); }
QList<AbstractModelNetworkServer*> ModelServerManager::serverList() const { return d->serverList(); }

void ModelServerManager::setIsPublished( bool isPublished ) { d->setIsPublished( isPublished ); }
void ModelServerManager::setClientsCountLimit( int clientsCountLimit )
{
    d->setClientsCountLimit( clientsCountLimit );
}

void ModelServerManager::addServer( AbstractModelNetworkServer* server ) { d->addServer( server ); }
void ModelServerManager::removeServer( AbstractModelNetworkServer* server ) { d->removeServer( server ); }

ModelServerManager::~ModelServerManager()
{
    delete d;
}
