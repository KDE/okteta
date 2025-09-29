/*
    SPDX-FileCopyrightText: 2006-2007, 2009, 2011, 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "documentcreatemanager.hpp"
#include "documentcreatemanager_p.hpp"

// Std
#include <utility>

namespace Kasten {

DocumentCreateManager::DocumentCreateManager(DocumentManager* manager)
    : d_ptr(std::make_unique<DocumentCreateManagerPrivate>(manager))
{
}

DocumentCreateManager::~DocumentCreateManager() = default;

bool DocumentCreateManager::canCreateNewFromData(const QMimeData* mimeData) const
{
    Q_D(const DocumentCreateManager);

    return d->canCreateNewFromData(mimeData);
}

void DocumentCreateManager::setDocumentFactory(std::unique_ptr<AbstractDocumentFactory>&& factory)
{
    Q_D(DocumentCreateManager);

    d->setDocumentFactory(std::move(factory));
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

#include "moc_documentcreatemanager.cpp"
