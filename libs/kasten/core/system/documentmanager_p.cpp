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

#include "documentmanager_p.hpp"

// lib
#include <abstractdocument.hpp>
// Qt
#include <QUrl>
#include <QMutableVectorIterator>
#include <QStringList>

// temporary
#include "documentcreatemanager.hpp"
#include "documentsyncmanager.hpp"
#include "modelcodecmanager.hpp"

namespace Kasten {

static int lastDocumentId = 0;

DocumentManagerPrivate::DocumentManagerPrivate(DocumentManager* q)
    : q_ptr(q)
    , mCreateManager(new DocumentCreateManager(q))
    , mSyncManager(new DocumentSyncManager(q))
    , mCodecManager(new ModelCodecManager())
{
}

DocumentManagerPrivate::~DocumentManagerPrivate()
{
    // TODO: emit signal here, too?
    qDeleteAll(mList);

    delete mCreateManager;
    delete mSyncManager;
    delete mCodecManager;
} // TODO: destroy all documents?

void DocumentManagerPrivate::addDocument(AbstractDocument* document)
{
    Q_Q(DocumentManager);

    // TODO: check for double insert
    document->setId(QString::number(++lastDocumentId));
    mList.append(document);
    // TODO: only emit if document was not included before
    const QVector<AbstractDocument*> addedDocuments { document };
    Q_EMIT q->added(addedDocuments);
}

void DocumentManagerPrivate::closeDocument(AbstractDocument* document)
{
    Q_Q(DocumentManager);

    QMutableVectorIterator<AbstractDocument*> iterator(mList);

    if (iterator.findNext(document)) {
        // TODO: first check if unsaved and ask, only then close

        iterator.remove();

        const QVector<AbstractDocument*> closedDocuments { document };
        Q_EMIT q->closing(closedDocuments);

        delete document;
    }
}

void DocumentManagerPrivate::closeDocuments(const QVector<AbstractDocument*>& documents)
{
    Q_Q(DocumentManager);

    // TODO: optimize
    for (AbstractDocument* document : documents) {
        mList.removeOne(document);
    }

    Q_EMIT q->closing(documents);

    for (AbstractDocument* document : documents) {
        delete document;
    }
}

void DocumentManagerPrivate::closeAll()
{
    Q_Q(DocumentManager);

    // TODO: is it better for remove the document from the list before emitting closing(document)?
    // TODO: or better emit close(documentList)? who would use this?
    const QVector<AbstractDocument*> closedDocuments = mList;
    mList.clear();

    Q_EMIT q->closing(closedDocuments);

    for (AbstractDocument* document : closedDocuments) {
        delete document;
    }
}

void DocumentManagerPrivate::closeAllOther(AbstractDocument* keptDocument)
{
    Q_Q(DocumentManager);

    // TODO: is it better for remove the document from the list before emitting closing(document)?
    // TODO: or better emit close(documentList)? who would use this?
    QVector<AbstractDocument*> closedDocuments = mList;
    closedDocuments.removeOne(keptDocument);

    mList.clear();
    mList.append(keptDocument);

    Q_EMIT q->closing(closedDocuments);

    for (AbstractDocument* document : qAsConst(closedDocuments)) {
        delete document;
    }
}

bool DocumentManagerPrivate::canClose(AbstractDocument* document) const
{
    return mSyncManager->canClose(document);
}

bool DocumentManagerPrivate::canClose(const QVector<AbstractDocument*>& documents) const
{
    bool canClose = true;

    for (AbstractDocument* document : documents) {
        if (!mSyncManager->canClose(document)) {
            canClose = false;
            break;
        }
    }

    return canClose;
}

bool DocumentManagerPrivate::canCloseAll() const
{
    bool canCloseAll = true;

    for (AbstractDocument* document : qAsConst(mList)) {
        if (!mSyncManager->canClose(document)) {
            canCloseAll = false;
            break;
        }
    }

    return canCloseAll;
}

bool DocumentManagerPrivate::canCloseAllOther(AbstractDocument* keptDocument) const
{
    bool canCloseAll = true;

    for (AbstractDocument* document : qAsConst(mList)) {
        if ((document != keptDocument) &&
            !mSyncManager->canClose(document)) {
            canCloseAll = false;
            break;
        }
    }

    return canCloseAll;
}

void DocumentManagerPrivate::requestFocus(AbstractDocument* document)
{
    Q_Q(DocumentManager);

    Q_EMIT q->focusRequested(document);
}

}
