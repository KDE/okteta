/*
    This file is part of the Kasten Framework, made within the KDE community.

    Copyright 2006-2007,2009,2011,2019 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "documentcreatemanager.hpp"
#include "documentcreatemanager_p.hpp"

namespace Kasten {

DocumentCreateManager::DocumentCreateManager(DocumentManager* manager)
    : d_ptr(new DocumentCreateManagerPrivate(manager))
{
}

DocumentCreateManager::~DocumentCreateManager() = default;

bool DocumentCreateManager::canCreateNewFromData(const QMimeData* mimeData) const
{
    Q_D(const DocumentCreateManager);

    return d->canCreateNewFromData(mimeData);
}

void DocumentCreateManager::setDocumentFactory(AbstractDocumentFactory* factory)
{
    Q_D(DocumentCreateManager);

    d->setDocumentFactory(factory);
}

void DocumentCreateManager::createNew() const
{
    Q_D(const DocumentCreateManager);

    d->createNew();
}

void DocumentCreateManager::createNewFromData(const QMimeData* mimeData, bool setModified) const
{
    Q_D(const DocumentCreateManager);

    d->createNewFromData(mimeData, setModified);
}

}
