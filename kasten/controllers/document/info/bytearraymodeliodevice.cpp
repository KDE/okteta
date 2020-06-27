/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2010 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearraymodeliodevice.hpp"

// Okteta core
#include <Okteta/AbstractByteArrayModel>

namespace Okteta {

ByteArrayModelIoDevice::ByteArrayModelIoDevice(AbstractByteArrayModel* byteArrayModel, QObject* parent)
    : QIODevice(parent)
    , mByteArrayModel(byteArrayModel)
{
    open(ReadOnly);   // krazy:exclude=syscalls
}

ByteArrayModelIoDevice::~ByteArrayModelIoDevice() = default;

qint64 ByteArrayModelIoDevice::size() const
{
    return mByteArrayModel->size();
}

bool ByteArrayModelIoDevice::canReadLine() const
{
    return
        isOpen()
        && (mByteArrayModel->indexOf("\n", 1, pos()) != -1
            || QIODevice::canReadLine());
}

bool ByteArrayModelIoDevice::open(OpenMode openMode)
{
    QIODevice::open(openMode);

    openMode ^= WriteOnly | Append;
    setOpenMode(openMode);

    if (!isReadable()) {
        return false;
    }

    seek(0);

    return true;
}

bool ByteArrayModelIoDevice::seek(qint64 pos)
{
    if (pos > mByteArrayModel->size() || pos < 0) {
        return false;
    }

    mReadOffset = pos;

    return QIODevice::seek(pos);
}

qint64 ByteArrayModelIoDevice::readData(char* data, qint64 maxlength)
{
    const Size copiedLength =
        mByteArrayModel->copyTo(reinterpret_cast<Byte*>(data), mReadOffset, maxlength);

    mReadOffset += copiedLength;

    return copiedLength;
}

qint64 ByteArrayModelIoDevice::writeData(const char* data, qint64 length)
{
    Q_UNUSED(data);
    Q_UNUSED(length);
    return -1;
}

}
