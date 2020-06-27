/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2010 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BYTEARRAYIHEXSTREAMENCODER_HPP
#define KASTEN_BYTEARRAYIHEXSTREAMENCODER_HPP

// lib
#include "abstractbytearraystreamencoder.hpp"
// Okteta core
#include <Okteta/OktetaCore>
// Qt
#include <QString>

class QTextStream;

namespace Kasten {

class IHexStreamEncoderSettings
{
public:
    enum class AddressSizeId
    {
        Bits32 = 0,
        Bits16 = 1,
        Bits8 = 2
    };

public:
    IHexStreamEncoderSettings();

public:
    AddressSizeId addressSizeId = AddressSizeId::Bits32;
};

class ByteArrayIHexStreamEncoder : public AbstractByteArrayStreamEncoder
{
    Q_OBJECT

private:
    static constexpr unsigned char dataRecordCode = 0x0;
    static constexpr unsigned char endOfFileRecordCode = 0x1;
    static constexpr unsigned char extendedSegmentAddressRecordCode = 0x2;
    static constexpr unsigned char startSegmentAddressRecordCode = 0x3;
    static constexpr unsigned char extendedLinearAddressRecordCode = 0x4;
    static constexpr unsigned char startLinearAddressRecordCode = 0x5;

    static constexpr char startCode = ':';

    static constexpr int byteCountLineOffset = 0;
    static constexpr int byteCountLineSize = 1;
    static constexpr int addressLineOffset = byteCountLineOffset + byteCountLineSize;
    static constexpr int addressLineSize = 2;
    static constexpr int recordTypeLineOffset = addressLineOffset + addressLineSize;
    static constexpr int recordTypeLineSize = 1;
    static constexpr int dataLineOffset = recordTypeLineOffset + recordTypeLineSize;
    static const char hexDigits[16];

private:
    static char hexValueOfNibble(int nibble);
    static void writeBigEndian(unsigned char* line, quint32 value, int byteSize);

    static void streamLine(QTextStream& textStream, const unsigned char* line);
    static void streamExtendedSegmentAddress(QTextStream& textStream, unsigned char* line,
                                             quint16 upperSegmentBaseAddress);
    static void streamExtendedLinearAddress(QTextStream& textStream, unsigned char* line,
                                            quint16 upperLinearBaseAddress);
    static void streamEndOfFile(QTextStream& textStream, unsigned char* line,
                                quint16 startAddress = 0);

public:
    ByteArrayIHexStreamEncoder();
    ~ByteArrayIHexStreamEncoder() override;

public:
    IHexStreamEncoderSettings settings() const;
    void setSettings(const IHexStreamEncoderSettings& settings);

protected: // AbstractByteArrayStreamEncoder API
    bool encodeDataToStream(QIODevice* device,
                            const ByteArrayView* byteArrayView,
                            const Okteta::AbstractByteArrayModel* byteArrayModel,
                            const Okteta::AddressRange& range) override;

private:
    IHexStreamEncoderSettings mSettings;
};

inline IHexStreamEncoderSettings ByteArrayIHexStreamEncoder::settings() const { return mSettings; }
inline void ByteArrayIHexStreamEncoder::setSettings(const IHexStreamEncoderSettings& settings)
{
    mSettings = settings;
    emit settingsChanged();
}

inline char ByteArrayIHexStreamEncoder::hexValueOfNibble(int nibble)
{ return hexDigits[nibble & 0xF]; }

inline void ByteArrayIHexStreamEncoder::writeBigEndian(unsigned char* line, quint32 value, int byteSize)
{
    while (byteSize > 0) {
        --byteSize;
        line[byteSize] = value;
        value >>= 8;
    }
}

}

#endif
