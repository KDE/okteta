/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "modsum32bytearraychecksumalgorithm.hpp"

// Okteta core
#include <Okteta/AbstractByteArrayModel>
// KF
#include <KLocalizedString>
// Qt
#include <QtEndian>

ModSum32ByteArrayChecksumAlgorithm::ModSum32ByteArrayChecksumAlgorithm()
    : AbstractByteArrayChecksumAlgorithm(
        i18nc("name of the checksum algorithm", "Modular sum 32-bit"))
{}

ModSum32ByteArrayChecksumAlgorithm::~ModSum32ByteArrayChecksumAlgorithm() = default;

AbstractByteArrayChecksumParameterSet* ModSum32ByteArrayChecksumAlgorithm::parameterSet() { return &mParameterSet; }

bool ModSum32ByteArrayChecksumAlgorithm::calculateChecksum(QString* result,
                                                           const Okteta::AbstractByteArrayModel* model, const Okteta::AddressRange& range) const
{
    const bool useLittleEndian = (mParameterSet.endianness() == LittleEndian);
    quint32 modSum = useLittleEndian ?
                     calculateModSumWithLittleEndian(model, range) :
                     calculateModSumWithBigEndian(model, range);

    modSum = ~modSum + 1;

    if (useLittleEndian) {
        modSum = qbswap(modSum);
    }

    *result = QStringLiteral("%1").arg(modSum, 8, 16, QChar::fromLatin1('0'));
    return true;
}

quint32 ModSum32ByteArrayChecksumAlgorithm::calculateModSumWithBigEndian(const Okteta::AbstractByteArrayModel* model, const Okteta::AddressRange& range) const
{
    quint32 modSum = 0x000000;
    Okteta::Address nextBlockEnd = range.start() + CalculatedByteCountSignalLimit;

    // TODO: move padding checks into extra code before and after loop
    for (Okteta::Address i = range.start(); i <= range.end(); ++i) {
        quint32 value = (quint32)(quint8)(model->byte(i)) << 24;
        ++i;
        if (i <= range.end()) {
            value |= (quint32)(quint8)(model->byte(i)) << 16;
            ++i;
            if (i <= range.end()) {
                value |= (quint32)(quint8)(model->byte(i)) << 8;
                ++i;
                if (i <= range.end()) {
                    value |= (quint32)(quint8)(model->byte(i));
                }
            }
        }

        modSum += value;
#if 0
        const uchar value = (crcBits & 0xFF) + model->byte(i);
        crcBits >>= 8;
        crcBits ^= lookupTable[value];
#endif
        if (i >= nextBlockEnd) {
            nextBlockEnd += CalculatedByteCountSignalLimit;
            emit calculatedBytes(range.localIndex(i) + 1);
        }
    }

    return modSum;
}

quint32 ModSum32ByteArrayChecksumAlgorithm::calculateModSumWithLittleEndian(const Okteta::AbstractByteArrayModel* model, const Okteta::AddressRange& range) const
{
    quint32 modSum = 0x000000;
    Okteta::Address nextBlockEnd = range.start() + CalculatedByteCountSignalLimit;

    // TODO: move padding checks into extra code before and after loop
    for (Okteta::Address i = range.start(); i <= range.end(); ++i) {
        quint32 value = (quint32)(quint8)(model->byte(i));
        ++i;
        if (i <= range.end()) {
            value |= (quint32)(quint8)(model->byte(i)) << 8;
            ++i;
            if (i <= range.end()) {
                value |= (quint32)(quint8)(model->byte(i)) << 16;
                ++i;
                if (i <= range.end()) {
                    value |= (quint32)(quint8)(model->byte(i)) << 24;
                }
            }
        }

        modSum += value;

        if (i >= nextBlockEnd) {
            nextBlockEnd += CalculatedByteCountSignalLimit;
            emit calculatedBytes(range.localIndex(i) + 1);
        }
    }

    return modSum;
}
