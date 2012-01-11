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

#include "bytearraysrecstreamencoder.h"

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

SRecStreamEncoderSettings::SRecStreamEncoderSettings()
 : addressSizeId( FourBytesId )
{}

const char ByteArraySRecStreamEncoder::hexDigits[16] =
{ '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F' };


void ByteArraySRecStreamEncoder::streamLine( QTextStream& textStream, RecordType recordType,
                                             const unsigned char* line )
{
    // checksum is ones' complement of sum of the values in the line
    unsigned char checksum = 0;

    textStream << startCode << charOfRecordType(recordType);

    const uint length = line[0];
    for( uint i = 0; i < length; ++i )
    {
        const unsigned char byte = line[i];
        textStream << hexValueOfNibble( byte >> 4 )
                   << hexValueOfNibble( byte );
        checksum += byte;
    }

    checksum = ~checksum;
    textStream << hexValueOfNibble( checksum >> 4 ) << hexValueOfNibble( checksum ) << '\n';
}


void ByteArraySRecStreamEncoder::streamBlockHeader( QTextStream& textStream, unsigned char* line )
//                         const char* moduleName = 0, const char* description = 0,
//                         quint8 version = 0, quint8 revision = 0 )
{
    // cmp. http://linux.die.net/man/5/srec
    // WP says: vendor specific data rather than program data
//     static const int moduleNameLineOffset = 3;
//     static const int moduleNameLength = 10;
//     static const int versionLineOffset = moduleNameLineOffset + moduleNameLength;
//     static const int versionLength = 1;
//     static const int revisionLineOffset = versionLineOffset + versionLength;
//     static const int revisionLength = 1;
//     static const int descriptionLineOffset = revisionLineOffset + revisionLength;
//     static const int descriptionLength = 18;
    static const int headerByteCount = 3;

    line[addressLineOffset] = 0; // address unused
    line[addressLineOffset+1] = 0; // address unused

    // leave data empty for now
    line[byteCountLineOffset] = headerByteCount;

    streamLine( textStream, BlockHeader, line );
}

void ByteArraySRecStreamEncoder::streamRecordCount( QTextStream& textStream, unsigned char* line,
                                                    quint16 recordCount )
{
    static const int recordCountLineSize = 2;
    static const int recordCountByteCount = byteCountLineSize + recordCountLineSize;

    line[byteCountLineOffset] = recordCountByteCount;
    writeBigEndian( &line[addressLineOffset], recordCount, recordCountLineSize );

    streamLine( textStream, RecordCount, line );
}

// from M68000PRM.pdf:
//comp. with tty 28 bytes are max (with 3b address)
// terminated with CR if downloading
// s-record may have some initial field, e.g. for line-number
// end of x-block: The address ﬁeld may optionally contain the x-byte address of the instruction to which control is to be passed.
// Under VERSAdos, the resident linkerOs ENTRY command can be used to
// specify this address. If this address is not speciﬁed, the ﬁrst entry point speci-
// ﬁcation encountered in the object module input will be used. There is no code/
// data ﬁeld.

// TODO: recordType is not limited to valid values, also brings recalculation of addressLineSize
void ByteArraySRecStreamEncoder::streamBlockEnd( QTextStream& textStream, unsigned char* line,
                                                 RecordType recordType, quint32 startAddress )
{
    const int addressLineSize = 11 - recordType;
    const int blockEndByteCount = byteCountLineSize + addressLineSize;

    line[byteCountLineOffset] = blockEndByteCount;
    writeBigEndian( &line[addressLineOffset], startAddress, addressLineSize );

    streamLine( textStream, recordType, line );
}


ByteArraySRecStreamEncoder::ByteArraySRecStreamEncoder()
  : AbstractByteArrayStreamEncoder( i18nc("name of the encoding target","S-Record"), QString::fromLatin1("text/x-srecord") )
{}


bool ByteArraySRecStreamEncoder::encodeDataToStream( QIODevice* device,
                                                     const ByteArrayView* byteArrayView,
                                                     const Okteta::AbstractByteArrayModel* byteArrayModel,
                                                     const Okteta::AddressRange& range )
{
    Q_UNUSED( byteArrayView );

    bool success = true;

    // encode
    QTextStream textStream( device );

    // prepare
    static const int maxLineLength = 64 / 2;
    const int addressLineSize = 4 - mSettings.addressSizeId;
    const int maxDataPerLineCount = maxLineLength - byteCountLineSize - addressLineSize;
    const int dataLineOffset = addressLineOffset + addressLineSize;

    const Okteta::ByteArrayTableLayout layout( byteArrayView->noOfBytesPerLine(),
                                               byteArrayView->firstLineOffset(),
                                               byteArrayView->startOffset(), 0, byteArrayModel->size() );

    const Okteta::Coord startCoord = layout.coordOfIndex( range.start() );
    const int lastLinePosition = layout.lastLinePosition( startCoord.line() );
    const int dataPerLineCount = qMin( byteArrayView->noOfBytesPerLine(), maxDataPerLineCount );
    const RecordType dataSequenceType =
        static_cast<RecordType>( DataSequence4B - mSettings.addressSizeId );
    const RecordType endOfBlockType =
        static_cast<RecordType>( EndOfBlock4B + mSettings.addressSizeId );

    unsigned char line[maxLineLength];

    unsigned char* const lineData = &line[dataLineOffset];
    const int firstDataEnd = lastLinePosition - startCoord.pos() + 1;
    int d = 0;
    int nextDataEnd = qMin( firstDataEnd, dataPerLineCount );
    Okteta::Address recordOffset = range.start();
    int recordCount = 0;

    // header
    streamBlockHeader( textStream, line );

    Okteta::Address i = range.start();
    while( i<=range.end() )
    {
        const Okteta::Byte byte = byteArrayModel->byte( i );
        lineData[d] = byte;

        ++d;
        ++i;
        if( d == nextDataEnd )
        {
            line[byteCountLineOffset] = d + 1 + addressLineSize;
            writeBigEndian( &line[addressLineOffset], recordOffset, addressLineSize );

            streamLine( textStream, dataSequenceType, line );

            ++recordCount;
            recordOffset = i;
            d = 0;
            nextDataEnd = qMin( range.end()-i+1, dataPerLineCount );
        }
    }

    // footer
    streamRecordCount( textStream, line, recordCount );
    streamBlockEnd( textStream, line, endOfBlockType );

    return success;
}

ByteArraySRecStreamEncoder::~ByteArraySRecStreamEncoder() {}

}
