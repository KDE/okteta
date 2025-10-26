/*
    SPDX-FileCopyrightText: 2024 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearrayutf8stringvalidator.hpp"

// Std
#include <algorithm>

namespace Okteta {

ByteArrayUtf8StringValidator::ByteArrayUtf8StringValidator() = default;

ByteArrayUtf8StringValidator::~ByteArrayUtf8StringValidator() = default;

QValidator::State ByteArrayUtf8StringValidator::validate(QString& string, int& pos) const
{
    Q_UNUSED(pos)

    QValidator::State result = QValidator::Acceptable;

    int usedStringLength = 0;
    int resultSize = 0;
    const ByteArrayUtf8StringDecoder::CodeState evalResult = m_decoder.validate(&resultSize, string, minLength(), maxLength(), &usedStringLength);

    result =
        (evalResult == ByteArrayUtf8StringDecoder::CodeAcceptable) ?   QValidator::Acceptable :
        (evalResult == ByteArrayUtf8StringDecoder::CodeIntermediate) ? QValidator::Intermediate :
        /* else */                                                     QValidator::Invalid;

    if ((result != QValidator::Invalid)) {
        if (usedStringLength < string.size()) {
            result = QValidator::Invalid;
        } else if (resultSize < minLength()) {
            result = QValidator::Intermediate;
        }
    }

    return result;
}

QByteArray ByteArrayUtf8StringValidator::toByteArray(const QString& string) const
{
    QByteArray result;

    std::ignore = m_decoder.decode(&result, string, minLength(), maxLength());

    if (result.size() < minLength()) {
        result.append(QByteArray(minLength() - result.size(), '\0'));
    }

    return result;
}

QString ByteArrayUtf8StringValidator::toString(const QByteArray& byteArray) const
{
    const int byteArraySize = std::min(byteArray.size(), maxLength());

    QString result = m_encoder.encodeAsString(byteArray.constData(), byteArraySize, m_decoder.textCodec());

    if (byteArraySize < minLength()) {
        result += QStringLiteral("\\x00").repeated(minLength() - byteArraySize);
    }

    return result;
}

}
