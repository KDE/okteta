/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2004, 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "octalbytecodec.hpp"

// Qt
#include <QString>

namespace Okteta {
static constexpr Byte octalDigitsFilledLimit = 32;

unsigned int OctalByteCodec::encodingWidth() const { return 3; }
Byte OctalByteCodec::digitsFilledLimit() const { return octalDigitsFilledLimit; }

void OctalByteCodec::encode(QString* digits, unsigned int pos, Byte byte) const
{
    const int minSizeNeeded = pos + 3;
    if (digits->size() < minSizeNeeded) {
        digits->resize(minSizeNeeded);
    }

    (*digits)[pos++] = QLatin1Char('0' + (byte >> 6));
    (*digits)[pos++] = QLatin1Char('0' + ((byte >> 3) & 0x07));
    (*digits)[pos] =   QLatin1Char('0' + ((byte)      & 0x07));
}

void OctalByteCodec::encodeShort(QString* digits, unsigned int pos, Byte byte) const
{
    const int encodingLength = (byte > 077) ? 3 : (byte > 07) ? 2 : 1;
    const int minSizeNeeded = pos + encodingLength;
    if (digits->size() < minSizeNeeded) {
        digits->resize(minSizeNeeded);
    }

    const unsigned char firstDigitValue = byte >> 6;
    if (firstDigitValue > 0) {
        (*digits)[pos++] = QLatin1Char('0' + firstDigitValue);
    }
    const unsigned char secondDigitValue = (byte >> 3) & 0x07;
    if (secondDigitValue > 0 || firstDigitValue > 0) {
        (*digits)[pos++] = QLatin1Char('0' + secondDigitValue);
    }
    const unsigned char lastDigitValue = byte & 0x07;
    (*digits)[pos] = QLatin1Char('0' + lastDigitValue);
}

bool OctalByteCodec::isValidDigit(unsigned char digit) const
{
    return ('0' <= digit && digit <= '7');
}

bool OctalByteCodec::turnToValue(unsigned char* digit) const
{
    if (isValidDigit(*digit)) {
        *digit -= '0';
        return true;
    }
    return false;
}

bool OctalByteCodec::appendDigit(Byte* byte, unsigned char digit) const
{
    if (turnToValue(&digit)) {
        Byte _byte = *byte;
        if (_byte < octalDigitsFilledLimit) {
            _byte <<= 3;
            _byte += digit;
            *byte = _byte;
            return true;
        }
    }
    return false;
}

void OctalByteCodec::removeLastDigit(Byte* byte) const
{
    *byte >>= 3;
}

}
