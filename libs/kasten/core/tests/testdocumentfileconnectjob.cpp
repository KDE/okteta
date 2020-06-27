/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "testdocumentfileconnectjob.hpp"

// lib
#include "testdocumentfilesynchronizer.hpp"
#include "testdocumentfilewritethread.hpp"
#include "testdocument.hpp"
// Qt
#include <QCoreApplication>

namespace Kasten {

TestDocumentFileConnectJob::TestDocumentFileConnectJob(TestDocumentFileSynchronizer* synchronizer,
                                                       AbstractDocument* document,
                                                       const QUrl& url,
                                                       AbstractModelSynchronizer::ConnectOption option)
    : AbstractFileSystemConnectJob(synchronizer, document, url, option)
{
}

TestDocumentFileConnectJob::~TestDocumentFileConnectJob() = default;

void TestDocumentFileConnectJob::startConnectWithFile()
{
    auto* testSynchronizer = qobject_cast<TestDocumentFileSynchronizer*>(synchronizer());
    auto* testDocument = qobject_cast<TestDocument*>(document());
    auto* writeThread =
        new TestDocumentFileWriteThread(this, testSynchronizer->header(), testDocument, file());

    writeThread->start();
    while (!writeThread->wait(100)) {
        QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents | QEventLoop::ExcludeSocketNotifiers);
    }

    const bool success = writeThread->success();

    if (success) {
        testSynchronizer->setDocument(testDocument);
    }
    delete writeThread;

    complete(success);
}

}
