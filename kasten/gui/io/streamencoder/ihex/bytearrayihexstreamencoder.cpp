/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2010 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearrayihexstreamencoder.hpp"

// lib
#include <bytearrayview.hpp>
// Okteta gui
#include <Okteta/ByteArrayTableLayout>
// Okteta core
#include <Okteta/AbstractByteArrayModel>
// KF
#include <KLocalizedString>
// Qt
#include <QTextStream>

namespace Kasten {

IHexStreamEncoderSettings::IHexStreamEncoderSettings() = default;

const char ByteArrayIHexStreamEncoder::hexDigits[16] = {
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
};

void ByteArrayIHexStreamEncoder::streamLine(QTextStream& textStream,
                                            const unsigned char* line)
{
    // checksum is two's complement of sum of the values in the line
    unsigned char checksum = 0;

    textStream << startCode;

    const uint length = byteCountLineSize + addressLineSize + recordTypeLineSize + line[0];
    for (uint i = 0; i < length; ++i) {
        const unsigned char byte = line[i];
        textStream << hexValueOfNibble(byte >> 4)
                   << hexValueOfNibble(byte);
        checksum += byte;
    }

    checksum = (checksum ^ 0xFF) + 1;
    textStream << hexValueOfNibble(checksum >> 4)
               << hexValueOfNibble(checksum) << '\n';
}

void ByteArrayIHexStreamEncoder::streamExtendedSegmentAddress(QTextStream& textStream,
                                                              unsigned char* line,
                                                              quint16 upperSegmentBaseAddress)
{
    constexpr int nullAddress = 0;
    constexpr int upperSegmentBaseAddressSize = 2;

    line[byteCountLineOffset] = upperSegmentBaseAddressSize;
    writeBigEndian(&line[addressLineOffset], nullAddress, addressLineSize);
    line[recordTypeLineOffset] = extendedSegmentAddressRecordCode;
    line[dataLineOffset] = upperSegmentBaseAddress >> 8;
    line[dataLineOffset + 1] = upperSegmentBaseAddress;

    streamLine(textStream, line);
}

void ByteArrayIHexStreamEncoder::streamExtendedLinearAddress(QTextStream& textStream,
                                                             unsigned char* line,
                                                             quint16 upperLinearBaseAddress)
{
    constexpr int nullAddress = 0;
    constexpr int upperLinearBaseAddressSize = 2;

    line[byteCountLineOffset] = upperLinearBaseAddressSize;
    writeBigEndian(&line[addressLineOffset], nullAddress, addressLineSize);
    line[recordTypeLineOffset] = extendedLinearAddressRecordCode;
    line[dataLineOffset] = upperLinearBaseAddress >> 8;
    line[dataLineOffset + 1] = upperLinearBaseAddress;

    streamLine(textStream, line);
}

void ByteArrayIHexStreamEncoder::streamEndOfFile(QTextStream& textStream,
                                                 unsigned char* line,
                                                 quint16 startAddress)
{
    constexpr int endOfFileByteCount = 0;

    line[byteCountLineOffset] = endOfFileByteCount;
    writeBigEndian(&line[addressLineOffset], startAddress, addressLineSize);
    line[recordTypeLineOffset] = endOfFileRecordCode;

    streamLine(textStream, line);
}

ByteArrayIHexStreamEncoder::ByteArrayIHexStreamEncoder()
    : AbstractByteArrayStreamEncoder(i18nc("name of the encoding target", "Intel Hex"), QStringLiteral("text/x-ihex"))
{}

ByteArrayIHexStreamEncoder::~ByteArrayIHexStreamEncoder() = default;

bool ByteArrayIHexStreamEncoder::encodeDataToStream(QIODevice* device,
                                                    const ByteArrayView* byteArrayView,
                                                    const Okteta::AbstractByteArrayModel* byteArrayModel,
                                                    const Okteta::AddressRange& range)
{
    Q_UNUSED(byteArrayView);

    bool success = true;

    // encode
    QTextStream textStream(device);

    // prepare
    constexpr int maxDataPerLineCount = 255;
    constexpr int maxLineLength =
        maxDataPerLineCount + byteCountLineSize + addressLineSize + recordTypeLineSize;

    const Okteta::ByteArrayTableLayout layout(byteArrayView->noOfBytesPerLine(),
                                              byteArrayView->firstLineOffset(),
                                              byteArrayView->startOffset(), 0, byteArrayModel->size());

    const Okteta::Coord startCoord = layout.coordOfIndex(range.start());
    const int lastLinePosition = layout.lastLinePosition(startCoord.line());
    const int dataPerLineCount = qMin(byteArrayView->noOfBytesPerLine(), maxDataPerLineCount);
    unsigned char line[maxLineLength];
    unsigned char* lineData = &line[dataLineOffset];
    Okteta::Address lineOffset = range.start();
    const int firstDataEnd = lastLinePosition - startCoord.pos() + 1;
    int nextUpperAddressChangeDataEnd = 0x10000 - (range.start() & 0xFFFF);
    int d = 0;
    int nextDataEnd = qMin(dataPerLineCount,
                           qMin(firstDataEnd, nextUpperAddressChangeDataEnd));

    // data
    if (mSettings.addressSizeId == IHexStreamEncoderSettings::AddressSizeId::Bits32) {
        const quint16 upperLinearBaseAddress = (range.start() >> 16);
        streamExtendedLinearAddress(textStream, line, upperLinearBaseAddress);
    } else if (mSettings.addressSizeId == IHexStreamEncoderSettings::AddressSizeId::Bits16) {
        const quint16 upperSegmentBaseAddress = (range.start() >> 4) & 0xF000;
        streamExtendedSegmentAddress(textStream, line, upperSegmentBaseAddress);
    }
    Okteta::Address i = range.start();
    while (i <= range.end()) {
        const Okteta::Byte byte = byteArrayModel->byte(i);
        lineData[d] = byte;

        ++d;
        ++i;
        if (d == nextDataEnd) {
            line[byteCountLineOffset] = d;
            writeBigEndian(&line[addressLineOffset], lineOffset, addressLineSize);
            line[recordTypeLineOffset] = dataRecordCode;

            streamLine(textStream, line);

            lineOffset = i;

            if (d == nextUpperAddressChangeDataEnd) {
                if (mSettings.addressSizeId == IHexStreamEncoderSettings::AddressSizeId::Bits32) {
                    const quint16 upperLinearBaseAddress = (i >> 16);
                    streamExtendedLinearAddress(textStream, line, upperLinearBaseAddress);
                } else if (mSettings.addressSizeId == IHexStreamEncoderSettings::AddressSizeId::Bits16) {
                    const quint16 upperSegmentBaseAddress = (i >> 4) & 0xF000;
                    streamExtendedSegmentAddress(textStream, line, upperSegmentBaseAddress);
                }
            }
            nextUpperAddressChangeDataEnd = 0x10000 - (i & 0xFFFF);
            nextDataEnd = qMin(dataPerLineCount,
                               qMin(range.end() - i + 1, nextUpperAddressChangeDataEnd));
            d = 0;
        }
    }

    // footer
    streamEndOfFile(textStream, line);

    return success;
}

}
