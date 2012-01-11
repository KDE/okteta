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

#include "bytearrayihexstreamencoder.h"

// lib
#include <bytearrayview.h>
// Okteta gui
#include <bytearraytablelayout.h>
// Okteta core
#include <abstractbytearraymodel.h>
// KDE
#include <KLocale>
// Qt
#include <QtCore/QTextStream>


namespace Kasten2
{

IHexStreamEncoderSettings::IHexStreamEncoderSettings()
 : addressSizeId( Bits32Id )
{}


const char ByteArrayIHexStreamEncoder::hexDigits[16] =
    { '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F' };


void ByteArrayIHexStreamEncoder::streamLine( QTextStream& textStream,
                                             const unsigned char* line )
{
    // checksum is two's complement of sum of the values in the line
    unsigned char checksum = 0;

    textStream << startCode;

    const uint length = byteCountLineSize + addressLineSize + recordTypeLineSize + line[0];
    for( uint i = 0; i < length; ++i )
    {
        const unsigned char byte = line[i];
        textStream << hexValueOfNibble( byte >> 4 )
                   << hexValueOfNibble( byte );
        checksum += byte;
    }

    checksum = (checksum ^ 0xFF) + 1;
    textStream << hexValueOfNibble( checksum >> 4 )
               << hexValueOfNibble( checksum ) << '\n';
}

void ByteArrayIHexStreamEncoder::streamExtendedSegmentAddress( QTextStream& textStream,
                                                               unsigned char* line,
                                                               quint16 upperSegmentBaseAddress )
{
    static const int nullAddress = 0;
    static const int upperSegmentBaseAddressSize = 2;

    line[byteCountLineOffset] = upperSegmentBaseAddressSize;
    writeBigEndian( &line[addressLineOffset], nullAddress, addressLineSize );
    line[recordTypeLineOffset] = ExtendedSegmentAddressRecord;
    line[dataLineOffset] = upperSegmentBaseAddress >> 8;
    line[dataLineOffset+1] = upperSegmentBaseAddress;

    streamLine( textStream, line );
}

void ByteArrayIHexStreamEncoder::streamExtendedLinearAddress( QTextStream& textStream,
                                                              unsigned char* line,
                                                              quint16 upperLinearBaseAddress )
{
    static const int nullAddress = 0;
    static const int upperLinearBaseAddressSize = 2;

    line[byteCountLineOffset] = upperLinearBaseAddressSize;
    writeBigEndian( &line[addressLineOffset], nullAddress, addressLineSize );
    line[recordTypeLineOffset] = ExtendedLinearAddressRecord;
    line[dataLineOffset] = upperLinearBaseAddress >> 8;
    line[dataLineOffset+1] = upperLinearBaseAddress;

    streamLine( textStream, line );
}

void ByteArrayIHexStreamEncoder::streamEndOfFile( QTextStream& textStream,
                                                  unsigned char* line,
                                                  quint16 startAddress )
{
    static const int endOfFileByteCount = 0;

    line[byteCountLineOffset] = endOfFileByteCount;
    writeBigEndian( &line[addressLineOffset], startAddress, addressLineSize );
    line[recordTypeLineOffset] = EndOfFileRecord;

    streamLine( textStream, line );
}


ByteArrayIHexStreamEncoder::ByteArrayIHexStreamEncoder()
  : AbstractByteArrayStreamEncoder( i18nc("name of the encoding target","Intel Hex"), QString::fromLatin1("text/x-ihex") )
{}


bool ByteArrayIHexStreamEncoder::encodeDataToStream( QIODevice* device,
                                                     const ByteArrayView* byteArrayView,
                                                     const Okteta::AbstractByteArrayModel* byteArrayModel,
                                                     const Okteta::AddressRange& range )
{
    Q_UNUSED( byteArrayView );

    bool success = true;

    // encode
    QTextStream textStream( device );

    // prepare
    static const int maxDataPerLineCount = 255;
    static const int maxLineLength =
        maxDataPerLineCount + byteCountLineSize + addressLineSize + recordTypeLineSize;

    const Okteta::ByteArrayTableLayout layout( byteArrayView->noOfBytesPerLine(),
                                               byteArrayView->firstLineOffset(),
                                               byteArrayView->startOffset(), 0, byteArrayModel->size() );

    const Okteta::Coord startCoord = layout.coordOfIndex( range.start() );
    const int lastLinePosition = layout.lastLinePosition( startCoord.line() );
    const int dataPerLineCount = qMin( byteArrayView->noOfBytesPerLine(), maxDataPerLineCount );
    unsigned char line[maxLineLength];
    unsigned char* lineData = &line[dataLineOffset];
    Okteta::Address lineOffset = range.start();
    const int firstDataEnd = lastLinePosition - startCoord.pos() + 1;
    int nextUpperAddressChangeDataEnd = 0x10000 - (range.start() & 0xFFFF);
    int d = 0;
    int nextDataEnd = qMin( dataPerLineCount,
                            qMin(firstDataEnd,nextUpperAddressChangeDataEnd) );

    // data
    if( mSettings.addressSizeId == IHexStreamEncoderSettings::Bits32Id )
    {
        const quint16 upperLinearBaseAddress = (range.start() >> 16);
        streamExtendedLinearAddress( textStream, line, upperLinearBaseAddress );
    }
    else if( mSettings.addressSizeId == IHexStreamEncoderSettings::Bits16Id )
    {
        const quint16 upperSegmentBaseAddress = (range.start() >> 4) & 0xF000;
        streamExtendedSegmentAddress( textStream, line, upperSegmentBaseAddress );
    }
    Okteta::Address i = range.start();
    while( i <= range.end() )
    {
        const Okteta::Byte byte = byteArrayModel->byte( i );
        lineData[d] = byte;

        ++d;
        ++i;
        if( d == nextDataEnd )
        {
            line[byteCountLineOffset] = d;
            writeBigEndian( &line[addressLineOffset], lineOffset, addressLineSize );
            line[recordTypeLineOffset] = DataRecord;

            streamLine( textStream, line );

            lineOffset = i;

            if( d == nextUpperAddressChangeDataEnd )
            {
                if( mSettings.addressSizeId == IHexStreamEncoderSettings::Bits32Id )
                {
                    const quint16 upperLinearBaseAddress = (i >> 16);
                    streamExtendedLinearAddress( textStream, line, upperLinearBaseAddress );
                }
                else if( mSettings.addressSizeId == IHexStreamEncoderSettings::Bits16Id )
                {
                    const quint16 upperSegmentBaseAddress = (i >> 4) & 0xF000;
                    streamExtendedSegmentAddress( textStream, line, upperSegmentBaseAddress );
                }
            }
            nextUpperAddressChangeDataEnd = 0x10000 - (i & 0xFFFF);
            nextDataEnd = qMin( dataPerLineCount,
                                qMin(range.end()-i+1, nextUpperAddressChangeDataEnd) );
            d = 0;
        }
    }

    // footer
    streamEndOfFile( textStream, line );

    return success;
}

ByteArrayIHexStreamEncoder::~ByteArrayIHexStreamEncoder() {}

}
