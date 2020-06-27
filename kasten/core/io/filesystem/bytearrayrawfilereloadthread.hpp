/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BYTEARRAYRAWFILERELOADTHREAD_HPP
#define KASTEN_BYTEARRAYRAWFILERELOADTHREAD_HPP

// Qt
#include <QByteArray>
#include <QThread>

class QFile;

namespace Kasten {

class ByteArrayRawFileReloadThread : public QThread
{
    Q_OBJECT

public:
    ByteArrayRawFileReloadThread(QObject* parent, QFile* file);

    ~ByteArrayRawFileReloadThread() override;

public: // QThread API
    void run() override;

public:
    bool success() const;
    const QString& errorString() const;

    const QByteArray& data() const;

Q_SIGNALS:
    void documentReloaded(bool success);

private:
    QFile* mFile;

    bool mSuccess = false;
    QString mErrorString;

    QByteArray mData;
};

inline bool ByteArrayRawFileReloadThread::success()                const { return mSuccess; }
inline const QString& ByteArrayRawFileReloadThread::errorString()  const { return mErrorString; }
inline const QByteArray& ByteArrayRawFileReloadThread::data()      const { return mData; }

}

#endif
