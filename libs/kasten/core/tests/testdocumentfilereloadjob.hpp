/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_TESTDOCUMENTFILERELOADJOB_HPP
#define KASTEN_TESTDOCUMENTFILERELOADJOB_HPP

// Kasten core
#include <abstractfilesystemsyncfromremotejob.hpp>

namespace Kasten {

class TestDocumentFileSynchronizer;
class TestDocumentFileReloadThread;

class TestDocumentFileReloadJob : public AbstractFileSystemSyncFromRemoteJob
{
    Q_OBJECT

public:
    explicit TestDocumentFileReloadJob(TestDocumentFileSynchronizer* synchronizer);
    ~TestDocumentFileReloadJob() override;

protected: // AbstractFileSystemSyncFromRemoteJob API
    void startReadFromFile() override;

private Q_SLOTS:
//     void onDataPulled();
};

}

#endif
