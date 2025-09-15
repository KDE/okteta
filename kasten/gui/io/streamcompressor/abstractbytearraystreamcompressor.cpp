/*
    SPDX-FileCopyrightText: 2025 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "abstractbytearraystreamcompressor.hpp"

// lib
#include "iodevicebytearrayoutput.hpp"
#include "abstractcompressionstream.hpp"
#include <logging.hpp>
// Okteta core
#include <Okteta/AbstractByteArrayModel>
// KF
#include <KLocalizedString>
// Qt
#include <QIODevice>

namespace Kasten {

AbstractByteArrayStreamCompressor::AbstractByteArrayStreamCompressor(const QString& remoteTypeName,
                                                                     const QString& remoteMimeType,
                                                                     const QString& remoteClipboardMimeType)
    : AbstractByteArrayStreamEncoder(remoteTypeName, remoteMimeType, remoteClipboardMimeType)
{
}

AbstractByteArrayStreamCompressor::~AbstractByteArrayStreamCompressor() = default;

bool AbstractByteArrayStreamCompressor::encodeDataToStream(QIODevice* ioDevice,
                                                           const ByteArrayView* byteArrayView,
                                                           const Okteta::AbstractByteArrayModel* byteArrayModel,
                                                           const Okteta::AddressRange& range)
{
    Q_UNUSED(byteArrayView)

    const bool isDeviceOpenessManaged = !ioDevice->isOpen();
    if (isDeviceOpenessManaged) {
        if (!ioDevice->open(QIODevice::WriteOnly)) {
            // TODO: add error reporting to AbstractModelStreamEncoder
            // setError(i18n("Error opening device: %1", ioDevice->errorString()));
            return false;
        }
    } else {
        const QIODevice::OpenMode deviceMode = ioDevice->openMode();
        if (!(deviceMode & QIODevice::WriteOnly)) {
            qCWarning(LOG_KASTEN_OKTETA_GUI) << "AbstractCompressionStream: device must be open in WriteOnly modes";
            return false;
        }
    }

    bool success = true;

    // encode
    IoDeviceByteArrayOutput byteArrayOutput(ioDevice);
    // TODO: see to have stream object mem-alloced with subclass instead
    auto compressionStream = createCompressionStream(&byteArrayOutput);

    const int inputBufferSize = compressionStream->inputBufferSizeHint();
    std::vector<Okteta::Byte> inputBuffer(inputBufferSize);

    success = compressionStream->writeHeader();

    if (success) {
        Okteta::AddressRange inputRange = Okteta::AddressRange::fromWidth(range.start(), inputBuffer.size());
        inputRange.restrictEndTo(range.end());

        while (inputRange.isValid()) {
            const Okteta::Size copiedSize = byteArrayModel->copyTo(inputBuffer.data(), inputRange);
            if (copiedSize != inputRange.width()) {
                success = false;
                break;
            }

            const int writeResult = compressionStream->writeData(reinterpret_cast<char*>(inputBuffer.data()), inputRange.width());
            if (writeResult == -1) {
                success = false;
                break;
            }

            inputRange.moveToStart(inputRange.nextBehindEnd());
            inputRange.restrictEndTo(range.end());
        }
    }

    if (success) {
        success = compressionStream->finish();
    }

    if (isDeviceOpenessManaged) {
        ioDevice->close();
    }

    return success;
}

}

#include "moc_abstractbytearraystreamcompressor.cpp"
