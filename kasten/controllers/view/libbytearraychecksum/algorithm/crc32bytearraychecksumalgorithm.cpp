/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "crc32bytearraychecksumalgorithm.h"

// Okteta core
#include <abstractbytearraymodel.h>
// KDE
#include <KLocale>


class Crc32LookupTable
{
  public:
    Crc32LookupTable();

  public:
    const quint32& operator[]( int i ) const;

  protected:
    static quint32 reverseBits( quint32 bits, char bitCount );

  protected:
    quint32 mTable[256];
};

Crc32LookupTable::Crc32LookupTable()
{
    quint32 polynomial = 0x04c11db7;

      // 256 values representing ASCII character codes.
    for( int i = 0; i <= 256; ++i )
    {
        int value = reverseBits( i, 8 ) << 24;
        for( int j = 0; j < 8; ++j )
        {
            const bool hasMsb = ( value & (1<<31) );
            value <<= 1;
            if( hasMsb )
                value ^= polynomial;
        }
        mTable[i] = reverseBits( value, 32 );
    }
}

quint32 Crc32LookupTable::reverseBits( quint32 bits, char bitCount )
{
    quint32 result = 0;
    for( int i=1; i <= bitCount; ++i )
    {
        if( bits & 0x01 )
            result |= ( 1 << (bitCount-i) );
        bits >>= 1;
    }
    return result;
}

inline const quint32& Crc32LookupTable::operator[]( int i ) const { return mTable[i]; }



Crc32ByteArrayChecksumAlgorithm::Crc32ByteArrayChecksumAlgorithm()
  : AbstractByteArrayChecksumAlgorithm(
     i18nc("name of the checksum algorithm, Cyclic Redundancy Check 32", "CRC-32") )
{}

AbstractByteArrayChecksumParameterSet* Crc32ByteArrayChecksumAlgorithm::parameterSet() { return &mParameterSet; }

bool Crc32ByteArrayChecksumAlgorithm::calculateChecksum( QString* result,
                                                       const Okteta::AbstractByteArrayModel* model, const Okteta::AddressRange& range ) const
{
    Crc32LookupTable lookupTable;
    quint32 crcBits = 0xffffffff;
    Okteta::Address nextBlockEnd = range.start() + CalculatedByteCountSignalLimit;
    for( Okteta::Address i = range.start(); i<=range.end(); ++i )
    {
        const uchar value = (crcBits & 0xFF) ^ model->byte( i );
        crcBits >>= 8;
        crcBits ^= lookupTable[value];

        if( i >= nextBlockEnd )
        {
            nextBlockEnd += CalculatedByteCountSignalLimit;
            emit calculatedBytes( range.localIndex(i)+1 );
        }
    }
    crcBits ^= 0xffffffff;

    *result = QString::fromLatin1("%1").arg( crcBits, 8, 16, QChar::fromLatin1('0') );

    return true;
}

Crc32ByteArrayChecksumAlgorithm::~Crc32ByteArrayChecksumAlgorithm() {}
