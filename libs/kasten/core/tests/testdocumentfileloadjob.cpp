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

TestDocumentFileLoadJob::TestDocumentFileLoadJob(std::unique_ptr<TestDocumentFileSynchronizer>&& synchronizer, const QUrl& url)
    : AbstractFileSystemLoadJob(std::move(synchronizer), url)
{}

TestDocumentFileLoadJob::~TestDocumentFileLoadJob() = default;

void TestDocumentFileLoadJob::startLoadFromFile()
{
    auto* testSynchronizer = qobject_cast<TestDocumentFileSynchronizer*>(synchronizer());

    auto loadThread = std::make_unique<TestDocumentFileLoadThread>(this, testSynchronizer->header(), file());
    loadThread->start();
    while (!loadThread->wait(100)) {
        QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents | QEventLoop::ExcludeSocketNotifiers);
    }

    auto document = loadThread->releaseDocument();
    testSynchronizer->setDocument(document.get());

    loadThread.reset();

    setDocument(std::move(document));
}

}

#include "moc_testdocumentfileloadjob.cpp"
