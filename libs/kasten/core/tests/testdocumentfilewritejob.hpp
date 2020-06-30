/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_TESTDOCUMENTFILEWRITEJOB_HPP
#define KASTEN_TESTDOCUMENTFILEWRITEJOB_HPP

// Kasten core
#include <abstractfilesystemsynctoremotejob.hpp>

namespace Kasten {

class TestDocumentFileSynchronizer;
class TestDocumentFileWriteThread;

class TestDocumentFileWriteJob : public AbstractFileSystemSyncToRemoteJob
{
    Q_OBJECT

public:
    TestDocumentFileWriteJob(TestDocumentFileSynchronizer* synchronizer);
    ~TestDocumentFileWriteJob() override;

protected: // AbstractFileSystemSyncToRemoteJob API
    void startWriteToFile() override;

private Q_SLOTS:
//     void onDataPulled();
};

}

#endif
