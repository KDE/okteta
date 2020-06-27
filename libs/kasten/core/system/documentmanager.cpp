/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2006-2007, 2009, 2011, 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "documentmanager.hpp"
#include "documentmanager_p.hpp"

namespace Kasten {

DocumentManager::DocumentManager()
    : d_ptr(new DocumentManagerPrivate(this))
{
}

DocumentManager::~DocumentManager() = default;

DocumentCreateManager* DocumentManager::createManager() const
{
    Q_D(const DocumentManager);

    return d->createManager();
}

DocumentSyncManager* DocumentManager::syncManager() const
{
    Q_D(const DocumentManager);

    return d->syncManager();
}
ModelCodecManager* DocumentManager::codecManager() const
{
    Q_D(const DocumentManager);

    return d->codecManager();
}

QVector<AbstractDocument*> DocumentManager::documents() const
{
    Q_D(const DocumentManager);

    return d->documents();
}

bool DocumentManager::isEmpty() const
{
    Q_D(const DocumentManager);

    return d->isEmpty();
}

void DocumentManager::addDocument(AbstractDocument* document)
{
    Q_D(DocumentManager);

    d->addDocument(document);
}

void DocumentManager::closeDocument(AbstractDocument* document)
{
    Q_D(DocumentManager);

    d->closeDocument(document);
}

void DocumentManager::closeDocuments(const QVector<AbstractDocument*>& documents)
{
    Q_D(DocumentManager);

    d->closeDocuments(documents);
}

void DocumentManager::closeAll()
{
    Q_D(DocumentManager);

    d->closeAll();
}

void DocumentManager::closeAllOther(AbstractDocument* keptDocument)
{
    Q_D(DocumentManager);

    d->closeAllOther(keptDocument);
}

bool DocumentManager::canClose(AbstractDocument* document) const
{
    Q_D(const DocumentManager);

    return d->canClose(document);
}

bool DocumentManager::canClose(const QVector<AbstractDocument*>& documents) const
{
    Q_D(const DocumentManager);

    return d->canClose(documents);
}

bool DocumentManager::canCloseAll() const
{
    Q_D(const DocumentManager);

    return d->canCloseAll();
}

bool DocumentManager::canCloseAllOther(AbstractDocument* keptDocument) const
{
    Q_D(const DocumentManager);

    return d->canCloseAllOther(keptDocument);
}

void DocumentManager::requestFocus(AbstractDocument* document)
{
    Q_D(DocumentManager);

    d->requestFocus(document);
}

}
