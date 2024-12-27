/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_TESTDOCUMENTFILELOADTHREAD_HPP
#define KASTEN_TESTDOCUMENTFILELOADTHREAD_HPP

// lib
#include "testdocument.hpp"
// Qt
#include <QByteArray>
#include <QThread>
// Std
#include <memory>

class QFile;

namespace Kasten {

class TestDocument;

class TestDocumentFileLoadThread : public QThread
{
    Q_OBJECT

public:
    TestDocumentFileLoadThread(QObject* parent, const QByteArray& header,
                               QFile* file);
    ~TestDocumentFileLoadThread() override;

public: // QThread API
    void run() override;

public:
    std::unique_ptr<TestDocument> releaseDocument();

Q_SIGNALS:
    void documentRead(Kasten::TestDocument* document);

private:
    QFile* mFile;
    const QByteArray mHeader;

    std::unique_ptr<TestDocument> mDocument = nullptr;
};

inline TestDocumentFileLoadThread::TestDocumentFileLoadThread(QObject* parent, const QByteArray& header, QFile* file)
    : QThread(parent)
    , mFile(file)
    , mHeader(header)
{}

inline std::unique_ptr<TestDocument> TestDocumentFileLoadThread::releaseDocument() { return std::move(mDocument); }

}

#endif
