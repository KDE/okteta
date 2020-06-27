/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2010 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BYTEARRAYSRECSTREAMENCODER_HPP
#define KASTEN_BYTEARRAYSRECSTREAMENCODER_HPP

// lib
#include "abstractbytearraystreamencoder.hpp"
// Okteta core
#include <Okteta/OktetaCore>
// Qt
#include <QString>

class QTextStream;

namespace Kasten {

class SRecStreamEncoderSettings
{
public:
    enum class AddressSizeId
    {
        FourBytes = 0,
        ThreeBytes = 1,
        TwoBytes = 2
    };

public:
    SRecStreamEncoderSettings();

public:
    AddressSizeId addressSizeId = AddressSizeId::FourBytes;
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

    static const char hexDigits[16];

private:
    static RecordType dataSequenceRecordType(SRecStreamEncoderSettings::AddressSizeId id);
    static RecordType endOfBlockRecordType(SRecStreamEncoderSettings::AddressSizeId id);
    static int endOfBlockAddressSize(RecordType type);
    static char charOfRecordType(RecordType type);
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
    SRecStreamEncoderSettings settings() const;
    void setSettings(const SRecStreamEncoderSettings& settings);

protected: // AbstractByteArrayStreamEncoder API
    bool encodeDataToStream(QIODevice* device,
                            const ByteArrayView* byteArrayView,
                            const Okteta::AbstractByteArrayModel* byteArrayModel,
                            const Okteta::AddressRange& range) override;

private:
    SRecStreamEncoderSettings mSettings;
};

inline SRecStreamEncoderSettings ByteArraySRecStreamEncoder::settings() const { return mSettings; }
inline void ByteArraySRecStreamEncoder::setSettings(const SRecStreamEncoderSettings& settings)
{
    mSettings = settings;
    emit settingsChanged();
}

inline char ByteArraySRecStreamEncoder::charOfRecordType(RecordType type)
{ return static_cast<char>('0' + static_cast<int>(type)); }
inline char ByteArraySRecStreamEncoder::hexValueOfNibble(int nibble)
{ return hexDigits[nibble & 0xF]; }

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
