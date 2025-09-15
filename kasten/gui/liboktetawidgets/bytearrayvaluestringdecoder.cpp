/*
    SPDX-FileCopyrightText: 2025 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearrayvaluestringdecoder.hpp"

// Okteta core
#include <Okteta/ValueCodec>
// Qt
#include <QString>

namespace Okteta {

ByteArrayValueStringDecoder::ByteArrayValueStringDecoder(const ValueCodec* valueCodec)
    : m_valueCodec(valueCodec)
{
}

ByteArrayValueStringDecoder::~ByteArrayValueStringDecoder() = default;


// TODO: turn into some public Okteta Core decoder class
ByteArrayValueStringDecoder::CodeState ByteArrayValueStringDecoder::decode(QByteArray* result,
                                                                           const QString& string) const
{
    ByteArrayValueStringDecoder::CodeState syntaxResult = CodeAcceptable;

    const int stringSize = string.size();
    // prepare for maximal possible size:
    // one digit & one space per byte, n digits, n - 1 spaces, "+ 1" to round up
    result->reserve((stringSize + 1) / 2);
    // eefault to assume no separators, determine any after first value read
    bool isSpaceSeparated = false;
    bool isExpectingSpaceSeparator = false;
    int i = 0;
    while (i < stringSize) {
        if (string[i].isSpace()) {
            // handle any whitespace separation formatting
            if (isSpaceSeparated) {
                isExpectingSpaceSeparator = false;
                ++i;
                continue;
            }
            // accept any leading spaces
            const bool isBeforeFirstValue = result->isEmpty();
            if (Q_UNLIKELY(isBeforeFirstValue)) {
                ++i;
                continue;
            }
            const bool isBehindFirstValue = (result->size() == 1);
            if (Q_UNLIKELY(isBehindFirstValue)) {
                // detect separation pattern
                isSpaceSeparated = true;
                ++i;
                continue;
            }
            // accept if part of trailing spaces
            ++i;
            while (i < stringSize) {
                if (!string[i].isSpace()) {
                    break;
                }
                ++i;
            }
            if  (i < stringSize) {
                syntaxResult = CodeInvalid;
            }

            break;
        }

        if (!isExpectingSpaceSeparator) {
            // try read a value
            Okteta::Byte byte;
            const int readChars = m_valueCodec->decode(&byte, string, i);
            if (readChars > 0) {
                result->append(byte);
                i += readChars;
                if (isSpaceSeparated) {
                    isExpectingSpaceSeparator = true;
                }
                continue;
            }
        }

        syntaxResult = CodeInvalid;
        break;
    }

    result->squeeze();
    return syntaxResult;
}

}
