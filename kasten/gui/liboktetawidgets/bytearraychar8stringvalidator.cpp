/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2024 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearraychar8stringvalidator.hpp"

// lib
#include "bytearraychar8stringdecoder.hpp"
// Okteta core
#include <Okteta/CharCodec>

namespace Okteta {

ByteArrayChar8StringValidator::ByteArrayChar8StringValidator() = default;

ByteArrayChar8StringValidator::~ByteArrayChar8StringValidator() = default;

void ByteArrayChar8StringValidator::setCharCodec(const QString& charCodecName)
{
    m_decoder.setCharCodec(charCodecName);
}

QValidator::State ByteArrayChar8StringValidator::validate(QString& string, int& pos) const
{
    Q_UNUSED(pos)

    QValidator::State result = QValidator::Acceptable;

    int usedStringLength = 0;
    int resultSize = 0;
    const ByteArrayChar8StringDecoder::CodeState evalResult = m_decoder.validate(&resultSize, string, minLength(), maxLength(), &usedStringLength);

    result =
        (evalResult == ByteArrayChar8StringDecoder::CodeAcceptable) ?   QValidator::Acceptable :
        (evalResult == ByteArrayChar8StringDecoder::CodeIntermediate) ? QValidator::Intermediate :
        /* else */                                                      QValidator::Invalid;

    if ((result != QValidator::Invalid)) {
        if (usedStringLength < string.size()) {
            result = QValidator::Invalid;
        } else if (resultSize < minLength()) {
            result = QValidator::Intermediate;
        }
    }

    return result;
}

QByteArray ByteArrayChar8StringValidator::toByteArray(const QString& string) const
{
    QByteArray result;

    std::ignore = m_decoder.decode(&result, string, minLength(), maxLength());

    if (result.size() < minLength()) {
        result.append(QByteArray(minLength() - result.size(), '\0'));
    }

    return result;
}

QString ByteArrayChar8StringValidator::toString(const QByteArray& byteArray) const
{
    const int byteArraySize = qMin(byteArray.size(), maxLength());

    QString result = m_encoder.encodeAsString(byteArray.constData(), byteArraySize, m_decoder.charCodec());

    if (byteArraySize < minLength()) {
        result += QStringLiteral("\\x00").repeated(minLength() - byteArraySize);
    }

    return result;
}

}
