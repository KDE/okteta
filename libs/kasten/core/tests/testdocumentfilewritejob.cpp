/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "testdocumentfilewritejob.hpp"

// lib
#include "testdocumentfilesynchronizer.hpp"
#include "testdocumentfilewritethread.hpp"
#include "testdocument.hpp"
// Qt
#include <QCoreApplication>

namespace Kasten {

TestDocumentFileWriteJob::TestDocumentFileWriteJob(TestDocumentFileSynchronizer* synchronizer)
    : AbstractFileSystemSyncToRemoteJob(synchronizer)
{}

TestDocumentFileWriteJob::~TestDocumentFileWriteJob() = default;

void TestDocumentFileWriteJob::startWriteToFile()
{
    auto* testSynchronizer = qobject_cast<TestDocumentFileSynchronizer*>(synchronizer());
    auto* document = qobject_cast<TestDocument*>(synchronizer()->document());

    auto* writeThread = new TestDocumentFileWriteThread(this, testSynchronizer->header(),  document, file());
    writeThread->start();
    while (!writeThread->wait(100)) {
        QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents | QEventLoop::ExcludeSocketNotifiers);
    }

    const bool success = writeThread->success();
    delete writeThread;

    completeWrite(success);
}

}
