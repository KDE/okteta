/*
    This file is part of the Okteta Kasten module, part of the KDE project.

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

#include "bytearrayuuencodingstreamencoder.h"

// lib
#include "bytearraybase64streamencoder.h"
// Okteta core
#include <abstractbytearraymodel.h>
// KDE
#include <KLocale>
// Qt
#include <QtCore/QTextStream>


namespace Kasten
{

static const int defaultInputLineLength = 45;
static const int inputLineLength = defaultInputLineLength;
static const int inputGroupLength = 3;
static const int maxInputGroupsPerLine = inputLineLength/inputGroupLength;

static const char zeroSubstituteChar = '`';

enum InputByteIndex { FirstByte, SecondByte, ThirdByte };

static inline char uumapByteClassic( char byte ) { return (byte > 0) ? (byte + 32) : zeroSubstituteChar; }
static inline char uumapByteBase64( char byte )  { return base64EncodeMap[(int)byte]; }

struct UumapEncodeData
{
    char (*mapByte)( char );
    const char* header;
    const char* footer;
    const char* twoBytePad;
    const char* oneBytePad;
    bool hasLength;
};

static const UumapEncodeData classicUumapEncodeData =
{
    &uumapByteClassic,
    "begin",
    "\n`\nend\n",
    "``","`",
    true
};

static const UumapEncodeData base64UumapEncodeData =
{
    &uumapByteBase64,
    "begin-base64",
    "\n====\n",
    "==","=",
    false
};


ByteArrayUuEncodingStreamEncoder::ByteArrayUuEncodingStreamEncoder()
  : AbstractByteArrayStreamEncoder( i18nc("name of the encoding target","UU Encoding"), QString::fromLatin1("text/plain") )
{}


bool ByteArrayUuEncodingStreamEncoder::encodeDataToStream( QIODevice* device,
                                                           const ByteArrayView* byteArrayView,
                                                           const Okteta::AbstractByteArrayModel* byteArrayModel,
                                                           const Okteta::AddressRange& range )
{
    Q_UNUSED( byteArrayView );

    bool success = true;

    // encode
    QTextStream textStream( device );

    // prepare
    static const char fileName[] = "okteta-export";
    InputByteIndex inputByteIndex = FirstByte;
    int inputGroupsPerLine = 0;
    unsigned char bitsFromLastByte;

    const UumapEncodeData* encodeData = &base64UumapEncodeData;

    // header
    textStream << encodeData->header << " 644 " << fileName;

    const int firstLineLength = qMin( range.width(), inputLineLength );
    if( firstLineLength > 0 )
    {
        textStream << '\n';
        if( encodeData->hasLength )
            textStream << encodeData->mapByte( firstLineLength );
    }

    for( Okteta::Address i=range.start(); i<=range.end(); ++i )
    {
        const Okteta::Byte byte = byteArrayModel->byte( i );

        switch( inputByteIndex )
        {
        case FirstByte:
            // bits 7..2
            textStream << encodeData->mapByte( byte >> 2 );
            // bits 1..0 -> 5..4 for next
            bitsFromLastByte = (byte & 0x3) << 4;
            inputByteIndex = SecondByte;
            break;
        case SecondByte:
            // from last and bits 7..4 as 3..0 from this
            textStream << encodeData->mapByte( bitsFromLastByte | byte >> 4 );
            // bits 3..0 -> 5..2 for next
            bitsFromLastByte = (byte & 0xf) << 2;
            inputByteIndex = ThirdByte;
            break;
        case ThirdByte:
            // from last and bits 7..6 as 1..0 from this
            textStream << encodeData->mapByte( bitsFromLastByte | byte >> 6 );
            // bits 5..0
            textStream << encodeData->mapByte( byte & 0x3F );
            inputByteIndex = FirstByte;
            ++inputGroupsPerLine;
            if( inputGroupsPerLine >= maxInputGroupsPerLine && i<range.end() )
            {
                const int remainsCount = range.end() - i;
                const int nextLineLength = qMin( remainsCount, inputLineLength );
                textStream << '\n';
                if( encodeData->hasLength )
                    textStream << encodeData->mapByte( nextLineLength );
                inputGroupsPerLine = 0;
            }
            break;
        }
    }
    const bool hasBitsLeft = ( inputByteIndex != FirstByte );
    if( hasBitsLeft )
        textStream << encodeData->mapByte(bitsFromLastByte)
                   // padding
                   << (inputByteIndex==SecondByte ? encodeData->twoBytePad : encodeData->oneBytePad);
    // footer
    textStream << encodeData->footer;

    return success;
}

ByteArrayUuEncodingStreamEncoder::~ByteArrayUuEncodingStreamEncoder() {}

}
