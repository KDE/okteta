/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_TESTDOCUMENTFILEWRITETHREAD_HPP
#define KASTEN_TESTDOCUMENTFILEWRITETHREAD_HPP

// Qt
#include <QByteArray>
#include <QThread>

class QFile;

namespace Kasten {

class TestDocument;

class TestDocumentFileWriteThread : public QThread
{
    Q_OBJECT

public:
    TestDocumentFileWriteThread(QObject* parent, const QByteArray& header,
                                TestDocument* document, QFile* file);
    ~TestDocumentFileWriteThread() override;

public: // QThread API
    void run() override;

public:
    bool success() const;

Q_SIGNALS:
    void documentWritten(bool success);

private:
    const QByteArray mHeader;
    const TestDocument* mDocument;
    QFile* mFile;

    bool mSuccess = false;
};

inline TestDocumentFileWriteThread::TestDocumentFileWriteThread(QObject* parent, const QByteArray& header,
                                                                TestDocument* document, QFile* file)
    : QThread(parent)
    , mHeader(header)
    , mDocument(document)
    , mFile(file)
{}

inline bool TestDocumentFileWriteThread::success() const { return mSuccess; }

}

#endif
