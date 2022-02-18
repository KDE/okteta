/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2022 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "ibm874charcodec.hpp"

// lib
#include <character.hpp>
// Qt
#include <QString>

namespace Okteta {

bool IBM874CharCodec::encode(Byte* byte, const QChar& _char) const
{
    const ushort unicodeValue = _char.unicode();

    if (unicodeValue <= 0x007f) {
        *byte = unicodeValue;
        return true;
    }

    constexpr unsigned char unmatched = 0xFF; // value not in target set
    const unsigned char _byte =
        (0x0E01 <= unicodeValue) && (unicodeValue <= 0x0E3A) ? (unicodeValue - 0x0E01 + 0xA1) :
        (0x0E3F <= unicodeValue) && (unicodeValue <= 0x0E5B) ? (unicodeValue - 0x0E3F + 0xDF) :
        (unicodeValue == 0x20AC) ?                              0x80 :
        (unicodeValue == 0x00A0) ?                              0xA0 :
        /* else */                                              unmatched;

    // not covered?
    if (_byte == unmatched) {
        return false;
    }

    *byte = _byte;

    return true;
}

Character IBM874CharCodec::decode(Byte byte) const
{
    const ushort unicode =
        (byte <= 0x7f)
            ?  ushort(byte) :
        (0x80 == byte)
            ?  ushort(0x20AC) :
        (0xA0 == byte)
            ?  ushort(0x00A0) :
        (0xA1 <= byte && byte <= 0xDA)
            ? ushort(0x0E01 - 0xA1 + byte) :
        (0xDF <= byte && byte <= 0xFB)
            ? ushort(0x0E3F - 0xDF + byte) :
        /* else */ 0xFFFD;

    if (unicode == 0xFFFD) {
        return {QChar(0), true};
    }

    return {QChar(unicode)};
}

bool IBM874CharCodec::canEncode(const QChar& _char) const
{
    const ushort charUnicode = _char.unicode();
    return
        (charUnicode <= 0x007f) ||
        (0x0E01 <= charUnicode && charUnicode <= 0x0E3A) ||
        (0x0E3F <= charUnicode && charUnicode <= 0x0E5B) ||
        (charUnicode == 0x20AC) || (charUnicode == 0x00A0);
}

const QString& IBM874CharCodec::name() const
{
    return codecName();
}

const QString& IBM874CharCodec::codecName()
{
    static const QString name = QStringLiteral("IBM874");
    return name;
}

}
