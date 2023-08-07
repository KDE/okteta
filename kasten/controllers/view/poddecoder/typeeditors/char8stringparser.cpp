/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2023 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "char8stringparser.hpp"

// Okteta core
#include <Okteta/CharCodec>
#include <Okteta/Character>

namespace Okteta {

Char8StringParser::Char8StringParser(const CharCodec* charCodec)
    : m_charCodec(charCodec)
    , m_octalNumberMatcher(QStringLiteral("^(?:[0-3][0-7]{0,2}|[4-7][0-7]?)$"))
    , m_hexNumberMatcher(QStringLiteral("^[A-Fa-f0-9]{1,2}$"))
{
}

Char8StringParser::SyntaxState Char8StringParser::evaluate(QChar* result,
                                                           const QString& string) const
{
    const int stringLength = string.length();
    if (stringLength == 0) {
        return SyntaxIntermediate;
    }

    const QChar c0 = string.at(0);
    if (stringLength == 1) {
        if (m_charCodec->canEncode(c0)) {
            if (result) {
                *result = c0;
            }
            return SyntaxAcceptable;
        }
        // escape started?
        if (c0 == QLatin1Char('\\')) {
            return SyntaxIntermediate;
        }
    } else if (c0 == QLatin1Char('\\')) {
        const QChar c1 = string.at(1);
        if (stringLength == 2) {
            QChar escapedChar;
            if (c1 == QLatin1Char('n')) {
                escapedChar = QLatin1Char('\n'); // newline
            } else if (c1 == QLatin1Char('r')) {
                escapedChar = QLatin1Char('\r'); // carriage return
            } else if (c1 == QLatin1Char('t')) {
                escapedChar = QLatin1Char('\t'); // horizontal tab
            } else if (c1 == QLatin1Char('v')) {
                escapedChar = QLatin1Char('\v'); // vertical tab
            } else if (c1 == QLatin1Char('f')) {
                escapedChar = QLatin1Char('\f'); // form feed - new page
            } else if (c1 == QLatin1Char('b')) {
                escapedChar = QLatin1Char('\b'); // backspace
            } else if (c1 == QLatin1Char('a')) {
                escapedChar = QLatin1Char('\a'); // audible bell
            }
            if (!escapedChar.isNull() && m_charCodec->canEncode(escapedChar)) {
                if (result) {
                    *result = escapedChar;
                }
                return SyntaxAcceptable;
            }
        }
        if (stringLength < 5) {
            bool couldConvert = false;
            int value = 0;
            // hex escape
            if ((c1 == QLatin1Char('x')) || (c1 == QLatin1Char('X'))) {
                if (stringLength == 2) {
                    return SyntaxIntermediate;
                }
                const QStringRef hexValueString = string.midRef(2, 2); // 2 chars
                // toInt just ignores unparseable data at the end, so test outselves
                QRegularExpressionMatch match = m_hexNumberMatcher.match(hexValueString);
                if (match.hasMatch()) {
                    value = hexValueString.toInt(&couldConvert, 16);
                }
            }
            // octal escape:
            else {
                const QStringRef octalValueString = string.midRef(1, 3); // 3 chars
                // toInt just ignores unparseable data at the end, so test outselves
                QRegularExpressionMatch match = m_octalNumberMatcher.match(octalValueString);
                if (match.hasMatch()) {
                    value = octalValueString.toInt(&couldConvert, 8);
                }
            }
            if (couldConvert) {
                const Character character = m_charCodec->decode(static_cast<Byte>(value));
                if (!character.isUndefined()) {
                    if (result) {
                        *result = character;
                    }
                    return SyntaxAcceptable;
                }
            }
        }
    }

    return SyntaxInvalid;
}

}
