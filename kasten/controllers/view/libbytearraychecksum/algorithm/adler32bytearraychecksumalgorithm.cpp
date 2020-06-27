/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "adler32bytearraychecksumalgorithm.hpp"

// Okteta core
#include <Okteta/AbstractByteArrayModel>
// KF
#include <KLocalizedString>

static constexpr int MOD_ADLER = 65521;

Adler32ByteArrayChecksumAlgorithm::Adler32ByteArrayChecksumAlgorithm()
    : AbstractByteArrayChecksumAlgorithm(
        i18nc("name of the checksum algorithm", "Adler-32"))
{}

Adler32ByteArrayChecksumAlgorithm::~Adler32ByteArrayChecksumAlgorithm() = default;

AbstractByteArrayChecksumParameterSet* Adler32ByteArrayChecksumAlgorithm::parameterSet() { return &mParameterSet; }

bool Adler32ByteArrayChecksumAlgorithm::calculateChecksum(QString* result,
                                                          const Okteta::AbstractByteArrayModel* model, const Okteta::AddressRange& range) const
{
    quint32 a = 1;
    quint32 b = 0;

    // TODO: this is the "inefficient but straightforward implementation" from the Wikipedia entry, search for improved
    Okteta::Address nextBlockEnd = range.start() + CalculatedByteCountSignalLimit;
    for (Okteta::Address i = range.start(); i <= range.end(); ++i) {
        a = (a + model->byte(i)) % MOD_ADLER;
        b = (b + a) % MOD_ADLER;

        if (i >= nextBlockEnd) {
            nextBlockEnd += CalculatedByteCountSignalLimit;
            emit calculatedBytes(range.localIndex(i) + 1);
        }
    }

    const quint32 sum = (b << 16) | a;
    *result = QStringLiteral("%1").arg(sum, 8, 16, QChar::fromLatin1('0'));

    return true;
}
