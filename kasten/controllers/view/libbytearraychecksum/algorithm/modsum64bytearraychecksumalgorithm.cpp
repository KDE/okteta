/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "modsum64bytearraychecksumalgorithm.hpp"

// Okteta core
#include <Okteta/AbstractByteArrayModel>
// KF
#include <KLocalizedString>
// Qt
#include <QtEndian>

ModSum64ByteArrayChecksumAlgorithm::ModSum64ByteArrayChecksumAlgorithm()
    : AbstractByteArrayChecksumAlgorithm(
        i18nc("name of the checksum algorithm", "Modular sum 64-bit"))
{}

ModSum64ByteArrayChecksumAlgorithm::~ModSum64ByteArrayChecksumAlgorithm() = default;

AbstractByteArrayChecksumParameterSet* ModSum64ByteArrayChecksumAlgorithm::parameterSet() { return &mParameterSet; }

bool ModSum64ByteArrayChecksumAlgorithm::calculateChecksum(QString* result,
                                                           const Okteta::AbstractByteArrayModel* model, const Okteta::AddressRange& range) const
{
    const bool useLittleEndian = (mParameterSet.endianness() == LittleEndian);
    quint64 modSum = useLittleEndian ?
                     calculateModSumWithLittleEndian(model, range) :
                     calculateModSumWithBigEndian(model, range);

    modSum = ~modSum + 1;

    if (useLittleEndian) {
        modSum = qbswap(modSum);
    }

    *result = QStringLiteral("%1").arg(modSum, 16, 16, QChar::fromLatin1('0'));
    return true;
}

quint64 ModSum64ByteArrayChecksumAlgorithm::calculateModSumWithBigEndian(const Okteta::AbstractByteArrayModel* model, const Okteta::AddressRange& range) const
{
    quint64 modSum = 0x00000000;
    Okteta::Address nextBlockEnd = range.start() + CalculatedByteCountSignalLimit;

    // TODO: move padding checks into extra code before and after loop
    for (Okteta::Address i = range.start(); i <= range.end(); ++i) {
        quint64 value = (quint64)((quint8)(model->byte(i))) << 56;
        ++i;
        if (i <= range.end()) {
            value |= (quint64)((quint8)(model->byte(i))) << 48;
            ++i;
            if (i <= range.end()) {
                value |= (quint64)((quint8)(model->byte(i))) << 40;
                ++i;
                if (i <= range.end()) {
                    value |= (quint64)((quint8)(model->byte(i))) << 32;
                    ++i;
                    if (i <= range.end()) {
                        value |= (quint64)((quint8)(model->byte(i))) << 24;
                        ++i;
                        if (i <= range.end()) {
                            value |= (quint64)((quint8)(model->byte(i))) << 16;
                            ++i;
                            if (i <= range.end()) {
                                value |= (quint64)((quint8)(model->byte(i))) << 8;
                                ++i;
                                if (i <= range.end()) {
                                    value |= (quint64)((quint8)(model->byte(i)));
                                }
                            }
                        }
                    }
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

quint64 ModSum64ByteArrayChecksumAlgorithm::calculateModSumWithLittleEndian(const Okteta::AbstractByteArrayModel* model, const Okteta::AddressRange& range) const
{
    quint64 modSum = 0x00000000;
    Okteta::Address nextBlockEnd = range.start() + CalculatedByteCountSignalLimit;

    // TODO: move padding checks into extra code before and after loop
    for (Okteta::Address i = range.start(); i <= range.end(); ++i) {
        quint64 value = (quint8)(model->byte(i));
        ++i;
        if (i <= range.end()) {
            value |= (quint64)((quint8)(model->byte(i))) << 8;
            ++i;
            if (i <= range.end()) {
                value |= (quint64)((quint8)(model->byte(i))) << 16;
                ++i;
                if (i <= range.end()) {
                    value |= (quint64)((quint8)(model->byte(i))) << 24;
                    ++i;
                    if (i <= range.end()) {
                        value |= (quint64)((quint8)(model->byte(i))) << 32;
                        ++i;
                        if (i <= range.end()) {
                            value |= (quint64)((quint8)(model->byte(i))) << 40;
                            ++i;
                            if (i <= range.end()) {
                                value |= (quint64)((quint8)(model->byte(i))) << 48;
                                ++i;
                                if (i <= range.end()) {
                                    value |= (quint64)((quint8)(model->byte(i))) << 56;
                                }
                            }
                        }
                    }
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
