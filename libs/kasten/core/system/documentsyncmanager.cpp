/*
    This file is part of the Kasten Framework, made within the KDE community.

    Copyright 2007-2009,2011,2019 Friedrich W. H. Kossebau <kossebau@kde.org>

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
