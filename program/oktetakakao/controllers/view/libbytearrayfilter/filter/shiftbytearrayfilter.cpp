/***************************************************************************
                          shiftbytearrayfilter.cpp  -  description
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


#include "shiftbytearrayfilter.h"

// Okteta core
#include <kabstractbytearraymodel.h>
// KDE
#include <KLocale>

#include <stdlib.h>

static const int BitsPerByte = 8;

ShiftByteArrayFilter::ShiftByteArrayFilter()
 : AbstractByteArrayFilter( i18n("SHIFT data") )
{}

AbstractByteArrayFilterParameterSet *ShiftByteArrayFilter::parameterSet() { return &mParameterSet; }

bool ShiftByteArrayFilter::filter( char *result,
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
            // full free bytes
            while( g < shiftByteWidth && m <= section.end() )
            {
                result[r++] = 0;
                ++m;
                ++g;
            }
            // byte layer shift
            while( g < groupSize && m <= section.end() )
            {
                result[r++] = model->datum( (m++)-shiftByteWidth );
                ++g;
            }
            // bit layer shift
            for( int b=1; b <= g; ++b )
            {
                result[r-b] = (unsigned char)result[r-b]>>shiftBitWidth;
                if( b < g )
                    result[r-b] |= (unsigned char)result[r-b-1] << otherShiftBitWidth;
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
            // byte layer shift
            while( g+shiftByteWidth < groupSize && m+shiftByteWidth <= section.end() )
            {
                result[r++] = model->datum( (m++)+shiftByteWidth );
                ++g;
            }
            // full free bytes
            while( g < groupSize && m <= section.end() )
            {
                result[r++] = 0;
                ++m;
                ++g;
            }

            // bit layer shift
            for( int b=g; b>0; --b )
            {
                result[r-b] = (unsigned char)result[r-b] << shiftBitWidth;
                if( b>1 )
                    result[r-b] |= (unsigned char)result[r-b+1] >> otherShiftBitWidth;
            }
        }
    }

    return true;
}

ShiftByteArrayFilter::~ShiftByteArrayFilter() {}
