/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "testdocumentfilereloadjob.hpp"

// lib
#include "testdocumentfilesynchronizer.hpp"
#include "testdocumentfilereloadthread.hpp"
#include "testdocument.hpp"
// Qt
#include <QCoreApplication>

namespace Kasten {

TestDocumentFileReloadJob::TestDocumentFileReloadJob(TestDocumentFileSynchronizer* synchronizer)
    : AbstractFileSystemSyncFromRemoteJob(synchronizer)
{}

TestDocumentFileReloadJob::~TestDocumentFileReloadJob() = default;

void TestDocumentFileReloadJob::startReadFromFile()
{
    auto* testSynchronizer = qobject_cast<TestDocumentFileSynchronizer*>(synchronizer());
    auto* document = qobject_cast<TestDocument*>(synchronizer()->document());
    auto* reloadThread =
        new TestDocumentFileReloadThread(this, testSynchronizer->header(), /*document, */ file());
    reloadThread->start();
    while (!reloadThread->wait(100)) {
        QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents | QEventLoop::ExcludeSocketNotifiers);
    }

    const bool success = reloadThread->success();
    // TODO: moved this here to avoid marshalling the change signals out of the thread. Good idea?
    if (success) {
        document->setData(reloadThread->byteArray());
    }
    delete reloadThread;

    completeRead(success);
}

}
