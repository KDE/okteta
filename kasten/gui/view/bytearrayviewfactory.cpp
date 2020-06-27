/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2006, 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearrayviewfactory.hpp"

// lib
#include "bytearrayview.hpp"
#include <bytearrayviewprofilemanager.hpp>
#include <bytearrayviewprofilesynchronizer.hpp>
// Okteta Kasten core
#include <Kasten/Okteta/ByteArrayDocument>

namespace Kasten {

ByteArrayViewFactory::ByteArrayViewFactory(ByteArrayViewProfileManager* byteArrayViewProfileManager)
    : mByteArrayViewProfileManager(byteArrayViewProfileManager)
{
}

AbstractView* ByteArrayViewFactory::createViewFor(AbstractDocument* _document)
{
    ByteArrayView* result = nullptr;

    auto* document = static_cast<ByteArrayDocument*>(_document);
    if (document) {
        auto* synchronizer = new ByteArrayViewProfileSynchronizer(mByteArrayViewProfileManager);

        synchronizer->setViewProfileId(mByteArrayViewProfileManager->defaultViewProfileId());

        result = new ByteArrayView(document, synchronizer);
    }

    return result;
}

AbstractView* ByteArrayViewFactory::createCopyOfView(AbstractView* _view, Qt::Alignment alignment)
{
    ByteArrayView* result = nullptr;

    auto* view = qobject_cast<ByteArrayView*>(_view);
    if (view) {
        auto* synchronizer = new ByteArrayViewProfileSynchronizer(mByteArrayViewProfileManager);

        synchronizer->setViewProfileId(view->synchronizer()->viewProfileId());

        result = new ByteArrayView(view, synchronizer, alignment);
    }

    return result;
}

}
