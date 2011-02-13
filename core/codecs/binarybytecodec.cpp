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

#include "binarybytecodec.h"

// Qt
#include <QtCore/QString>


namespace Okteta
{
static const Byte binaryDigitsFilledLimit = 128;

unsigned int BinaryByteCodec::encodingWidth() const { return 8; }
Byte BinaryByteCodec::digitsFilledLimit() const { return binaryDigitsFilledLimit; }

void BinaryByteCodec::encode( QString& digits, unsigned int pos, Byte byte ) const
{
    for( Byte mask = 1<<7; mask > 0; mask >>= 1 )
        digits[pos++] = QLatin1Char( (byte & mask) ? '1' : '0' );
}

void BinaryByteCodec::encodeShort( QString& digits, unsigned int pos, Byte byte ) const
{
    Byte mask = 1<<7;
    // find first set bit, at last break on LSB
    for( ; mask > 1; mask >>= 1 )
        if( byte & mask )
            break;
    // now set the
    for( ; mask > 0; mask >>= 1 )
        digits[pos++] = QLatin1Char( (byte & mask) ? '1' : '0' );
}


bool BinaryByteCodec::isValidDigit( unsigned char digit ) const
{
    return digit == '0' || digit == '1';
}


bool BinaryByteCodec::turnToValue( unsigned char* digit ) const
{
    if( isValidDigit(*digit) )
    {
        *digit -= '0';
        return true;
    }
    return false;
}


bool BinaryByteCodec::appendDigit( Byte* byte, unsigned char digit ) const
{
    if( turnToValue(&digit) )
    {
        Byte _byte = *byte;
        if( _byte < binaryDigitsFilledLimit )
        {
            _byte <<= 1;
            _byte += digit;
            *byte = _byte;
            return true;
        }
    }
    return false;
}


void BinaryByteCodec::removeLastDigit( Byte* byte ) const
{
    *byte >>= 1;
}

}
