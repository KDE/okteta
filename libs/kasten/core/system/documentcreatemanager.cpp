/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2006-2007, 2009, 2011, 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
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
