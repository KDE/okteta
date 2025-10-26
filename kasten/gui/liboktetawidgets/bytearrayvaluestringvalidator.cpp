/*
    SPDX-FileCopyrightText: 2024 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearrayvaluestringvalidator.hpp"

// Okteta core
#include <Okteta/ValueCodec>
// Std
#include <algorithm>

namespace Okteta {

ByteArrayValueStringValidator::ByteArrayValueStringValidator(Coding valueCodingId)
    : m_valueCodec(ValueCodec::createCodec(static_cast<Okteta::ValueCoding>(valueCodingId)))
    , m_codecId(valueCodingId)
{
}

ByteArrayValueStringValidator::~ByteArrayValueStringValidator() = default;

void ByteArrayValueStringValidator::setCoding(Coding codecId)
{
    if (codecId == m_codecId) {
        return;
    }

    m_codecId = codecId;

    m_valueCodec = ValueCodec::createCodec(static_cast<Okteta::ValueCoding>(m_codecId));
}

QValidator::State ByteArrayValueStringValidator::validate(QString& string, int& pos) const
{
    Q_UNUSED(pos)

    QValidator::State result = QValidator::Acceptable;

    const int stringLength = string.length();
    int byteCount = 0;
    int i = 0;
    while ((i < stringLength) && (byteCount < maxLength())) {
        Okteta::Byte dummyByte;
        const int usedCharCount = m_valueCodec->decode(&dummyByte, string, i);

        // could not decode?
        // TODO: tolerate whitespace
        if (usedCharCount == 0) {
            result = QValidator::Invalid;
            break;
        }
        i += usedCharCount;
        ++byteCount;
    }

    if ((result != QValidator::Invalid)) {
        if (i < stringLength) {
            result = QValidator::Invalid;
        } else if (byteCount < minLength()) {
            result = QValidator::Intermediate;
        }
    }

    return result;
}

QByteArray ByteArrayValueStringValidator::toByteArray(const QString& string) const
{
    QByteArray result;

    const int stringLength = string.length();
    int i = 0;
    while (i < stringLength) {
        Byte byte;
        const int readChars = m_valueCodec->decode(&byte, string, i);
        if (readChars > 0) {
            i += readChars;
            result.append(byte);
        } else {
            // TODO: tolerate whitespace
            // TODO: signal error
            break;
        }
        if (result.size() >= maxLength()) {
            break;
        }
    }

    if (result.size() < minLength()) {
        result.append(QByteArray(minLength() - result.size(), '\0'));
    }

    return result;
}

QString ByteArrayValueStringValidator::toString(const QByteArray& byteArray) const
{
    QString result;

    const int byteArraySize = std::min(byteArray.size(), maxLength());
    const int encodingWidth = m_valueCodec->encodingWidth();
    result.resize(byteArraySize * encodingWidth);
    int r = 0;
    for (int i = 0; i < byteArraySize; ++i, r += encodingWidth) {
        m_valueCodec->encode(&result, r, byteArray[i]);
    }

    if (byteArraySize < minLength()) {
        const int paddingCount = (minLength() - byteArraySize) * encodingWidth;
        result += QString(paddingCount, QLatin1Char('0'));
    }

    return result;
}

}
