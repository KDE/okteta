/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "modsum8bytearraychecksumalgorithm.hpp"

// Okteta core
#include <Okteta/AbstractByteArrayModel>
// KF
#include <KLocalizedString>
// Qt
#include <QtGlobal>

ModSum8ByteArrayChecksumAlgorithm::ModSum8ByteArrayChecksumAlgorithm()
    : AbstractByteArrayChecksumAlgorithm(
        i18nc("name of the checksum algorithm", "Modular sum 8-bit"))
{}

ModSum8ByteArrayChecksumAlgorithm::~ModSum8ByteArrayChecksumAlgorithm() = default;

AbstractByteArrayChecksumParameterSet* ModSum8ByteArrayChecksumAlgorithm::parameterSet() { return &mParameterSet; }

bool ModSum8ByteArrayChecksumAlgorithm::calculateChecksum(QString* result,
                                                          const Okteta::AbstractByteArrayModel* model, const Okteta::AddressRange& range) const
{
    quint8 modSum = 0x00;
    Okteta::Address nextBlockEnd = range.start() + CalculatedByteCountSignalLimit;
    for (Okteta::Address i = range.start(); i <= range.end(); ++i) {
        modSum += (quint8)(model->byte(i));
#if 0
        const uchar value = (crcBits & 0xFF) + model->datum(i);
        crcBits >>= 8;
        crcBits ^= lookupTable[value];
#endif
        if (i >= nextBlockEnd) {
            nextBlockEnd += CalculatedByteCountSignalLimit;
            emit calculatedBytes(range.localIndex(i) + 1);
        }
    }

    modSum = ~modSum + 1;

    *result = QStringLiteral("%1").arg(modSum, 2, 16, QChar::fromLatin1('0'));
    return true;
}
