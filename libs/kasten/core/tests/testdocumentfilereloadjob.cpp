/*
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
    auto* const testSynchronizer = qobject_cast<TestDocumentFileSynchronizer*>(synchronizer());
    auto* const document = qobject_cast<TestDocument*>(synchronizer()->document());
    auto reloadThread = std::make_unique<TestDocumentFileReloadThread>(this, testSynchronizer->header(), /*document, */ file());
    reloadThread->start();
    while (!reloadThread->wait(100)) {
        QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents | QEventLoop::ExcludeSocketNotifiers);
    }

    const bool success = reloadThread->success();
    // TODO: moved this here to avoid marshalling the change signals out of the thread. Good idea?
    if (success) {
        document->setData(reloadThread->byteArray());
    }
    reloadThread.reset();

    completeRead(success);
}

}

#include "moc_testdocumentfilereloadjob.cpp"
