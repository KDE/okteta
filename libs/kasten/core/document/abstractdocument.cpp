/*
    This file is part of the Kasten Framework, part of the KDE project.

    Copyright 2007-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "abstractdocument.h"
#include "abstractdocument_p.h"


namespace Kasten
{

AbstractDocument::AbstractDocument()
  : d( new AbstractDocumentPrivate(this) )
{}

QString AbstractDocument::id() const { return d->id(); }
AbstractModelSynchronizer* AbstractDocument::synchronizer() const { return d->synchronizer(); }
AbstractModelSynchronizer* AbstractDocument::liveSynchronizer() const { return d->liveSynchronizer(); }

void AbstractDocument::setId( const QString& id ) { d->setId(id); }
void AbstractDocument::setSynchronizer( AbstractModelSynchronizer* synchronizer ) { d->setSynchronizer(synchronizer); }
void AbstractDocument::setLiveSynchronizer( AbstractModelSynchronizer* synchronizer ) { d->setLiveSynchronizer(synchronizer); }

bool AbstractDocument::hasLocalChanges() const { return syncStates().testFlag(LocalHasChanges) ; }

AbstractDocument::~AbstractDocument()
{
    delete d;
}

}
