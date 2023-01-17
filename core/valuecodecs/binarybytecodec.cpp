/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2004, 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "binarybytecodec.hpp"

// Qt
#include <QString>

namespace Okteta {
static constexpr Byte binaryDigitsFilledLimit = 128;

unsigned int BinaryByteCodec::encodingWidth() const { return 8; }
Byte BinaryByteCodec::digitsFilledLimit() const { return binaryDigitsFilledLimit; }

void BinaryByteCodec::encode(QString* digits, unsigned int pos, Byte byte) const
{
    const int minSizeNeeded = pos + 8;
    if (digits->size() < minSizeNeeded) {
        digits->resize(minSizeNeeded);
    }

    for (Byte mask = 1 << 7; mask > 0; mask >>= 1) {
        (*digits)[pos++] = QLatin1Char((byte & mask) ? '1' : '0');
    }
}

void BinaryByteCodec::encodeShort(QString* digits, unsigned int pos, Byte byte) const
{
    int encodingLength = 8;
    Byte mask = 1 << 7;
    // find first set bit, at last break on LSB
    for (; mask > 1; mask >>= 1, --encodingLength) {
        if (byte & mask) {
            break;
        }
    }

    const int minSizeNeeded = pos + encodingLength;
    if (digits->size() < minSizeNeeded) {
        digits->resize(minSizeNeeded);
    }

    // now set the
    for (; mask > 0; mask >>= 1) {
        (*digits)[pos++] = QLatin1Char((byte & mask) ? '1' : '0');
    }
}

bool BinaryByteCodec::isValidDigit(unsigned char digit) const
{
    return digit == '0' || digit == '1';
}

bool BinaryByteCodec::turnToValue(unsigned char* digit) const
{
    if (isValidDigit(*digit)) {
        *digit -= '0';
        return true;
    }
    return false;
}

bool BinaryByteCodec::appendDigit(Byte* byte, unsigned char digit) const
{
    if (turnToValue(&digit)) {
        Byte _byte = *byte;
        if (_byte < binaryDigitsFilledLimit) {
            _byte <<= 1;
            _byte += digit;
            *byte = _byte;
            return true;
        }
    }
    return false;
}

void BinaryByteCodec::removeLastDigit(Byte* byte) const
{
    *byte >>= 1;
}

}
