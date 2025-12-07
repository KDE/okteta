/*
    SPDX-FileCopyrightText: 2009, 2023 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "qcryptographicbytearraychecksumalgorithm.hpp"

// Okteta core
#include <Okteta/AbstractByteArrayModel>
// Qt
#include <QByteArray>

QCryptographicByteArrayChecksumAlgorithm::QCryptographicByteArrayChecksumAlgorithm(const QString& name,
                                                                                   const QString& id,
                                                                                   QCryptographicHash::Algorithm algorithm)
    : AbstractByteArrayChecksumAlgorithm(name, id)
    , m_algorithm(algorithm)
{
}

QCryptographicByteArrayChecksumAlgorithm::~QCryptographicByteArrayChecksumAlgorithm() = default;

AbstractByteArrayChecksumParameterSet* QCryptographicByteArrayChecksumAlgorithm::parameterSet() { return &mParameterSet; }

bool QCryptographicByteArrayChecksumAlgorithm::calculateChecksum(QString* result,
                                                                 const Okteta::AbstractByteArrayModel* model,
                                                                 const Okteta::AddressRange& range) const
{
    QCryptographicHash hash(m_algorithm);

    // TODO: find a way without needing to copy, perhaps by smart iterator which can return spans of original data or using some QIODevice wrapper
    // TODO: see if buffer size could be a value which matches the algorithm and QCryptographicHash

    char buffer[CalculatedByteCountSignalLimit];
    int bufferLength = CalculatedByteCountSignalLimit;
    Okteta::Address nextBlockEnd = range.start() + CalculatedByteCountSignalLimit;
    for (Okteta::Address i = range.start(); i <= range.end(); i += CalculatedByteCountSignalLimit) {
        if (range.end() < i + CalculatedByteCountSignalLimit) {
            bufferLength = range.end() - i + 1;
        }
        std::ignore = model->copyTo(reinterpret_cast<Okteta::Byte*>(buffer), i, bufferLength);
        hash.addData(QByteArrayView(buffer, bufferLength));

        if (i >= nextBlockEnd) {
            nextBlockEnd += CalculatedByteCountSignalLimit;
            Q_EMIT calculatedBytes(range.localIndex(i) + 1);
        }
    }

    const QByteArray hashResult = hash.result();

    *result = QString::fromLatin1(hashResult.toHex());
    return true;
}

#include "moc_qcryptographicbytearraychecksumalgorithm.cpp"
