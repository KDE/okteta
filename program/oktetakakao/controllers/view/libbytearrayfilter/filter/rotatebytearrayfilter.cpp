/***************************************************************************
                          rotatebytearrayfilter.cpp  -  description
                             -------------------
    begin                : Fri Jan 11 2008
    copyright            : 2008 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU General Public License           *
 *   version 2 as published by the Free Software Foundation.               *
 *                                                                         *
 ***************************************************************************/


#include "rotatebytearrayfilter.h"

// Okteta core
#include <kabstractbytearraymodel.h>
// KDE
#include <KLocale>
#include <KDebug>

static const int BitsPerByte = 8;

RotateByteArrayFilter::RotateByteArrayFilter()
 : AbstractByteArrayFilter( i18n("ROTATE data") )
{}

AbstractByteArrayFilterParameterSet *RotateByteArrayFilter::parameterSet() { return &mParameterSet; }

bool RotateByteArrayFilter::filter( char *result,
                                    KHECore::KAbstractByteArrayModel *model, const KHE::KSection &section ) const
{
    const int groupSize = mParameterSet.groupSize();
    const int groupBitCount = (groupSize * BitsPerByte );
    const int groupShiftBitWidth = abs( mParameterSet.moveBitWidth() ) % groupBitCount;

    const int shiftByteWidth = groupShiftBitWidth / BitsPerByte;
    const int shiftBitWidth = groupShiftBitWidth - shiftByteWidth * BitsPerByte;
    const int otherShiftBitWidth = BitsPerByte - shiftBitWidth;

    const bool toRight = ( mParameterSet.moveBitWidth() > 0 );
    if( toRight )
    {
        int r = 0;
        int m = section.start();
        while( m <= section.end() )
        {
            int g = 0;
            // round the edge byte layer shift
            while( g < shiftByteWidth && m+groupSize <= section.end() )
            {
                result[r++] = model->datum( (m++)+groupSize-shiftByteWidth );
                ++g;
            }
            // normal byte layer shift
            while( g < groupSize && m <= section.end() )
            {
                result[r++] = model->datum( (m++)-shiftByteWidth );
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
        }
    }
    else
    {
        int r = 0;
        int m = section.start();
        while( m <= section.end() )
        {
            int g = 0;
            // normal byte layer shift
            while( g+shiftByteWidth < groupSize && m+shiftByteWidth <= section.end() )
            {
                result[r++] = model->datum( (m++)-shiftByteWidth );
                ++g;
            }
            // round the edge byte layer shift
            while( g < groupSize && m <= section.end() )
            {
                result[r++] = model->datum( (m++)+shiftByteWidth-groupSize );
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
        }
    }

    return true;
}

RotateByteArrayFilter::~RotateByteArrayFilter() {}
