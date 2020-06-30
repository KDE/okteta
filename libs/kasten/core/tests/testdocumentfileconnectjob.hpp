/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_TESTDOCUMENTFILECONNECTJOB_HPP
#define KASTEN_TESTDOCUMENTFILECONNECTJOB_HPP

// Kasten core
#include <abstractfilesystemconnectjob.hpp>

namespace Kasten {

class TestDocumentFileSynchronizer;
class TestDocument;
// class TestDocumentFileWriteThread;

class TestDocumentFileConnectJob : public AbstractFileSystemConnectJob
{
    Q_OBJECT

public:
    TestDocumentFileConnectJob(TestDocumentFileSynchronizer* synchronizer, AbstractDocument* document,
                               const QUrl& url, AbstractModelSynchronizer::ConnectOption option);
    ~TestDocumentFileConnectJob() override;

protected: // AbstractFileSystemLoadJob API
    void startConnectWithFile() override;

private Q_SLOTS:
//     void onDataPulled();

private:
//     TestDocumentFileWriteThread* mWriteThread;
};

}

#endif
