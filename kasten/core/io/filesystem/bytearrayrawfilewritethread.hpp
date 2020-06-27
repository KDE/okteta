/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BYTEARRAYRAWFILEWRITETHREAD_HPP
#define KASTEN_BYTEARRAYRAWFILEWRITETHREAD_HPP

// Qt
#include <QThread>

class QFile;

namespace Kasten {

class ByteArrayDocument;

class ByteArrayRawFileWriteThread : public QThread
{
    Q_OBJECT

public:
    ByteArrayRawFileWriteThread(QObject* parent,
                                ByteArrayDocument* document, QFile* file);

    ~ByteArrayRawFileWriteThread() override;

public: // QThread API
    void run() override;

public:
    bool success() const;

Q_SIGNALS:
    void documentWritten(bool success);

private:
    ByteArrayDocument* mDocument;
    QFile* mFile;

    bool mSuccess = false;
};

inline ByteArrayRawFileWriteThread::ByteArrayRawFileWriteThread(QObject* parent,
                                                                ByteArrayDocument* document, QFile* file)
    : QThread(parent)
    , mDocument(document)
    , mFile(file)
{}

inline bool ByteArrayRawFileWriteThread::success() const { return mSuccess; }

}

#endif
