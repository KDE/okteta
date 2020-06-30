/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_TESTDOCUMENTFILERELOADTHREAD_HPP
#define KASTEN_TESTDOCUMENTFILERELOADTHREAD_HPP

// Qt
#include <QByteArray>
#include <QThread>

class QFile;

namespace Kasten {

class TestDocumentFileReloadThread : public QThread
{
    Q_OBJECT

public:
    TestDocumentFileReloadThread(QObject* parent, const QByteArray& header,
                                 /*TestDocument* document, */ QFile* file);
    ~TestDocumentFileReloadThread() override;

public: // QThread API
    void run() override;

public:
    bool success() const;
    QByteArray byteArray() const;

Q_SIGNALS:
    void documentReloaded(bool success);

private:
//     TestDocument* mDocument;
    const QByteArray mHeader;
    QFile* mFile;

    bool mSuccess = false;
    QByteArray mByteArray;
};

inline bool TestDocumentFileReloadThread::success()         const { return mSuccess; }
inline QByteArray TestDocumentFileReloadThread::byteArray() const { return mByteArray; }

}

#endif
