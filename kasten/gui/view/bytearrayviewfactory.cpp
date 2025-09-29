/*
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
// Std
#include <utility>

namespace Kasten {

ByteArrayViewFactory::ByteArrayViewFactory(ByteArrayViewProfileManager* byteArrayViewProfileManager)
    : mByteArrayViewProfileManager(byteArrayViewProfileManager)
{
}

std::unique_ptr<AbstractView> ByteArrayViewFactory::createViewFor(AbstractDocument* _document)
{
    std::unique_ptr<ByteArrayView> result;

    auto* document = static_cast<ByteArrayDocument*>(_document);
    if (document) {
        auto synchronizer = std::make_unique<ByteArrayViewProfileSynchronizer>(mByteArrayViewProfileManager);

        synchronizer->setViewProfileId(mByteArrayViewProfileManager->defaultViewProfileId());

        result = std::make_unique<ByteArrayView>(document, std::move(synchronizer));
    }

    return result;
}

std::unique_ptr<AbstractView> ByteArrayViewFactory::createCopyOfView(AbstractView* _view, Qt::Alignment alignment)
{
    std::unique_ptr<ByteArrayView> result;

    auto* view = qobject_cast<ByteArrayView*>(_view);
    if (view) {
        auto synchronizer = std::make_unique<ByteArrayViewProfileSynchronizer>(mByteArrayViewProfileManager);

        synchronizer->setViewProfileId(view->synchronizer()->viewProfileId());

        result = std::make_unique<ByteArrayView>(view, std::move(synchronizer), alignment);
    }

    return result;
}

}
