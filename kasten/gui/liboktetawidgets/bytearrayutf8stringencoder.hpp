/*
    SPDX-FileCopyrightText: 2024 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_BYTEARRAYUTF8STRINGENCODER_HPP
#define OKTETA_BYTEARRAYUTF8STRINGENCODER_HPP

class QByteArray;
class QString;
class QLatin1String;
class QTextCodec;
class QChar;

namespace Okteta {

class ByteArrayUtf8StringEncoder
{
public:
    [[nodiscard]]
    QString encodeAsString(const char* byteArrayData, int byteArraySize, QTextCodec* utf8Codec) const;
    [[nodiscard]]
    QString encodeAsString(const QByteArray& byteArray, QTextCodec* utf8Codec) const;

private:
    [[nodiscard]]
    static QLatin1String encodeAsControlString(QChar character);
};

}

#endif
