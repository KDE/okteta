/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2004, 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "decimalbytecodec.hpp"

// Qt
#include <QString>

namespace Okteta {

unsigned int DecimalByteCodec::encodingWidth() const { return 3; }
Byte DecimalByteCodec::digitsFilledLimit() const { return 26; }

void DecimalByteCodec::encode(QString* digits, unsigned int pos, Byte byte) const
{
    const int minSizeNeeded = pos + 3;
    if (digits->size() < minSizeNeeded) {
        digits->resize(minSizeNeeded);
    }

    unsigned char digitValue = byte / 100;
    (*digits)[pos++] = QLatin1Char('0' + digitValue);
    byte -= digitValue * 100;
    digitValue = byte / 10;
    (*digits)[pos++] = QLatin1Char('0' + digitValue);
    byte -= digitValue * 10;
    (*digits)[pos] = QLatin1Char('0' + byte);
}

void DecimalByteCodec::encodeShort(QString* digits, unsigned int pos, Byte byte) const
{
    const int encodingLength = (byte > 99) ? 3 : (byte > 9) ? 2 : 1;
    const int minSizeNeeded = pos + encodingLength;
    if (digits->size() < minSizeNeeded) {
        digits->resize(minSizeNeeded);
    }

    const unsigned char firstDigitValue = byte / 100;
    if (firstDigitValue > 0) {
        (*digits)[pos++] = QLatin1Char('0' + firstDigitValue);
        byte -= firstDigitValue * 100;
    }
    const unsigned char secondDigitValue = byte / 10;
    if (secondDigitValue > 0 || firstDigitValue > 0) {
        (*digits)[pos++] = QLatin1Char('0' + secondDigitValue);
        byte -= secondDigitValue * 10;
    }
    (*digits)[pos] = QLatin1Char('0' + byte);
}

bool DecimalByteCodec::isValidDigit(unsigned char digit) const
{
    return ('0' <= digit && digit <= '9');
}

bool DecimalByteCodec::turnToValue(unsigned char* digit) const
{
    if (isValidDigit(*digit)) {
        *digit -= '0';
        return true;
    }
    return false;
}

bool DecimalByteCodec::appendDigit(Byte* byte, unsigned char digit) const
{
    if (turnToValue(&digit)) {
        Byte _byte = *byte;
        if (_byte < 25 ||
            (_byte == 25 && digit < 6)) {
            _byte *= 10;
            _byte += digit;
            *byte = _byte;
            return true;
        }
    }
    return false;
}

void DecimalByteCodec::removeLastDigit(Byte* byte) const
{
    *byte /= 10;
}

}
