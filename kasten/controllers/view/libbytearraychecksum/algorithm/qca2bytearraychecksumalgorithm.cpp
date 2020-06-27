/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

// QCA
// need to have this first, as QCA needs QT_NO_CAST_FROM_ASCII disabled when included
#include <config-qca2.hpp> // krazy:excludeall=includes
#ifdef HAVE_QCA2
// disable QT_NO_CAST_FROM_ASCII
#ifdef QT_NO_CAST_FROM_ASCII
#undef QT_NO_CAST_FROM_ASCII
#endif
#include <QtCrypto>
#endif

#include "qca2bytearraychecksumalgorithm.hpp"

// Okteta core
#include <Okteta/AbstractByteArrayModel>
// Qt
#include <QByteArray>

Qca2ByteArrayChecksumAlgorithm::Qca2ByteArrayChecksumAlgorithm(const QString& name, const QString& type)
    : AbstractByteArrayChecksumAlgorithm(name)
    , mType(type)
{}

Qca2ByteArrayChecksumAlgorithm::~Qca2ByteArrayChecksumAlgorithm() = default;

AbstractByteArrayChecksumParameterSet* Qca2ByteArrayChecksumAlgorithm::parameterSet() { return &mParameterSet; }

bool Qca2ByteArrayChecksumAlgorithm::calculateChecksum(QString* result,
                                                       const Okteta::AbstractByteArrayModel* model, const Okteta::AddressRange& range) const
{
    QCA::Hash hash(mType);

    // TODO: find a way without needing to copy, perhaps by smart iterator which can return spans of original data
    // TODO: see if buffer size could be a value which matches the algorithm and qca2

    char buffer[CalculatedByteCountSignalLimit];
    int bufferLength = CalculatedByteCountSignalLimit;
    Okteta::Address nextBlockEnd = range.start() + CalculatedByteCountSignalLimit;
    for (Okteta::Address i = range.start(); i <= range.end(); i += CalculatedByteCountSignalLimit) {
        if (range.end() < i + CalculatedByteCountSignalLimit) {
            bufferLength = range.end() - i + 1;
        }
        model->copyTo(reinterpret_cast<Okteta::Byte*>(buffer), i, bufferLength);
        hash.update(buffer, bufferLength);

        if (i >= nextBlockEnd) {
            nextBlockEnd += CalculatedByteCountSignalLimit;
            emit calculatedBytes(range.localIndex(i) + 1);
        }
    }

    const QByteArray hashResult = hash.final ().toByteArray();

    *result = QCA::arrayToHex(hashResult);
    return true;
}
