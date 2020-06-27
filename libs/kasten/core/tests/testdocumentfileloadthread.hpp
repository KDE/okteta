/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_TESTDOCUMENTFILELOADTHREAD_HPP
#define KASTEN_TESTDOCUMENTFILELOADTHREAD_HPP

// Qt
#include <QThread>

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
    TestDocument* document() const;

Q_SIGNALS:
    void documentRead(Kasten::TestDocument* document);

private:
    QFile* mFile;
    const QByteArray mHeader;

    TestDocument* mDocument = nullptr;
};

inline TestDocumentFileLoadThread::TestDocumentFileLoadThread(QObject* parent, const QByteArray& header, QFile* file)
    : QThread(parent)
    , mFile(file)
    , mHeader(header)
{}

inline TestDocument* TestDocumentFileLoadThread::document() const { return mDocument; }

}

#endif
