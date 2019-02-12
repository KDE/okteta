/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library. If not, see <http://www.gnu.org/licenses/>.
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
