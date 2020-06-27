/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BYTEARRAYRAWFILELOADTHREAD_HPP
#define KASTEN_BYTEARRAYRAWFILELOADTHREAD_HPP

// Qt
#include <QThread>

class QFile;

namespace Kasten {

class ByteArrayDocument;

class ByteArrayRawFileLoadThread : public QThread
{
    Q_OBJECT

public:
    ByteArrayRawFileLoadThread(QObject* parent,
                               QFile* file);
    ~ByteArrayRawFileLoadThread() override;

public: // QThread API
    void run() override;

public:
    ByteArrayDocument* document() const;

    const QString& errorString() const;

Q_SIGNALS:
    void documentRead(Kasten::ByteArrayDocument* document);

private:
    QFile* mFile;

    ByteArrayDocument* mDocument = nullptr;
    QString mErrorString;
};

inline ByteArrayRawFileLoadThread::ByteArrayRawFileLoadThread(QObject* parent, QFile* file)
    : QThread(parent)
    , mFile(file)
{}

inline ByteArrayDocument* ByteArrayRawFileLoadThread::document() const { return mDocument; }
inline const QString& ByteArrayRawFileLoadThread::errorString()  const { return mErrorString; }

}

#endif
