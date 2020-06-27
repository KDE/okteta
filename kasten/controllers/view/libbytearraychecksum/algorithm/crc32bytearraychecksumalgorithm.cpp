/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "crc32bytearraychecksumalgorithm.hpp"

// Okteta core
#include <Okteta/AbstractByteArrayModel>
// KF
#include <KLocalizedString>

class Crc32LookupTable
{
public:
    Crc32LookupTable();

public:
    const quint32& operator[](int i) const;

private:
    static quint32 reverseBits(quint32 bits, char bitCount);

private:
    quint32 mTable[256];
};

Crc32LookupTable::Crc32LookupTable()
{
    quint32 polynomial = 0x04c11db7;

    // 256 values representing ASCII character codes.
    for (int i = 0; i < 256; ++i) {
        int value = reverseBits(i, 8) << 24;
        for (int j = 0; j < 8; ++j) {
            const bool hasMsb = (value & (1 << 31));
            value <<= 1;
            if (hasMsb) {
                value ^= polynomial;
            }
        }

        mTable[i] = reverseBits(value, 32);
    }
}

quint32 Crc32LookupTable::reverseBits(quint32 bits, char bitCount)
{
    quint32 result = 0;
    for (int i = 1; i <= bitCount; ++i) {
        if (bits & 0x01) {
            result |= (1 << (bitCount - i));
        }
        bits >>= 1;
    }

    return result;
}

inline const quint32& Crc32LookupTable::operator[](int i) const { return mTable[i]; }

Crc32ByteArrayChecksumAlgorithm::Crc32ByteArrayChecksumAlgorithm()
    : AbstractByteArrayChecksumAlgorithm(
        i18nc("name of the checksum algorithm, Cyclic Redundancy Check 32", "CRC-32"))
{}

Crc32ByteArrayChecksumAlgorithm::~Crc32ByteArrayChecksumAlgorithm() = default;

AbstractByteArrayChecksumParameterSet* Crc32ByteArrayChecksumAlgorithm::parameterSet() { return &mParameterSet; }

bool Crc32ByteArrayChecksumAlgorithm::calculateChecksum(QString* result,
                                                        const Okteta::AbstractByteArrayModel* model, const Okteta::AddressRange& range) const
{
    Crc32LookupTable lookupTable;
    quint32 crcBits = 0xffffffff;
    Okteta::Address nextBlockEnd = range.start() + CalculatedByteCountSignalLimit;
    for (Okteta::Address i = range.start(); i <= range.end(); ++i) {
        const uchar value = (crcBits & 0xFF) ^ model->byte(i);
        crcBits >>= 8;
        crcBits ^= lookupTable[value];

        if (i >= nextBlockEnd) {
            nextBlockEnd += CalculatedByteCountSignalLimit;
            emit calculatedBytes(range.localIndex(i) + 1);
        }
    }

    crcBits ^= 0xffffffff;

    *result = QStringLiteral("%1").arg(crcBits, 8, 16, QChar::fromLatin1('0'));

    return true;
}
