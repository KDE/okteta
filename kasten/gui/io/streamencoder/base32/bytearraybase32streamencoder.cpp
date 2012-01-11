/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2010 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "bytearraybase32streamencoder.h"

// Okteta core
#include <abstractbytearraymodel.h>
// KDE
#include <KLocale>
// Qt
#include <QtCore/QTextStream>


namespace Kasten2
{

static const char base32ClassicEncodeMap[32] =
{
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
    'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
    'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
    'Y', 'Z', '2', '3', '4', '5', '6', '7'
};
static const char base32HexEncodeMap[32] =
{
    '0', '1', '2', '3', '4', '5', '6', '7',
    '8', '9', 'A', 'B', 'C', 'D', 'E', 'F',
    'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N',
    'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V'
};
static const char base32ZHexEncodeMap[32] =
{
    'y', 'b', 'n', 'd', 'r', 'f', 'g', '8',
    'e', 'j', 'k', 'm', 'c', 'p', 'q', 'x',
    'o', 't', '1', 'u', 'w', 'i', 's', 'z',
    'a', '3', '4', '5', 'h', '7', '6', '9'
};

static const char* const base32PaddingData[4] =
{
    "======",
    "====",
    "===",
    "="
};

static inline const char* base32Padding( ByteArrayBase32StreamEncoder::InputByteIndex index )
{
    return base32PaddingData[index - 1];
}
static inline const char* noPadding( ByteArrayBase32StreamEncoder::InputByteIndex index )
{
    Q_UNUSED(index);

    return "";
}

struct Base32EncodingData
{
    const char* const encodeMap;
    const char* (*padding)( ByteArrayBase32StreamEncoder::InputByteIndex );
};

static const Base32EncodingData
base32EncodingData[3] =
{
    {base32ClassicEncodeMap, &base32Padding},
    {base32HexEncodeMap, &base32Padding},
    {base32ZHexEncodeMap, &noPadding}
};


Base32StreamEncoderSettings::Base32StreamEncoderSettings()
 : algorithmId( ClassicId )
{}

ByteArrayBase32StreamEncoder::ByteArrayBase32StreamEncoder()
  : AbstractByteArrayStreamEncoder( i18nc("name of the encoding target","Base32"), QString::fromLatin1("text/x-base32") )
{}


bool ByteArrayBase32StreamEncoder::encodeDataToStream( QIODevice* device,
                                                       const ByteArrayView* byteArrayView,
                                                       const Okteta::AbstractByteArrayModel* byteArrayModel,
                                                       const Okteta::AddressRange& range )
{
    Q_UNUSED( byteArrayView );

    bool success = true;

    // encode
    QTextStream textStream( device );

    // prepare
    const char* const base32EncodeMap = base32EncodingData[mSettings.algorithmId].encodeMap;
    const char* (*base32Padding)( InputByteIndex ) = base32EncodingData[mSettings.algorithmId].padding;

    InputByteIndex inputByteIndex = FirstByte;
    int outputGroupsPerLine = 0;
    unsigned char bitsFromLastByte;

    for( Okteta::Address i=range.start(); i<=range.end(); ++i )
    {
        const Okteta::Byte byte = byteArrayModel->byte( i );

        switch( inputByteIndex )
        {
        case FirstByte:
            // bits 7..3
            textStream << base32EncodeMap[( byte >> 3 )];
            // bits 2..0 -> 4..2 for next
            bitsFromLastByte = (byte & 0x7) << 2;
            inputByteIndex = SecondByte;
            break;
        case SecondByte:
            // from last and bits 7..6 as 1..0 from this
            textStream << base32EncodeMap[( bitsFromLastByte | byte >> 6 )];
            // bits 5..1 as 4..0
            textStream << base32EncodeMap[( byte & 0x3E )>>1];
            // bits 0 -> 4 for next
            bitsFromLastByte = (byte & 0x1) << 4;
            inputByteIndex = ThirdByte;
            break;
        case ThirdByte:
            // from last and bits 7..4 as 3..0 from this
            textStream << base32EncodeMap[( bitsFromLastByte | byte >> 4 )];
            // bits 3..0 -> 4..1 for next
            bitsFromLastByte = (byte & 0xF) << 1;
            inputByteIndex = FourthByte;
            break;
        case FourthByte:
            // from last and bit 7 as 0 from this
            textStream << base32EncodeMap[( bitsFromLastByte | byte >> 7 )];
            // bits 6..2 as 4..0
            textStream << base32EncodeMap[( byte & 0x7C )>>2];
            // bits 1..0 -> 4..3 for next
            bitsFromLastByte = (byte & 0x3) << 3;
            inputByteIndex = FifthByte;
            break;
        case FifthByte:
            // from last and bits 7..5 as 2..0 from this
            textStream << base32EncodeMap[( bitsFromLastByte | byte >> 5 )];
            // bits 4..0
            textStream << base32EncodeMap[( byte & 0x1F )];
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
        textStream << base32EncodeMap[bitsFromLastByte]
                   << base32Padding(inputByteIndex);

    return success;
}

ByteArrayBase32StreamEncoder::~ByteArrayBase32StreamEncoder() {}

}
