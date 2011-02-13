/*
    This file is part of the Okteta Core library, made within the KDE community.

    Copyright 2004,2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library. If not, see <http://www.gnu.org/licenses/>.
*/

#include "decimalbytecodec.h"

// Qt
#include <QtCore/QString>


namespace Okteta
{

unsigned int DecimalByteCodec::encodingWidth() const { return 3; }
Byte DecimalByteCodec::digitsFilledLimit() const { return 26; }

void DecimalByteCodec::encode( QString& digits, unsigned int pos, Byte byte ) const
{
    unsigned char digitValue = byte / 100;
    digits[pos++] = QLatin1Char( '0'+digitValue );
    byte -= digitValue * 100;
    digitValue = byte / 10;
    digits[pos++] = QLatin1Char( '0'+digitValue );
    byte -= digitValue * 10;
    digits[pos] = QLatin1Char( '0'+byte );
}


void DecimalByteCodec::encodeShort( QString& digits, unsigned int pos, Byte byte ) const
{
    const unsigned char firstDigitValue = byte / 100;
    if( firstDigitValue > 0 )
    {
        digits[pos++] = QLatin1Char( '0'+firstDigitValue );
        byte -= firstDigitValue * 100;
    }
    const unsigned char secondDigitValue = byte / 10;
    if( secondDigitValue > 0 || firstDigitValue > 0 )
    {
        digits[pos++] = QLatin1Char( '0'+secondDigitValue );
        byte -= secondDigitValue * 10;
    }
    digits[pos] = QLatin1Char( '0'+byte );
}


bool DecimalByteCodec::isValidDigit( unsigned char digit ) const
{
    return ('0' <= digit && digit <= '9');
}

bool DecimalByteCodec::turnToValue( unsigned char* digit ) const
{
    if( isValidDigit(*digit) )
    {
        *digit -= '0';
        return true;
    }
    return false;
}


bool DecimalByteCodec::appendDigit( Byte* byte, unsigned char digit ) const
{
    if( turnToValue(&digit) )
    {
        Byte _byte = *byte;
        if( _byte < 25 ||
            (_byte == 25 && digit < 6) )
        {
            _byte *= 10;
            _byte += digit;
            *byte = _byte;
            return true;
        }
    }
    return false;
}

void DecimalByteCodec::removeLastDigit( Byte* byte ) const
{
    *byte /= 10;
}

}
