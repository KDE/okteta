/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2010 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef BYTEARRAYIHEXSTREAMENCODER_H
#define BYTEARRAYIHEXSTREAMENCODER_H

// lib
#include "abstractbytearraystreamencoder.h"
// Okteta core
#include <okteta/oktetacore.h>
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
    static const unsigned char dataRecordCode = 0x0;
    static const unsigned char endOfFileRecordCode = 0x1;
    static const unsigned char extendedSegmentAddressRecordCode = 0x2;
    static const unsigned char startSegmentAddressRecordCode = 0x3;
    static const unsigned char extendedLinearAddressRecordCode = 0x4;
    static const unsigned char startLinearAddressRecordCode = 0x5;

    static const char startCode = ':';

    static const int byteCountLineOffset = 0;
    static const int byteCountLineSize = 1;
    static const int addressLineOffset = byteCountLineOffset + byteCountLineSize;
    static const int addressLineSize = 2;
    static const int recordTypeLineOffset = addressLineOffset + addressLineSize;
    static const int recordTypeLineSize = 1;
    static const int dataLineOffset = recordTypeLineOffset + recordTypeLineSize;
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

protected:
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
