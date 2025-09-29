/*
    SPDX-FileCopyrightText: 2006-2007, 2009, 2011, 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "documentcreatemanager_p.hpp"

// lib
#include "abstractdocumentfactory.hpp"
#include "documentmanager.hpp"
#include <abstractdocument.hpp>
// Std
#include <utility>

namespace Kasten {

DocumentCreateManagerPrivate::DocumentCreateManagerPrivate(DocumentManager* manager)
    : mManager(manager)
{
}

DocumentCreateManagerPrivate::~DocumentCreateManagerPrivate() = default;

bool DocumentCreateManagerPrivate::canCreateNewFromData(const QMimeData* mimeData) const
{
    return mFactory->canCreateFromData(mimeData);
}

void DocumentCreateManagerPrivate::setDocumentFactory(std::unique_ptr<AbstractDocumentFactory>&& factory)
{
    mFactory = std::move(factory);
}

void DocumentCreateManagerPrivate::createNew() const
{
    auto document = mFactory->create();
    if (document) {
        mManager->addDocument(std::move(document));
    }
}

void DocumentCreateManagerPrivate::createNewFromData(const QMimeData* mimeData, bool setModified) const
{
    auto document = mFactory->createFromData(mimeData, setModified);
    if (document) {
        mManager->addDocument(std::move(document));
    }
}

}
