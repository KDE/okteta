/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2024 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_BYTEARRAYCHAR8STRINGDECODER_HPP
#define OKTETA_BYTEARRAYCHAR8STRINGDECODER_HPP

// Std
#include <memory>

class QByteArray;
class QChar;
class QString;

namespace Okteta {

class CharCodec;

class ByteArrayChar8StringDecoder
{
public:
    enum CodeState {
        CodeInvalid,
        CodeIntermediate,
        CodeAcceptable,
    };

public:
    ByteArrayChar8StringDecoder();
    ByteArrayChar8StringDecoder(const ByteArrayChar8StringDecoder&) = delete;
    ByteArrayChar8StringDecoder(ByteArrayChar8StringDecoder&&) = delete;

    ~ByteArrayChar8StringDecoder();

    ByteArrayChar8StringDecoder& operator=(const ByteArrayChar8StringDecoder&) = delete;
    ByteArrayChar8StringDecoder& operator=(ByteArrayChar8StringDecoder&&) = delete;

public:
    [[nodiscard]]
    CodeState decode(QByteArray* result, const QString& string,
                     int minResultSize = 0, int maxResultSize = -1, int* usedStringSize = nullptr) const;
    [[nodiscard]]
    CodeState validate(int* resultSize, const QString& string,
                       int minResultSize = 0, int maxResultSize = -1, int* usedStringSize = nullptr) const;

    /// Sets the char codec to use. Does not change the current codec.
    void setCharCodec(const QString& charCodecName);

    [[nodiscard]]
    const CharCodec* charCodec() const;

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
    std::unique_ptr<const CharCodec> m_charCodec;
};

inline const CharCodec* ByteArrayChar8StringDecoder::charCodec() const { return m_charCodec.get(); }

}

#endif
