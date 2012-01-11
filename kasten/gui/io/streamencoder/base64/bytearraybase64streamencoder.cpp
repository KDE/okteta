/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2009-2010 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "bytearraybase64streamencoder.h"

// Okteta core
#include <abstractbytearraymodel.h>
// KDE
#include <KLocale>
// Qt
#include <QtCore/QTextStream>


namespace Kasten2
{

const char base64EncodeMap[64] =
{
  'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
  'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
  'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
  'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
  'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
  'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
  'w', 'x', 'y', 'z', '0', '1', '2', '3',
  '4', '5', '6', '7', '8', '9', '+', '/'
};

static const char* const base64PaddingData[2] =
{
    "==",
    "="
};
static inline const char* base64Padding( ByteArrayBase64StreamEncoder::InputByteIndex index )
{
    return base64PaddingData[(int)(index) - 1];
}


ByteArrayBase64StreamEncoder::ByteArrayBase64StreamEncoder()
  : AbstractByteArrayStreamEncoder( i18nc("name of the encoding target","Base64"), QString::fromLatin1("application/base64") )
{}


bool ByteArrayBase64StreamEncoder::encodeDataToStream( QIODevice* device,
                                                       const ByteArrayView* byteArrayView,
                                                       const Okteta::AbstractByteArrayModel* byteArrayModel,
                                                       const Okteta::AddressRange& range )
{
    Q_UNUSED( byteArrayView );

    bool success = true;

    // encode
    QTextStream textStream( device );

    // prepare
    InputByteIndex inputByteIndex = FirstByte;
    int outputGroupsPerLine = 0;
    unsigned char bitsFromLastByte;

    for( Okteta::Address i=range.start(); i<=range.end(); ++i )
    {
        const Okteta::Byte byte = byteArrayModel->byte( i );

        switch( inputByteIndex )
        {
        case FirstByte:
            // bits 7..2
            textStream << base64EncodeMap[( byte >> 2 )];
            // bits 1..0 -> 5..4 for next
            bitsFromLastByte = (byte & 0x3) << 4;
            inputByteIndex = SecondByte;
            break;
        case SecondByte:
            // from last and bits 7..4 as 3..0 from this
            textStream << base64EncodeMap[( bitsFromLastByte | byte >> 4 )];
            // bits 3..0 -> 5..2 for next
            bitsFromLastByte = (byte & 0xf) << 2;
            inputByteIndex = ThirdByte;
            break;
        case ThirdByte:
            // from last and bits 7..6 as 1..0 from this
            textStream << base64EncodeMap[( bitsFromLastByte | byte >> 6 )];
            // bits 5..0
            textStream << base64EncodeMap[( byte & 0x3F )];
            inputByteIndex = FirstByte;
            ++outputGroupsPerLine;
            if( outputGroupsPerLine >= maxOutputGroupsPerLine && i<range.end() )
            {
                textStream << "\r\n";
                outputGroupsPerLine = 0;
            }
            break;
        }
    }
    const bool hasBitsLeft = ( inputByteIndex != FirstByte );
    if( hasBitsLeft )
        textStream << base64EncodeMap[bitsFromLastByte]
                   << base64Padding(inputByteIndex);

    return success;
}

ByteArrayBase64StreamEncoder::~ByteArrayBase64StreamEncoder() {}

}
