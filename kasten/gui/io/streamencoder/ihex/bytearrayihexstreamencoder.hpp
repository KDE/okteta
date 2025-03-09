/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2010 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BYTEARRAYIHEXSTREAMENCODER_HPP
#define KASTEN_BYTEARRAYIHEXSTREAMENCODER_HPP

// lib
#include "abstractbytearraystreamencoder.hpp"
// Qt
#include <QString>
// Std
#include <array>

class KConfigGroup;

class QTextStream;

namespace Kasten {

class IHexStreamEncoderSettings
{
public:
    enum class AddressSizeId
    {
        Bits32 = 0,
        Bits16 = 1,
        Bits8 = 2,
        _Count,
    };

    static inline constexpr int AddressSizeCount = static_cast<int>(AddressSizeId::_Count);
    static const std::array<QString, AddressSizeCount> addressSizeConfigValueList;

private:
    static inline constexpr char AddressSizeConfigKey[] = "AddressSize";

    static inline constexpr AddressSizeId DefaultAddressSize = AddressSizeId::Bits32;

public:
    IHexStreamEncoderSettings();
    IHexStreamEncoderSettings(const IHexStreamEncoderSettings&) = default;
    IHexStreamEncoderSettings(IHexStreamEncoderSettings&&) = default;

    ~IHexStreamEncoderSettings() = default;

    IHexStreamEncoderSettings& operator=(const IHexStreamEncoderSettings&) = default;
    IHexStreamEncoderSettings& operator=(IHexStreamEncoderSettings&&) = default;

    [[nodiscard]]
    bool operator==(const IHexStreamEncoderSettings& other) const;

public:
    void loadConfig(const KConfigGroup& configGroup);
    void saveConfig(KConfigGroup& configGroup) const;

public:
    AddressSizeId addressSizeId = AddressSizeId::Bits32;
};

class ByteArrayIHexStreamEncoder : public AbstractByteArrayStreamEncoder
{
    Q_OBJECT

private:
    static inline constexpr char ConfigGroupId[] = "ByteArrayIntelHexStreamEncoder";

    static inline constexpr unsigned char dataRecordCode = 0x0;
    static inline constexpr unsigned char endOfFileRecordCode = 0x1;
    static inline constexpr unsigned char extendedSegmentAddressRecordCode = 0x2;
    static inline constexpr unsigned char startSegmentAddressRecordCode = 0x3;
    static inline constexpr unsigned char extendedLinearAddressRecordCode = 0x4;
    static inline constexpr unsigned char startLinearAddressRecordCode = 0x5;

    static inline constexpr char startCode = ':';

    static inline constexpr int byteCountLineOffset = 0;
    static inline constexpr int byteCountLineSize = 1;
    static inline constexpr int addressLineOffset = byteCountLineOffset + byteCountLineSize;
    static inline constexpr int addressLineSize = 2;
    static inline constexpr int recordTypeLineOffset = addressLineOffset + addressLineSize;
    static inline constexpr int recordTypeLineSize = 1;
    static inline constexpr int dataLineOffset = recordTypeLineOffset + recordTypeLineSize;
    static const char hexDigits[16];

private:
    [[nodiscard]]
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
    [[nodiscard]]
    IHexStreamEncoderSettings settings() const;
    void setSettings(const IHexStreamEncoderSettings& settings);

protected: // AbstractByteArrayStreamEncoder API
    [[nodiscard]]
    bool encodeDataToStream(QIODevice* device,
                            const ByteArrayView* byteArrayView,
                            const Okteta::AbstractByteArrayModel* byteArrayModel,
                            const Okteta::AddressRange& range) override;

private:
    IHexStreamEncoderSettings mSettings;
};

inline IHexStreamEncoderSettings ByteArrayIHexStreamEncoder::settings() const { return mSettings; }

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
