/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_TESTDOCUMENTFILELOADJOB_HPP
#define KASTEN_TESTDOCUMENTFILELOADJOB_HPP

// Kasten core
#include <abstractfilesystemloadjob.hpp>

namespace Kasten {

class TestDocumentFileSynchronizer;
class TestDocumentFileLoadThread;

class TestDocumentFileLoadJob : public AbstractFileSystemLoadJob
{
    Q_OBJECT

public:
    TestDocumentFileLoadJob(TestDocumentFileSynchronizer* synchronizer, const QUrl& url);
    ~TestDocumentFileLoadJob() override;

protected: // AbstractFileSystemLoadJob API
    void startLoadFromFile() override;

private Q_SLOTS:
//     void onDataPulled();
};

}

#endif
