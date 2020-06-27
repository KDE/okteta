/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2007-2008, 2012 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_TESTDOCUMENTFILESYNCHRONIZER_HPP
#define KASTEN_TESTDOCUMENTFILESYNCHRONIZER_HPP

// Kasten
#include <abstractmodelfilesystemsynchronizer.hpp>
// Qt
#include <QByteArray>

namespace Kasten {

class TestDocument;

class TestDocumentFileSynchronizer : public AbstractModelFileSystemSynchronizer
{
    Q_OBJECT

    friend class TestDocumentFileLoadJob;
    friend class TestDocumentFileConnectJob;

public:
    explicit TestDocumentFileSynchronizer(const QByteArray& header = QByteArray());

public: // AbstractModelSynchronizer API
    AbstractLoadJob* startLoad(const QUrl& url) override;
    AbstractSyncToRemoteJob* startSyncToRemote() override;
    AbstractSyncFromRemoteJob* startSyncFromRemote() override;
    AbstractSyncWithRemoteJob* startSyncWithRemote(const QUrl& url, AbstractModelSynchronizer::ConnectOption option) override;
    AbstractConnectJob* startConnect(AbstractDocument* document,
                                     const QUrl& url, AbstractModelSynchronizer::ConnectOption option) override;

    AbstractDocument* document() const override;

    LocalSyncState localSyncState() const override;

public:
    const QByteArray& header() const;

protected:
    void setDocument(TestDocument* document);

private:
    TestDocument* mDocument = nullptr;
    const QByteArray mHeader;
};

inline const QByteArray& TestDocumentFileSynchronizer::header() const { return mHeader; }
inline void TestDocumentFileSynchronizer::setDocument(TestDocument* document) { mDocument = document; }

}

#endif
