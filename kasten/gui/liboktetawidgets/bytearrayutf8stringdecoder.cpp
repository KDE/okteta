/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2024 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearrayutf8stringdecoder.hpp"

// Qt
#include <QTextCodec>
// Std
#include <limits>

namespace Okteta {

ByteArrayUtf8StringDecoder::ByteArrayUtf8StringDecoder()
    : m_utf8Codec(QTextCodec::codecForName("UTF-8"))

{
}

ByteArrayUtf8StringDecoder::~ByteArrayUtf8StringDecoder() = default;

int ByteArrayUtf8StringDecoder::valueFromOctalDigit(QChar character)
{
    return
        ((QLatin1Char('0') <= character) && (character <= QLatin1Char('7'))) ?
            character.unicode() - '0' :
            -1;
}

int ByteArrayUtf8StringDecoder::valueFromHexDigit(QChar character)
{
    return
        ((QLatin1Char('0') <= character) && (character <= QLatin1Char('9'))) ?
            character.unicode() - '0' :
        ((QLatin1Char('A') <= character) && (character <= QLatin1Char('F'))) ?
            character.unicode() - 'A' + 10 :
        ((QLatin1Char('a') <= character) && (character <= QLatin1Char('f'))) ?
            character.unicode() - 'a' + 10 :
            -1;
}

ByteArrayUtf8StringDecoder::CodeState ByteArrayUtf8StringDecoder::decode(QByteArray* result,
                                                                         const QString& string,
                                                                         int minResultSize,
                                                                         int maxResultSize,
                                                                         int* usedStringSize) const
{
    return doDecode(result, nullptr, string, minResultSize, maxResultSize, usedStringSize);
}

ByteArrayUtf8StringDecoder::CodeState ByteArrayUtf8StringDecoder::validate(int* resultSize,
                                                                           const QString& string,
                                                                           int minResultSize,
                                                                           int maxResultSize,
                                                                           int* usedStringSize) const
{
    return doDecode(nullptr, resultSize, string, minResultSize, maxResultSize, usedStringSize);
}

ByteArrayUtf8StringDecoder::CodeState ByteArrayUtf8StringDecoder::doDecode(QByteArray* result,
                                                                           int* resultSizeArg,
                                                                           const QString& string,
                                                                           int minResultSize,
                                                                           int maxResultSize,
                                                                           int* usedStringSize) const
{
    int pos = 0;
    int resultSize = 0;
    const int stringSize = string.size();
    ParseState parseState = ParseNextToRead;
    if (maxResultSize < 0) {
        maxResultSize = std::numeric_limits<int>::max();
    }

    ByteArrayUtf8StringDecoder::CodeState syntaxResult = (string.isEmpty() && (minResultSize > 0)) ? CodeIntermediate : CodeAcceptable;

    while ((resultSize < maxResultSize) && (pos < stringSize) && (syntaxResult != CodeInvalid)) {
        const QChar character = string.at(pos);

        switch (parseState) {
        case ParseNextToRead: {
            // encoded byte started by escaoe?
            if (character == QLatin1Char('\\')) {
                ++pos;
                // look ahead, still input to process?
                if (pos < stringSize)  {
                    parseState = ParseAfterEscape;
                } else {
                    // default to 0 for assumed later escaped input
                    if (result) {
                        result->append('\0');
                    }
                    ++resultSize;
                    syntaxResult = CodeIntermediate;
                }
                break;
            }
            if (m_utf8Codec->canEncode(character)) {
                // TODO: research if there are multi-character expressions which need special handling
                const QByteArray bytes = m_utf8Codec->fromUnicode(&character, 1);
                if (resultSize + bytes.size() <= maxResultSize) {
                    if (result) {
                        result->append(bytes);
                    }
                    resultSize += bytes.size();
                    ++pos;
                    parseState = ParseNextToRead;
                } else {
                    syntaxResult = CodeInvalid;
                }
            } else {
                syntaxResult = CodeInvalid;
            }
            break;
        }
        case ParseAfterEscape: {
            // first try escaped control character
            QChar escapedChar;
            if (character == QLatin1Char('\\')) {
                escapedChar = QLatin1Char('\\'); // backslash
            } else if (character == QLatin1Char('n')) {
                escapedChar = QLatin1Char('\n'); // newline
            } else if (character == QLatin1Char('r')) {
                escapedChar = QLatin1Char('\r'); // carriage return
            } else if (character == QLatin1Char('t')) {
                escapedChar = QLatin1Char('\t'); // horizontal tab
            } else if (character == QLatin1Char('v')) {
                escapedChar = QLatin1Char('\v'); // vertical tab
            } else if (character == QLatin1Char('f')) {
                escapedChar = QLatin1Char('\f'); // form feed - new page
            } else if (character == QLatin1Char('b')) {
                escapedChar = QLatin1Char('\b'); // backspace
            } else if (character == QLatin1Char('a')) {
                escapedChar = QLatin1Char('\a'); // audible bell
            }
            if (!escapedChar.isNull()) {
                const QByteArray bytes = m_utf8Codec->fromUnicode(&escapedChar, 1);
                if (resultSize + bytes.size() <= maxResultSize) {
                    if (result) {
                        result->append(bytes);
                    }
                    resultSize += bytes.size();
                    ++pos;
                    parseState = ParseNextToRead;
                } else {
                    syntaxResult = CodeInvalid;
                }
                break;
            }

            // next try value
            // hex escape
            if ((character == QLatin1Char('x')) || (character == QLatin1Char('X'))) {
                // try to process next 2 positions as digit values
                // if incomplete, still use lower digit or value 0 with none
                constexpr int maxDigitSize = 2;
                const int potentialDigitSize = qMin(maxDigitSize, stringSize - pos - 1);
                const int valuePos = pos + 1;
                int value = 0;
                int digitPos = 0;
                for (; digitPos < potentialDigitSize; ++digitPos) {
                    const QChar digitCharacter = string.at(valuePos + digitPos);
                    const int digitValue = valueFromHexDigit(digitCharacter);
                    if (digitValue == -1) {
                        break;
                    }
                    value = value * 16 + digitValue;
                }
                if (result) {
                    result->append(static_cast<char>(value));
                }
                ++resultSize;
                pos = valuePos + digitPos;
                if (digitPos < maxDigitSize) {
                    syntaxResult = CodeIntermediate;
                }

                parseState = ParseNextToRead;
                break;
            }

            // last assume octal escape:
            // try to process this and next 1-2 positions as digit values
            // if incomplete, still use lower digits or value 0 with none
            int maxDigitSize = 3;
            const int valuePos = pos;
            int value = 0;
            int digitPos = 0;
            // look at first and check if to expect 2 or 3 digits
            const int digitValue = valueFromOctalDigit(character);
            if (digitValue != -1) {
                value = digitValue;
                if (digitValue > 3) {
                    maxDigitSize = 2;
                }
                const int potentialDigitSize = qMin(maxDigitSize, stringSize - pos);
                digitPos = 1;
                for (; digitPos < potentialDigitSize; ++digitPos) {
                    const QChar digitCharacter = string.at(valuePos + digitPos);
                    const int digitValue = valueFromOctalDigit(digitCharacter);
                    if (digitValue == -1) {
                        break;
                    }
                    value = value * 8 + digitValue;
                }
            }
            // default to 0 for assumed following escaped input
            // digitPos == 0 -> same position will be retried by normal read on next loop
            if (result) {
                result->append(static_cast<char>(value));
            }
            ++resultSize;
            pos = valuePos + digitPos;
            if (digitPos < maxDigitSize) {
                syntaxResult = CodeIntermediate;
            }

            parseState = ParseNextToRead;
            break;
        }
    }
    }

    if (resultSizeArg) {
        *resultSizeArg = resultSize;
    }

    if (usedStringSize) {
        *usedStringSize = pos;
    }

    return syntaxResult;
}

}
