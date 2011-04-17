/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "rotatebytearrayfilter.h"

// Okteta core
#include <abstractbytearraymodel.h>
// KDE
#include <KLocale>
// Qt
#include <QtCore/QtGlobal>

static const int RotateBitsPerByte = 8;

RotateByteArrayFilter::RotateByteArrayFilter()
 : AbstractByteArrayFilter(
     i18nc("name of the filter; it moves the bits and pushes the ones over the end to the begin again",
           "ROTATE data") )
{}

AbstractByteArrayFilterParameterSet *RotateByteArrayFilter::parameterSet() { return &mParameterSet; }

bool RotateByteArrayFilter::filter( Okteta::Byte* result,
                                    Okteta::AbstractByteArrayModel *model, const Okteta::AddressRange& range ) const
{
    const int groupSize = mParameterSet.groupSize();
    const int groupBitCount = (groupSize * RotateBitsPerByte );
    const int groupShiftBitWidth = qAbs( mParameterSet.moveBitWidth() ) % groupBitCount;

    const int shiftByteWidth = groupShiftBitWidth / RotateBitsPerByte;
    const int shiftBitWidth = groupShiftBitWidth - shiftByteWidth * RotateBitsPerByte;
    const int otherShiftBitWidth = RotateBitsPerByte - shiftBitWidth;
    int filteredBytesCount = 0;

    const bool toRight = ( mParameterSet.moveBitWidth() > 0 );
    if( toRight )
    {
        int r = 0;
        Okteta::Address m = range.start();
        while( m <= range.end() )
        {
            int g = 0;
            // round the edge byte layer shift
            while( g < shiftByteWidth && m+groupSize <= range.end() )
            {
                result[r++] = model->byte( (m++)+groupSize-shiftByteWidth );
                ++g;
            }
            // normal byte layer shift
            while( g < groupSize && m <= range.end() )
            {
                result[r++] = model->byte( (m++)-shiftByteWidth );
                ++g;
            }

            // bit layer shift
            const unsigned char last = (unsigned char) result[r-1];
            for( int b=1; b<=g; ++b )
            {
                result[r-b] = (unsigned char)result[r-b]>>shiftBitWidth;
                if( b < g )
                    result[r-b] |= (unsigned char)result[r-b-1]<<otherShiftBitWidth;
                else if( g == groupSize )
                    result[r-b] |= last<<otherShiftBitWidth;
            }

            filteredBytesCount += g;
            if( filteredBytesCount >= FilteredByteCountSignalLimit )
            {
                filteredBytesCount = 0;
                emit filteredBytes( m-range.start() );
            }
        }
    }
    else
    {
        int r = 0;
        Okteta::Address m = range.start();
        while( m <= range.end() )
        {
            int g = 0;
            // normal byte layer shift
            while( g+shiftByteWidth < groupSize && m+shiftByteWidth <= range.end() )
            {
                result[r++] = model->byte( (m++)-shiftByteWidth );
                ++g;
            }
            // round the edge byte layer shift
            while( g < groupSize && m <= range.end() )
            {
                result[r++] = model->byte( (m++)+shiftByteWidth-groupSize );
                ++g;
            }

            // bit layer shift
            const unsigned char first = result[r-g];
            for( int b=g; b>0; --b )
            {
                result[r-b] = (unsigned char)result[r-b] << shiftBitWidth;
                if( b > 1 )
                    result[r-b] |= (unsigned char)result[r-b+1] >> otherShiftBitWidth;
                else if( g == groupSize )
                    result[r-b] |= first >> otherShiftBitWidth;
            }

            filteredBytesCount += g;
            if( filteredBytesCount >= FilteredByteCountSignalLimit )
            {
                filteredBytesCount = 0;
                emit filteredBytes( m-range.start() );
            }
        }
    }

    return true;
}

RotateByteArrayFilter::~RotateByteArrayFilter() {}
