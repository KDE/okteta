/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2024 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_BYTEARRAYCHAR8STRINGENCODER_HPP
#define OKTETA_BYTEARRAYCHAR8STRINGENCODER_HPP

// Okteta core
#include <Okteta/Byte>

class QByteArray;
class QString;

namespace Okteta {

class CharCodec;
class Character;

class ByteArrayChar8StringEncoder
{
public:
    /// Beware, not compatible with other enums
    enum class ValueCoding
    {
        Hexadecimal = 0,
        Octal = 1,
    };

public:
    [[nodiscard]]
    QString encodeAsString(const char* byteArrayData, int byteArraySize, const CharCodec* charCodec) const;
    [[nodiscard]]
    QString encodeAsString(const QByteArray& byteArray, const CharCodec* charCodec) const;
    [[nodiscard]]
    QString encodeAsString(Byte byte, const CharCodec* charCodec,
                           ValueCoding escapeValueCoding) const;

private:
    [[nodiscard]]
    static QString encodeAsControlString(Character character);
};

}

#endif
