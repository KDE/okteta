/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2019 Lars Maier <lars.maier@tefax.net>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "crc64bytearraychecksumalgorithm.hpp"

// Okteta core
#include <Okteta/AbstractByteArrayModel>
// KF
#include <KLocalizedString>

struct Crc64AlgorithmSpec
{
    quint64 polynomial;
    quint64 initialValue;
    quint64 finalXorValue;
    bool inputReflected;
    bool outputReflected;
};

static const Crc64AlgorithmSpec algorithms[] = {
    // ECMA-182
    { 0x42F0E1EBA9EA3693, 0x0, 0x0, false, false },

    // ISO-3309
    { 0x000000000000001B, 0xFFFFFFFFFFFFFFFFllu,0xFFFFFFFFFFFFFFFFllu, true, true}

    // Add more algorithms here
};

static void fillTable(quint64 poly, quint64 *table)
{
    for (size_t i = 0; i < 256; i++) {
        quint64 crc = 0;
        for (size_t j = 0; j < 8; j++) {
            bool b = (i >> (7u - j)) & 0x01u;
            if (((crc >> 63u) != 0) != b) {
                crc = (crc << 1u) ^ poly;
            } else {
                crc = (crc << 1u);
            }
        }
        table[i] = crc;
    }
}

static quint64 reflect64(quint64 x)
{
    quint64 y = 0;
    for (size_t i = 0; i < 64; i++) {
        if ((x >> i) & 0x01u) {
            y |= 0x01llu << (63 - i);
        }
    }

    return y;
}

static uchar reflect8(uchar x)
{
    uchar y = 0;

    for (size_t i = 0; i < 8; i++) {
        if ((x >> i) & 0x01u) {
            y |= 0x01u << (7 - i);
        }
    }

    return y;
}

Crc64ByteArrayChecksumAlgorithm::Crc64ByteArrayChecksumAlgorithm()
    : AbstractByteArrayChecksumAlgorithm(
        i18nc("name of the checksum algorithm, Cyclic Redundancy Check 64", "CRC-64"))
{}

Crc64ByteArrayChecksumAlgorithm::~Crc64ByteArrayChecksumAlgorithm() = default;

AbstractByteArrayChecksumParameterSet*
Crc64ByteArrayChecksumAlgorithm::parameterSet() { return &mParameterSet; }

bool Crc64ByteArrayChecksumAlgorithm::calculateChecksum(QString* result,
                                                        const Okteta::AbstractByteArrayModel* model, const Okteta::AddressRange& range) const
{
    const Crc64AlgorithmSpec *spec = &algorithms[static_cast<size_t>(mParameterSet.crc64Variant())];

    quint64 lookupTable[256];
    fillTable(spec->polynomial, lookupTable);

    quint64 crcBits = spec->initialValue;
    Okteta::Address nextBlockEnd = range.start() + CalculatedByteCountSignalLimit;
    for (Okteta::Address i = range.start(); i <= range.end(); ++i) {

        uchar value = model->byte(i);
        if (spec->inputReflected) {
            value = reflect8(value);
        }

        const uchar idx = (crcBits >> 56u) ^ value;
        crcBits = (crcBits << 8u) ^ lookupTable[idx & 0xffu];

        if (i >= nextBlockEnd) {
            nextBlockEnd += CalculatedByteCountSignalLimit;
            emit calculatedBytes(range.localIndex(i) + 1);
        }
    }

    if (spec->outputReflected) {
        crcBits = reflect64(crcBits);
    }
    crcBits = crcBits ^ spec->finalXorValue;

    *result = QStringLiteral("%1").arg(crcBits, 16, 16, QChar::fromLatin1('0'));
    return true;
}
