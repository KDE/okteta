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

#include "bytearraybase85streamencoder.h"

// Okteta core
#include <abstractbytearraymodel.h>
// KDE
#include <KLocale>
// Qt
#include <QtCore/QTextStream>


namespace Kasten2
{

static inline void streamEncoded( QTextStream& textStream, int& outputBytesPerLine,
                                  quint32 tuple, int inputByteCount )
{
    // radix85 values, most significant first
    char data[5];

    for( int i=4; i>=0; --i )
    {
        // TODO: find an efficient bit manipulating algorithm
        data[i] = tuple % 85;
        tuple /= 85;
    }

    // output inputByteCount+1 from radix85 values
    for( int i = 0; i<=inputByteCount; ++i )
    {
        textStream << (char)(data[i] + 33);
        ++outputBytesPerLine;
        if( outputBytesPerLine >= ByteArrayBase85StreamEncoder::maxOutputBytesPerLine )
        {
            textStream << '\n';
            outputBytesPerLine = 0;
        }
    }
}


// TODO: for now this is just the Adobe/Ascii85 implementation, so present as that
// later also add btoa with different version, e.g. 4.2 added a "y" for 4 spaces
ByteArrayBase85StreamEncoder::ByteArrayBase85StreamEncoder()
  : AbstractByteArrayStreamEncoder( i18nc("name of the encoding target","Ascii85"), QString::fromLatin1("text/x-ascii85") )
{}


bool ByteArrayBase85StreamEncoder::encodeDataToStream( QIODevice* device,
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
    quint32 tuple = 0;

    // header
    int outputBytesPerLine = 2;
    textStream << "<~";

    for( Okteta::Address i=range.start(); i<=range.end(); ++i )
    {
        const Okteta::Byte byte = byteArrayModel->byte( i );

        switch( inputByteIndex )
        {
        case FirstByte:
            tuple |= (byte << 24);
            inputByteIndex = SecondByte;
            break;
        case SecondByte:
            tuple |= (byte << 16);
            inputByteIndex = ThirdByte;
            break;
        case ThirdByte:
            tuple |= (byte <<  8);
            inputByteIndex = FourthByte;
            break;
        case FourthByte:
            tuple |= byte;
            if( tuple == 0 )
            {
                textStream << 'z';
                ++outputBytesPerLine;
                if( outputBytesPerLine >= maxOutputBytesPerLine )
                {
                    textStream << '\n';
                    outputBytesPerLine = 0;
                }
            }
            else
                streamEncoded( textStream, outputBytesPerLine, tuple, inputByteIndex+1 );
            tuple = 0;
            inputByteIndex = FirstByte;
            break;
        }
    }
    const bool hasBitsLeft = ( inputByteIndex != FirstByte );
    if( hasBitsLeft )
        streamEncoded( textStream, outputBytesPerLine, tuple, inputByteIndex );

    // footer
    if( outputBytesPerLine + 2 > maxOutputBytesPerLine )
        textStream << '\n';
    textStream << "~>\n";

    return success;
}

ByteArrayBase85StreamEncoder::~ByteArrayBase85StreamEncoder() {}

}
