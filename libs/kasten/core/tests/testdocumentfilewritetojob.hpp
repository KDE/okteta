/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_TESTDOCUMENTFILEWRITETOJOB_HPP
#define KASTEN_TESTDOCUMENTFILEWRITETOJOB_HPP

// Kasten core
#include <abstractfilesystemsyncwithremotejob.hpp>

namespace Kasten {

class TestDocumentFileSynchronizer;
class TestDocumentFileWriteThread;

class TestDocumentFileWriteToJob : public AbstractFileSystemSyncWithRemoteJob
{
    Q_OBJECT

public:
    TestDocumentFileWriteToJob(TestDocumentFileSynchronizer* synchronizer,
                               const QUrl& url, AbstractModelSynchronizer::ConnectOption option);
    ~TestDocumentFileWriteToJob() override;

protected: // AbstractFileSystemSyncWithRemoteJob API
    void startSyncWithRemote() override;

private Q_SLOTS:
//     void onDataPulled();
};

}

#endif
