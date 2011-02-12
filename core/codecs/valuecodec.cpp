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

#include "valuecodec.h"

// lib
#include "binarybytecodec.h"
#include "octalbytecodec.h"
#include "decimalbytecodec.h"
#include "hexadecimalbytecodec.h"
// Qt
#include <QtCore/QString>


namespace Okteta {

ValueCodec *ValueCodec::createCodec( ValueCoding valueCoding )
{
    ValueCodec *result;
    switch( valueCoding )
    {
        case DecimalCoding: result = new DecimalByteCodec(); break;
        case OctalCoding:   result = new OctalByteCodec();   break;
        case BinaryCoding:  result = new BinaryByteCodec();  break;
        case HexadecimalCoding:
        default:            result = new HexadecimalByteCodec();
    }
    return result;
}

unsigned int ValueCodec::decode( unsigned char* byte, const QString& digits, unsigned int pos ) const
{
    const unsigned int oldPos = pos;
    const unsigned int left = digits.size() - pos;

    unsigned int d = encodingWidth();
    if( left < d )
        d = left;

    unsigned char result = 0;
    while( d > 0 )
    {
        if( !appendDigit(&result,digits.at(pos).toLatin1()) ) // TODO: use QChar.digitValue()
            break;

        ++pos;
        --d;
    }

    *byte = result;
    return pos - oldPos;
}

ValueCodec::~ValueCodec() {}

}
