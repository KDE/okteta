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


namespace Kasten
{

static const int outputLineLength = 78;
static const int maxOutputBytesPerLine = outputLineLength;

enum RecordType { BlockHeader=0, DataSequence2B=1, DataSequence3B=2, DataSequence4B=3,
                  RecordCount=5, EndOfBlock4B=7, EndOfBlock3B=8, EndOfBlock2B=9 };

static const char startCode = 'S';

static const int byteCountLineOffset = 0;
static const int addressLineOffset = 1;

static const char hexDigits[16] =
    { '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F' };

static inline char charOfRecordType( RecordType type ) { return (char)('0'+type); }
static inline char hexValueOfNibble( int nibble ) { return hexDigits[nibble & 0xF]; }

static inline QTextStream& operator<<( QTextStream& textStream, unsigned char byte )
{
    return textStream << hexValueOfNibble( byte >> 4 )
                      << hexValueOfNibble( byte );
}

static inline void writeBigEndian( unsigned char* line, quint32 value, int byteSize )
{
    while( byteSize > 0 )
    {
        --byteSize;
        line[byteSize] = value;
        value >>= 8;
    }
}

static inline void streamLine( QTextStream& textStream, RecordType recordType,
                               const unsigned char* line )
{
    // checksum is ones' complement of sum of the values in the line
    unsigned char checksum = 0;

    textStream << startCode << charOfRecordType(recordType);

    uint length = line[0];
    for( uint i = 0; i < length; ++i )
    {
        const unsigned char byte = line[i];
        textStream << byte;
        checksum += byte;
    }

    checksum = ~checksum;
    textStream << checksum << '\n';
}


static
void streamBlockHeader( QTextStream& textStream, unsigned char* line,
                        const char* moduleName = 0, const char* description = 0,
                        quint8 version = 0, quint8 revision = 0 )
{
    // cmp. http://linux.die.net/man/5/srec
    // WP says: vendor specific data rather than program data
    static const int moduleNameLineOffset = 3;
    static const int moduleNameLength = 10;
    static const int versionLineOffset = moduleNameLineOffset + moduleNameLength;
    static const int versionLength = 1;
    static const int revisionLineOffset = versionLineOffset + versionLength;
    static const int revisionLength = 1;
    static const int descriptionLineOffset = revisionLineOffset + revisionLength;
    static const int descriptionLength = 18;

    line[addressLineOffset] = 0; // address unused
    line[addressLineOffset+1] = 0; // address unused

    // leave data empty for now
    line[byteCountLineOffset] = 3;//recordCountByteCount;

    streamLine( textStream, BlockHeader, line );
}

static
void streamRecordCount( QTextStream& textStream, unsigned char* line,
                        quint16 recordCount )
{
    static const int recordCountByteCount = 3;

    line[byteCountLineOffset] = recordCountByteCount;
    writeBigEndian( &line[addressLineOffset], recordCount, 2 );

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

static
void streamBlockEnd( QTextStream& textStream, unsigned char* line,
                     quint32 startAddress = 0 ) // TODO: make address
{
    static const int blockEndByteCount = 5;

    line[byteCountLineOffset] = blockEndByteCount;
    writeBigEndian( &line[addressLineOffset], startAddress, 4 );

    streamLine( textStream, EndOfBlock4B, line );
}

ByteArraySRecStreamEncoder::ByteArraySRecStreamEncoder()
  : AbstractByteArrayStreamEncoder( i18nc("name of the encoding target","S-Record"), QString::fromLatin1("text/plain") )
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
    static const int countByteSize = 1;
    static const int dataAddressSize = 4;
     // TODO: depends on actually used address size
    static const int maxDataPerLineCount = maxLineLength - countByteSize - dataAddressSize;

    const Okteta::ByteArrayTableLayout layout( byteArrayView->noOfBytesPerLine(),
                                               byteArrayView->firstLineOffset(),
                                               byteArrayView->startOffset(), 0, byteArrayModel->size() );

    const Okteta::Coord startCoord = layout.coordOfIndex( range.start() );
    const int lastLinePosition = layout.lastLinePosition( startCoord.line() );
    const int dataPerLineCount = qMin( byteArrayView->noOfBytesPerLine(), maxDataPerLineCount );
    unsigned char line[maxLineLength];
    unsigned char* lineData = &line[addressLineOffset+dataAddressSize];
    Okteta::Address lineOffset = range.start();
    const int firstDataEnd = lastLinePosition - startCoord.pos() + 1;
    int d = 0;
    int nextDataEnd = qMin( firstDataEnd, dataPerLineCount );
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
            line[byteCountLineOffset] = d + 1 + dataAddressSize;
            writeBigEndian( &line[addressLineOffset], lineOffset, dataAddressSize );

            streamLine( textStream, DataSequence4B, line );

            ++recordCount;
            lineOffset = i;
            d = 0;
            nextDataEnd = qMin( range.end()-i+1, dataPerLineCount );
        }
    }

    // footer
    streamRecordCount( textStream, line, recordCount );
    streamBlockEnd( textStream, line );

    return success;
}

ByteArraySRecStreamEncoder::~ByteArraySRecStreamEncoder() {}

}
