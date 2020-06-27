/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2006-2007, 2009, 2011, 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "documentcreatemanager_p.hpp"

// lib
#include "abstractdocumentfactory.hpp"
#include "documentmanager.hpp"
#include <abstractdocument.hpp>

namespace Kasten {

DocumentCreateManagerPrivate::DocumentCreateManagerPrivate(DocumentManager* manager)
    : mManager(manager)
{
}

DocumentCreateManagerPrivate::~DocumentCreateManagerPrivate()
{
    delete mFactory;
}

bool DocumentCreateManagerPrivate::canCreateNewFromData(const QMimeData* mimeData) const
{
    return mFactory->canCreateFromData(mimeData);
}

void DocumentCreateManagerPrivate::setDocumentFactory(AbstractDocumentFactory* factory)
{
    mFactory = factory;
}

void DocumentCreateManagerPrivate::createNew() const
{
    AbstractDocument* document = mFactory->create();
    if (document) {
        mManager->addDocument(document);
    }
}

void DocumentCreateManagerPrivate::createNewFromData(const QMimeData* mimeData, bool setModified) const
{
    AbstractDocument* document = mFactory->createFromData(mimeData, setModified);
    if (document) {
        mManager->addDocument(document);
    }
}

}
