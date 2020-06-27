/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "testdocumentfileloadjob.hpp"

// lib
#include "testdocumentfilesynchronizer.hpp"
#include "testdocumentfileloadthread.hpp"
#include "testdocument.hpp"
// Qt
#include <QCoreApplication>

namespace Kasten {

TestDocumentFileLoadJob::TestDocumentFileLoadJob(TestDocumentFileSynchronizer* synchronizer, const QUrl& url)
    : AbstractFileSystemLoadJob(synchronizer, url)
{}

TestDocumentFileLoadJob::~TestDocumentFileLoadJob() = default;

void TestDocumentFileLoadJob::startLoadFromFile()
{
    auto* testSynchronizer = qobject_cast<TestDocumentFileSynchronizer*>(synchronizer());

    auto* loadThread = new TestDocumentFileLoadThread(this, testSynchronizer->header(), file());
    loadThread->start();
    while (!loadThread->wait(100)) {
        QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents | QEventLoop::ExcludeSocketNotifiers);
    }

    TestDocument* document = loadThread->document();
    testSynchronizer->setDocument(document);

    delete loadThread;

    setDocument(document);
}

}
