/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2024 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearraychar8stringencoder.hpp"

// Okteta core
#include <Okteta/Character>
#include <Okteta/CharCodec>
// Qt
#include <QByteArray>
#include <QString>

namespace Okteta {

QLatin1String ByteArrayChar8StringEncoder::encodeAsControlString(Character character)
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

QString ByteArrayChar8StringEncoder::encodeAsString(const QByteArray& byteArray, const CharCodec* charCodec) const
{
    return encodeAsString(byteArray.constData(), byteArray.size(), charCodec);
}

QString ByteArrayChar8StringEncoder::encodeAsString(const char* byteArrayData, int byteArraySize, const CharCodec* charCodec) const
{
    QString result;

    result.reserve(byteArraySize); // TODO: or prepare for worse-case and then shrink afterwards?
    for (int i = 0; i < byteArraySize; ++i) {
        const char byte = byteArrayData[i];
        const Character character = charCodec->decode(byte);
        if (!character.isUndefined()) {
            if (character.isPrint()) {
                if (character == QLatin1Char('\\')) {
                    result.append(QLatin1String("\\\\"));
                } else {
                    result.append(character);
                }
                continue;
            }
            const QLatin1String controlString = encodeAsControlString(character);
            if (controlString.data() != nullptr) {
                result.append(controlString);
                continue;
            }
        }
        // TODO: make hex vs. oct. configurable? \X vs. \x? support memory of user usages?
        const unsigned int numericValue = static_cast<unsigned char>(byte);
        result.append(QStringLiteral("\\x%1").arg(numericValue, 2, 16, QLatin1Char('0')));
    }

    return result;
}

}
