/***************************************************************************
                          reversebytearrayfilter.cpp  -  description
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


#include "reversebytearrayfilter.h"

// Okteta core
#include <kabstractbytearraymodel.h>
// KDE
#include <KLocale>


ReverseByteArrayFilter::ReverseByteArrayFilter()
 : AbstractByteArrayFilter( i18n("REVERSE data") )
{}

AbstractByteArrayFilterParameterSet *ReverseByteArrayFilter::parameterSet() { return &mParameterSet; }

bool ReverseByteArrayFilter::filter( char *result,
                                     KHECore::KAbstractByteArrayModel *model, const KHE::KSection &section ) const
{
    int r = section.width()-1;
    int m = section.start();
    while( m <= section.end() )
    {
        unsigned char byte = (unsigned char)model->datum( m++ );
        unsigned char reverseByte;
        if( mParameterSet.invertsBits() )
        {
            reverseByte = 0;
            for( int b = 7; b >= 0; --b )
            {
                reverseByte |= (byte & 0x80) >> b;
                byte <<= 1;
            }
        }
        else
            reverseByte = byte;
        result[r--] = reverseByte;
    }

    return true;
}

ReverseByteArrayFilter::~ReverseByteArrayFilter()
{
}
