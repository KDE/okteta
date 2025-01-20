/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BYTEARRAYRAWFILELOADTHREAD_HPP
#define KASTEN_BYTEARRAYRAWFILELOADTHREAD_HPP

// lib
#include "bytearraydocument.hpp"
// Qt
#include <QThread>
// Std
#include <memory>

class QFile;

namespace Kasten {

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
    std::unique_ptr<ByteArrayDocument> releaseDocument();

    [[nodiscard]]
    const QString& errorString() const;

private:
    QFile* const mFile;

    std::unique_ptr<ByteArrayDocument> mDocument;
    QString mErrorString;
};

inline ByteArrayRawFileLoadThread::ByteArrayRawFileLoadThread(QObject* parent, QFile* file)
    : QThread(parent)
    , mFile(file)
{}

inline std::unique_ptr<ByteArrayDocument> ByteArrayRawFileLoadThread::releaseDocument() { return std::move(mDocument); }
inline const QString& ByteArrayRawFileLoadThread::errorString()  const { return mErrorString; }

}

#endif
