/*
    SPDX-FileCopyrightText: 2010 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BYTEARRAYSRECSTREAMENCODER_HPP
#define KASTEN_BYTEARRAYSRECSTREAMENCODER_HPP

// lib
#include "abstractbytearraystreamencoder.hpp"
// Qt
#include <QString>
// Std
#include <array>

class KConfigGroup;

class QTextStream;

namespace Kasten {

class SRecStreamEncoderSettings
{
public:
    enum class AddressSizeId
    {
        FourBytes = 0,
        ThreeBytes = 1,
        TwoBytes = 2,
        _Count,
    };

    static constexpr auto AddressSizeCount = static_cast<std::size_t>(AddressSizeId::_Count);
    static const std::array<QString, AddressSizeCount> addressSizeConfigValueList;

private:
    static constexpr AddressSizeId DefaultAddressSize = AddressSizeId::FourBytes;

    static constexpr char AddressSizeConfigKey[] = "AddressSize";

public:
    SRecStreamEncoderSettings();
    SRecStreamEncoderSettings(const SRecStreamEncoderSettings&) = default;
    SRecStreamEncoderSettings(SRecStreamEncoderSettings&&) = default;

    ~SRecStreamEncoderSettings() = default;

    SRecStreamEncoderSettings& operator=(const SRecStreamEncoderSettings&) = default;
    SRecStreamEncoderSettings& operator=(SRecStreamEncoderSettings&&) = default;

    [[nodiscard]]
    bool operator==(const SRecStreamEncoderSettings& other) const;

public:
    void loadConfig(const KConfigGroup& configGroup);
    void saveConfig(KConfigGroup& configGroup) const;

public:
    AddressSizeId addressSizeId = DefaultAddressSize;
};

class ByteArraySRecStreamEncoder : public AbstractByteArrayStreamEncoder
{
    Q_OBJECT

private:
    enum class RecordType
    {
        BlockHeader = 0,
        DataSequence2B = 1,
        DataSequence3B = 2,
        DataSequence4B = 3,
        RecordCount = 5,
        EndOfBlock4B = 7,
        EndOfBlock3B = 8,
        EndOfBlock2B = 9
    };

    static constexpr char startCode = 'S';

    static constexpr int byteCountLineOffset = 0;
    static constexpr int byteCountLineSize = 1;
    static constexpr int addressLineOffset = byteCountLineOffset + byteCountLineSize;

    static const std::array<char, 16> hexDigits;

    static const QString ConfigGroupId;

private:
    [[nodiscard]]
    static RecordType dataSequenceRecordType(SRecStreamEncoderSettings::AddressSizeId id);
    [[nodiscard]]
    static RecordType endOfBlockRecordType(SRecStreamEncoderSettings::AddressSizeId id);
    [[nodiscard]]
    static int endOfBlockAddressSize(RecordType type);
    [[nodiscard]]
    static char charOfRecordType(RecordType type);
    [[nodiscard]]
    static char hexValueOfNibble(int nibble);
    static void writeBigEndian(unsigned char* line, quint32 value, int byteSize);
    static void streamLine(QTextStream& textStream, RecordType recordType,
                           const unsigned char* line);
    static void streamBlockHeader(QTextStream& textStream, unsigned char* line);
    static void streamRecordCount(QTextStream& textStream, unsigned char* line,
                                  quint16 recordCount);
    static void streamBlockEnd(QTextStream& textStream, unsigned char* line,
                               RecordType recordType, quint32 startAddress = 0);   // TODO: make address

public:
    ByteArraySRecStreamEncoder();
    ~ByteArraySRecStreamEncoder() override;

public:
    [[nodiscard]]
    SRecStreamEncoderSettings settings() const;
    void setSettings(const SRecStreamEncoderSettings& settings);

protected: // AbstractByteArrayStreamEncoder API
    [[nodiscard]]
    bool encodeDataToStream(QIODevice* device,
                            const ByteArrayView* byteArrayView,
                            const Okteta::AbstractByteArrayModel* byteArrayModel,
                            const Okteta::AddressRange& range) override;

private:
    SRecStreamEncoderSettings mSettings;
};

inline SRecStreamEncoderSettings ByteArraySRecStreamEncoder::settings() const { return mSettings; }

inline char ByteArraySRecStreamEncoder::charOfRecordType(RecordType type)
{ return static_cast<char>('0' + static_cast<int>(type)); }
inline char ByteArraySRecStreamEncoder::hexValueOfNibble(int nibble)
{
    const auto index = static_cast<std::size_t>(nibble & 0xF);
    return hexDigits[index];
}

inline void ByteArraySRecStreamEncoder::writeBigEndian(unsigned char* line,
                                                       quint32 value, int byteSize)
{
    while (byteSize > 0) {
        --byteSize;
        line[byteSize] = value;
        value >>= 8;
    }
}

inline ByteArraySRecStreamEncoder::RecordType
ByteArraySRecStreamEncoder::dataSequenceRecordType(SRecStreamEncoderSettings::AddressSizeId id)
{
    return static_cast<RecordType>(static_cast<int>(RecordType::DataSequence4B) - static_cast<int>(id));
}

inline ByteArraySRecStreamEncoder::RecordType
ByteArraySRecStreamEncoder::endOfBlockRecordType(SRecStreamEncoderSettings::AddressSizeId id)
{
    return static_cast<RecordType>(static_cast<int>(RecordType::EndOfBlock4B) + static_cast<int>(id));
}

inline int ByteArraySRecStreamEncoder::endOfBlockAddressSize(RecordType type)
{
    return 11 - static_cast<int>(type);
}

}

#endif
