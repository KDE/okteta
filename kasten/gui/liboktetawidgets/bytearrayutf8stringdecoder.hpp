/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2024 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_BYTEARRAYUTF8STRINGDECODER_HPP
#define OKTETA_BYTEARRAYUTF8STRINGDECODER_HPP

class QByteArray;
class QChar;
class QString;
class QTextCodec;

namespace Okteta {

class ByteArrayUtf8StringDecoder
{
public:
    enum CodeState {
        CodeInvalid,
        CodeIntermediate,
        CodeAcceptable,
    };

public:
    ByteArrayUtf8StringDecoder();
    ~ByteArrayUtf8StringDecoder();

public:
    [[nodiscard]]
    CodeState decode(QByteArray* result, const QString& string,
                     int minResultSize = 0, int maxResultSize = -1, int* usedStringSize = nullptr) const;
    [[nodiscard]]
    CodeState validate(int* resultSize, const QString& string,
                       int minResultSize = 0, int maxResultSize = -1, int* usedStringSize = nullptr) const;

    [[nodiscard]]
    QTextCodec* textCodec() const;

private:
    [[nodiscard]]
    static int valueFromOctalDigit(QChar character);
    [[nodiscard]]
    static int valueFromHexDigit(QChar character);

    [[nodiscard]]
    CodeState doDecode(QByteArray* result, int* resultSize, const QString& string,
                       int minResultSize = 0, int maxResultSize = -1, int* usedStringSize = nullptr) const;

private:
    enum ParseState
    {
        ParseNextToRead,
        ParseAfterEscape,
    };

private:
    QTextCodec* const m_utf8Codec;
};

inline QTextCodec* ByteArrayUtf8StringDecoder::textCodec() const { return m_utf8Codec; }

}

#endif
