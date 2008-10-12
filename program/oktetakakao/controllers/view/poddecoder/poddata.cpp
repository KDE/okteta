/*
    This file is part of the Okteta Kakao module, part of the KDE project.

    Copyright 2007 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "poddata.h"


static inline void swapDataOrder( unsigned char *data, const int length )
{
    const int last = length-1;
    const int mid = length / 2;
    for( int i=0,j=last; i<mid; ++i,--j )
    {
        unsigned char helper = data[i];
        data[i] = data[j];
        data[j] = helper;
    }
}

PODData::PODData()
 : mCurrentData( 0 ),
   mCurrentSize( 0 ),
   mByteOrder( ThisMachineEndianOrder )
{
}

const unsigned char *PODData::data() const { return mCurrentData; }
int PODData::byteOrder()             const { return mByteOrder; }
int PODData::size()                  const { return mCurrentSize; }


unsigned char *PODData::rawData()    { return mAligned64Bit.Data; }

void PODData::setByteOrder( int byteOrder )
{
    if( mByteOrder == byteOrder )
        return;

    mByteOrder = byteOrder;

    // swap data
    if( mCurrentData )
        swapDataOrder( mAligned64Bit.Data, Size );
}

unsigned long PODData::bitValue( int noOfBitsToRead ) const
{
    static const int BitsPerByte = 8;
    static const unsigned char BitMask[9] =
    {
        0, 1<<7, 3<<6, 7<<5, 15<<4, 31<<3, 63<<2, 127<<1, 255
    };

    unsigned long result = 0;

    if( noOfBitsToRead < 1 )
        noOfBitsToRead = 1;

    // TODO: the cursor currently does not go into the byte
    int noOfUsedBits = 0;//7 - state.cell;

    const bool isReverse = ( mByteOrder != ThisMachineEndianOrder );
    const unsigned char *data = mAligned64Bit.Data;
    if( isReverse )
        data += 7;

    while( noOfBitsToRead > 0 )
    {
        unsigned char byte = *data << noOfUsedBits;

        const int noOfNextBits =
            ( (BitsPerByte-noOfUsedBits) >= noOfBitsToRead ) ? noOfBitsToRead : (BitsPerByte-noOfUsedBits);

        byte &= BitMask[noOfNextBits];

        byte >>= BitsPerByte - noOfNextBits;

        result = (result<<noOfNextBits) | byte;

        noOfBitsToRead -= noOfNextBits;
        noOfUsedBits += noOfNextBits;

        if( noOfUsedBits >= BitsPerByte )
        {
            noOfUsedBits = 0;
            if( isReverse )
                --data;
            else
                ++data;
        }
    }

    return result;
}


bool PODData::updateRawData( int size )
{
    const unsigned char *oldCurrentData = mCurrentData;

    if( size>0 )
    {
        if( mByteOrder != ThisMachineEndianOrder )
            swapDataOrder( mAligned64Bit.Data, Size );

        mCurrentData = mAligned64Bit.Data;
        mCurrentSize = size;
    }
    else
    {
        mCurrentData = 0;
        mCurrentSize = 0;
    }
    return ( mCurrentData || oldCurrentData );
}

void PODData::pointers( const void **P8Bit, const void **P16Bit, const void **P32Bit, const void **P64Bit ) const
{
    static const int MachineOffsets[4] = { 0, 0, 0, 0 };
    static const int ReversedOffsets[4] = { 7, 6, 4, 0 };

    const int *offsets = ( mByteOrder == ThisMachineEndianOrder ) ? MachineOffsets : ReversedOffsets;
    const unsigned char *data = mAligned64Bit.Data;

    *P8Bit =  (mCurrentSize>=1) ? data + offsets[0] : 0;
    *P16Bit = (mCurrentSize>=2) ? data + offsets[1] : 0;
    *P32Bit = (mCurrentSize>=4) ? data + offsets[2] : 0;
    *P64Bit = (mCurrentSize>=8) ? data + offsets[3] : 0;
}

void PODData::pointer( const void **P ) const
{
    const int offset = ( mByteOrder == ThisMachineEndianOrder ) ? 0 : 8-mCurrentSize;
    const unsigned char *data = mAligned64Bit.Data;

    *P = (mCurrentSize>=1) ? data + offset : 0;
}
