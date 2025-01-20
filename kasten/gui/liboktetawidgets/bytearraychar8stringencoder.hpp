/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2024 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_BYTEARRAYCHAR8STRINGENCODER_HPP
#define OKTETA_BYTEARRAYCHAR8STRINGENCODER_HPP

class QByteArray;
class QString;
class QLatin1String;

namespace Okteta {

class CharCodec;
class Character;

class ByteArrayChar8StringEncoder
{
public:
    [[nodiscard]]
    QString encodeAsString(const char* byteArrayData, int byteArraySize, const CharCodec* charCodec) const;
    [[nodiscard]]
    QString encodeAsString(const QByteArray& byteArray, const CharCodec* charCodec) const;

private:
    [[nodiscard]]
    static QLatin1String encodeAsControlString(Character character);
};

}

#endif
