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

ByteArrayRawFileLoadJob::ByteArrayRawFileLoadJob(ByteArrayRawFileSynchronizer* synchronizer, const QUrl& url)
    : AbstractFileSystemLoadJob(synchronizer, url)
{}

ByteArrayRawFileLoadJob::~ByteArrayRawFileLoadJob() = default;

void ByteArrayRawFileLoadJob::startLoadFromFile()
{
    auto* loadThread = new ByteArrayRawFileLoadThread(this, file());
    loadThread->start();
    while (!loadThread->wait(100)) {
        QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents | QEventLoop::ExcludeSocketNotifiers);
    }

    ByteArrayDocument* document = loadThread->document();
    qobject_cast<ByteArrayRawFileSynchronizer*>(synchronizer())->setDocument(document);

    if (document) {
//         ExternalBookmarkStorage().readBookmarks( document, url() );
    } else {
        // TODO: these reports should go to a notification system, for log or popup
        setError(KJob::KilledJobError);
        setErrorText(loadThread->errorString());
    }

    delete loadThread;

    setDocument(document);
}

}
