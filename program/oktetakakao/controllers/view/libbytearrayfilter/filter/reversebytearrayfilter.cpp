/*
    This file is part of the Okteta Kakao module, part of the KDE project.

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

#include "reversebytearrayfilter.h"

// Okteta core
#include <abstractbytearraymodel.h>
// KDE
#include <KLocale>


ReverseByteArrayFilter::ReverseByteArrayFilter()
 : AbstractByteArrayFilter(
     i18nc("name of the filter; it changes the order of the bytes/bits to backwards",
           "REVERSE data") )
{}

AbstractByteArrayFilterParameterSet *ReverseByteArrayFilter::parameterSet() { return &mParameterSet; }

bool ReverseByteArrayFilter::filter( char *result,
                                     KHECore::AbstractByteArrayModel *model, const KHE::Section &section ) const
{
    int r = section.width()-1;
    int m = section.start();
    int filteredBytesCount = 0;
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

        ++filteredBytesCount;
        if( filteredBytesCount >= FilteredByteCountSignalLimit )
        {
            filteredBytesCount = 0;
            emit filteredBytes( m-section.start() );
        }
    }

    return true;
}

ReverseByteArrayFilter::~ReverseByteArrayFilter()
{
}
