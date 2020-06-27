/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2007-2009, 2011, 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "documentsyncmanager.hpp"
#include "documentsyncmanager_p.hpp"

// Qt
#include <QUrl>

namespace Kasten {

DocumentSyncManager::DocumentSyncManager(DocumentManager* manager)
    : d_ptr(new DocumentSyncManagerPrivate(this, manager))
{}

DocumentSyncManager::~DocumentSyncManager() = default;

void DocumentSyncManager::setSaveDiscardDialog(AbstractSaveDiscardDialog* saveDiscardDialog)
{
    Q_D(DocumentSyncManager);

    d->setSaveDiscardDialog(saveDiscardDialog);
}

void DocumentSyncManager::setOverwriteDialog(AbstractOverwriteDialog* overwriteDialog)
{
    Q_D(DocumentSyncManager);

    d->setOverwriteDialog(overwriteDialog);
}

// TODO: make a difference between stream/storage formats and work/live formats?
QStringList DocumentSyncManager::supportedRemoteTypes() const
{
    Q_D(const DocumentSyncManager);

    return d->supportedRemoteTypes();
}

bool DocumentSyncManager::hasSynchronizerForLocal(const QString& workDocumentType) const
{
    Q_D(const DocumentSyncManager);

    return d->hasSynchronizerForLocal(workDocumentType);
}

QUrl DocumentSyncManager::urlOf(AbstractDocument* document) const
{
    Q_D(const DocumentSyncManager);

    return d->urlOf(document);
}

void DocumentSyncManager::setDocumentSynchronizerFactory(AbstractModelSynchronizerFactory* synchronizerFactory)
{
    Q_D(DocumentSyncManager);

    d->setDocumentSynchronizerFactory(synchronizerFactory);
}

void DocumentSyncManager::load(const QUrl& url)
{
    Q_D(DocumentSyncManager);

    d->load(url);
}

bool DocumentSyncManager::setSynchronizer(AbstractDocument* document)
{
    Q_D(DocumentSyncManager);

    return d->setSynchronizer(document);
}

bool DocumentSyncManager::canClose(AbstractDocument* document)
{
    Q_D(DocumentSyncManager);

    return d->canClose(document);
}

void DocumentSyncManager::reload(AbstractDocument* document)
{
    Q_D(DocumentSyncManager);

    d->reload(document);
}

void DocumentSyncManager::save(AbstractDocument* document)
{
    Q_D(DocumentSyncManager);

    d->save(document);
}

}
