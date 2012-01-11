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

#include "bytearrayxxencodingstreamencoder.h"

// Okteta core
#include <abstractbytearraymodel.h>
// KDE
#include <KLocale>
// Qt
#include <QtCore/QTextStream>


namespace Kasten2
{

static const char xxencodeMap[64] =
{
    '+', '-', '0', '1', '2', '3', '4', '5',
    '6', '7', '8', '9', 'A', 'B', 'C', 'D',
    'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L',
    'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
    'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b',
    'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
    'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r',
    's', 't', 'u', 'v', 'w', 'x', 'y', 'z'
};

static const int defaultxxInputLineLength = 45;
static const int xxInputLineLength = defaultxxInputLineLength;
static const int xxInputGroupLength = 3;
static const int maxXxInputGroupsPerLine = xxInputLineLength/xxInputGroupLength;

static inline char xxmapByte( char byte )           { return xxencodeMap[(int)byte]; }

static inline const char* xxpadding( ByteArrayXxencodingStreamEncoder::InputByteIndex index )
{
    const char* const paddingData[2] = {"++","+"};

    return paddingData[index - 1];
}


XxencodingStreamEncoderSettings::XxencodingStreamEncoderSettings()
 : fileName( QString::fromLatin1("okteta-export"))
{}

ByteArrayXxencodingStreamEncoder::ByteArrayXxencodingStreamEncoder()
  : AbstractByteArrayStreamEncoder( i18nc("name of the encoding target","Xxencoding"), QString::fromLatin1("text/x-xxencode") )
{}

// TODO: make this algorithm shared with ByteArrayUuencodingStreamEncoder again
bool ByteArrayXxencodingStreamEncoder::encodeDataToStream( QIODevice* device,
                                                           const ByteArrayView* byteArrayView,
                                                           const Okteta::AbstractByteArrayModel* byteArrayModel,
                                                           const Okteta::AddressRange& range )
{
    Q_UNUSED( byteArrayView );

    const char header[] = "begin";
    const char footer[] = "\n+\nend\n";

    bool success = true;

    // encode
    QTextStream textStream( device );

    // prepare
    InputByteIndex inputByteIndex = FirstByte;
    int inputGroupsPerLine = 0;
    unsigned char bitsFromLastByte;

    // header
    textStream << header << " 644 " << mSettings.fileName.toAscii();

    const int firstLineLength = qMin( range.width(), xxInputLineLength );
    if( firstLineLength > 0 )
    {
        textStream << '\n';
        textStream << xxmapByte( firstLineLength );
    }

    for( Okteta::Address i=range.start(); i<=range.end(); ++i )
    {
        const Okteta::Byte byte = byteArrayModel->byte( i );

        switch( inputByteIndex )
        {
        case FirstByte:
            // bits 7..2
            textStream << xxmapByte( byte >> 2 );
            // bits 1..0 -> 5..4 for next
            bitsFromLastByte = (byte & 0x3) << 4;
            inputByteIndex = SecondByte;
            break;
        case SecondByte:
            // from last and bits 7..4 as 3..0 from this
            textStream << xxmapByte( bitsFromLastByte | byte >> 4 );
            // bits 3..0 -> 5..2 for next
            bitsFromLastByte = (byte & 0xf) << 2;
            inputByteIndex = ThirdByte;
            break;
        case ThirdByte:
            // from last and bits 7..6 as 1..0 from this
            textStream << xxmapByte( bitsFromLastByte | byte >> 6 );
            // bits 5..0
            textStream << xxmapByte( byte & 0x3F );
            inputByteIndex = FirstByte;
            ++inputGroupsPerLine;
            if( inputGroupsPerLine >= maxXxInputGroupsPerLine && i<range.end() )
            {
                const int remainsCount = range.end() - i;
                const int nextLineLength = qMin( remainsCount, xxInputLineLength );
                textStream << '\n';
                textStream << xxmapByte( nextLineLength );
                inputGroupsPerLine = 0;
            }
            break;
        }
    }
    const bool hasBitsLeft = ( inputByteIndex != FirstByte );
    if( hasBitsLeft )
        textStream << xxmapByte(bitsFromLastByte)
                   << xxpadding(inputByteIndex);
    // footer
    textStream << footer;

    return success;
}

ByteArrayXxencodingStreamEncoder::~ByteArrayXxencodingStreamEncoder() {}

}
