/*
    This file is part of the Okteta Kasten module, part of the KDE project.

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

#include "andbytearrayfilter.h"

// Okteta core
#include <abstractbytearraymodel.h>
// KDE
#include <KLocale>


AndByteArrayFilter::AndByteArrayFilter()
 : AbstractByteArrayFilter(
     i18nc("name of the filter; it does a logic AND operation",
           "operand AND data") )
{}

AbstractByteArrayFilterParameterSet *AndByteArrayFilter::parameterSet() { return &mParameterSet; }

bool AndByteArrayFilter::filter( char *result,
                                 KHECore::AbstractByteArrayModel *model, const KHE::Section &section ) const
{
    const QByteArray operand = mParameterSet.operand();
    const int operandSize = operand.size();
    int filteredBytesCount = 0;

    if( mParameterSet.alignAtEnd() )
    {
        int r = section.width();
        int m = section.nextBehindEnd();

        while( m > section.start() )
        {
            int o = operandSize;
            while( m > section.start() && o > 0 )
                result[(r--)-1] = model->datum( (m--)-1 ) & operand[(o--)-1];

            filteredBytesCount += (operandSize-o);
            if( filteredBytesCount >= FilteredByteCountSignalLimit )
            {
                filteredBytesCount = 0;
                emit filteredBytes( section.end()-m );
            }
        }
    }
    else
    {
        int r = 0;
        int m = section.start();

        while( m <= section.end() )
        {
            int o = 0;
            while( m <= section.end() && o < operandSize )
                result[r++] = model->datum( m++ ) & operand[o++];

            filteredBytesCount += o;
            if( filteredBytesCount >= FilteredByteCountSignalLimit )
            {
                filteredBytesCount = 0;
                emit filteredBytes( m-section.start() );
            }
        }
    }

    return true;
}

AndByteArrayFilter::~AndByteArrayFilter()
{
}
