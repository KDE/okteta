/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2008-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearrayrawfileloadjob.hpp"

// lib
// #include "externalbookmarkstorage.hpp"
#include "bytearrayrawfilesynchronizer.hpp"
#include "bytearrayrawfileloadthread.hpp"
#include "bytearraydocument.hpp"
// Qt
#include <QCoreApplication>
#include <QUrl>

namespace Kasten {

ByteArrayRawFileLoadJob::ByteArrayRawFileLoadJob(std::unique_ptr<ByteArrayRawFileSynchronizer>&& synchronizer, const QUrl& url)
    : AbstractFileSystemLoadJob(std::move(synchronizer), url)
{}

ByteArrayRawFileLoadJob::~ByteArrayRawFileLoadJob() = default;

void ByteArrayRawFileLoadJob::startLoadFromFile()
{
    auto loadThread = std::make_unique<ByteArrayRawFileLoadThread>(this, file());
    loadThread->start();
    while (!loadThread->wait(100)) {
        QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents | QEventLoop::ExcludeSocketNotifiers);
    }

    auto document = loadThread->releaseDocument();
    qobject_cast<ByteArrayRawFileSynchronizer*>(synchronizer())->setDocument(document.get());

    if (document) {
//         ExternalBookmarkStorage().readBookmarks( document, url() );
    } else {
        // TODO: these reports should go to a notification system, for log or popup
        setError(KJob::KilledJobError);
        setErrorText(loadThread->errorString());
    }

    loadThread.reset();

    setDocument(std::move(document));
}

}

#include "moc_bytearrayrawfileloadjob.cpp"
