/*
    SPDX-FileCopyrightText: 2024 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearrayutf8stringencoder.hpp"

// Qt
#include <QByteArray>
#include <QString>
#include <QTextCodec>
// Std
#include <algorithm>

namespace Okteta {

QLatin1String ByteArrayUtf8StringEncoder::encodeAsControlString(QChar character)
{
    const char* string;
    switch (character.unicode()) {
    case '\a': string = "\\a"; break;
    case '\b': string = "\\b"; break;
    case '\f': string = "\\f"; break;
    case '\n': string = "\\n"; break;
    case '\r': string = "\\r"; break;
    case '\t': string = "\\t"; break;
    case '\v': string = "\\v"; break;
    default: string = nullptr; break;
    }

    return QLatin1String(string);
}

QString ByteArrayUtf8StringEncoder::encodeAsString(const QByteArray& byteArray, QTextCodec* utf8Codec) const
{
    return encodeAsString(byteArray.constData(), byteArray.size(), utf8Codec);
}

QString ByteArrayUtf8StringEncoder::encodeAsString(const char* byteArrayData, std::size_t byteArraySize, QTextCodec* utf8Codec) const
{
    QString result;

    const QChar replacementChar(QChar::ReplacementCharacter);
    result.reserve(byteArraySize); // TODO: or prepare for worse-case and then shrink afterwards?
    for (std::size_t i = 0; i < byteArraySize;) {
        const std::size_t maxUtf8DataSize = std::min(byteArraySize - i, static_cast<std::size_t>(4));
        const char* const data = byteArrayData + i;
        std::size_t byteUsed = 0;
        QString utf8;
        for (std::size_t b = 1; b <= maxUtf8DataSize; ++b) {
            utf8 = utf8Codec->toUnicode(data, b);
            if (utf8.size() == 1 && utf8[0] != replacementChar) {
                byteUsed = b;
                break;
            }
        }

        if (byteUsed > 0) {
            const QChar character = utf8[0];
            if (character.isPrint()) {
                if (character == QLatin1Char('\\')) {
                    result.append(QLatin1String("\\\\"));
                } else {
                    result.append(character);
                }
                i += byteUsed;
                continue;
            }
            const QLatin1String controlString = encodeAsControlString(character);
            if (controlString.data() != nullptr) {
                result.append(controlString);
                i += byteUsed;
                continue;
            }
        } else {
            byteUsed = 1;
        }
        for (std::size_t b = 0; b < byteUsed; ++b) {
            // TODO: make hex vs. oct. configurable? \X vs. \x? support memory of user usages?
            const char byte = byteArrayData[i];
            const unsigned int numericValue = static_cast<unsigned char>(byte);
            result.append(QStringLiteral("\\x%1").arg(numericValue, 2, 16, QLatin1Char('0')));
            ++i;
        }
    }

    return result;
}

}
