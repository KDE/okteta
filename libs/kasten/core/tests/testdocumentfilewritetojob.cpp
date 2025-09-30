/*
    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "testdocumentfilewritetojob.hpp"

// lib
#include "testdocumentfilesynchronizer.hpp"
#include "testdocumentfilewritethread.hpp"
#include "testdocument.hpp"
// Qt
#include <QCoreApplication>

namespace Kasten {

TestDocumentFileWriteToJob::TestDocumentFileWriteToJob(TestDocumentFileSynchronizer* synchronizer,
                                                       const QUrl& url, AbstractModelSynchronizer::ConnectOption option)
    : AbstractFileSystemSyncWithRemoteJob(synchronizer, url, option)
{}

TestDocumentFileWriteToJob::~TestDocumentFileWriteToJob() = default;

void TestDocumentFileWriteToJob::startSyncWithRemote()
{
    auto* const testSynchronizer = qobject_cast<TestDocumentFileSynchronizer*>(synchronizer());
    auto* const document = qobject_cast<TestDocument*>(synchronizer()->document());
    auto writeThread = std::make_unique<TestDocumentFileWriteThread>(this, testSynchronizer->header(), document, file());
    writeThread->start();
    while (!writeThread->wait(100)) {
        QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents | QEventLoop::ExcludeSocketNotifiers);
    }

    const bool success = writeThread->success();
    writeThread.reset();

    completeSync(success);
}

}

#include "moc_testdocumentfilewritetojob.cpp"
