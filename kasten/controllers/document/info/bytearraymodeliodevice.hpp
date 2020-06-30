/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2010 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BYTEARRAYMODELIODEVICE_HPP
#define KASTEN_BYTEARRAYMODELIODEVICE_HPP

// Okteta core
#include <Okteta/Address>
// Qt
#include <QIODevice>

namespace Okteta {

class AbstractByteArrayModel;

class ByteArrayModelIoDevice : public QIODevice
{
    Q_OBJECT

public:
    explicit ByteArrayModelIoDevice(AbstractByteArrayModel* byteArrayModel, QObject* parent = nullptr);
    ByteArrayModelIoDevice() = delete;

    ~ByteArrayModelIoDevice() override;

public: // QIODevice API
    qint64 size() const override;
    bool canReadLine() const override;

    bool open(OpenMode openMode) override;
    bool seek(qint64 pos) override;

protected:
    qint64 readData(char* data, qint64 maxlength) override;
    qint64 writeData(const char* data, qint64 length) override;

private:
    AbstractByteArrayModel* mByteArrayModel;
    Address mReadOffset = 0;
};

}

#endif
