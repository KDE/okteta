/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2008-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "xorbytearrayfilter.h"

// Okteta core
#include <abstractbytearraymodel.h>
// KDE
#include <KLocale>


XOrByteArrayFilter::XOrByteArrayFilter()
 : AbstractByteArrayFilter(
     i18nc("name of the filter; it does a logic XOR operation",
           "operand XOR data") )
{}

AbstractByteArrayFilterParameterSet *XOrByteArrayFilter::parameterSet() { return &mParameterSet; }

bool XOrByteArrayFilter::filter( Okteta::Byte* result,
                                 Okteta::AbstractByteArrayModel *model, const Okteta::AddressRange& range ) const
{
    const QByteArray operand = mParameterSet.operand();
    const int operandSize = operand.size();

    if( mParameterSet.alignAtEnd() )
    {
        const int behindLastResult = range.width();
        int r = behindLastResult;
        Okteta::Address m = range.nextBehindEnd();
        int nextBlockEnd = r - FilteredByteCountSignalLimit;

        while( m > range.start() )
        {
            int o = operandSize;
            while( m > range.start() && o > 0 )
                result[(r--)-1] = model->byte( (m--)-1 ) ^ operand[(o--)-1];

            if( r <= nextBlockEnd )
            {
                nextBlockEnd -= FilteredByteCountSignalLimit;
                emit filteredBytes( behindLastResult - r );
            }
        }
    }
    else
    {
        int r = 0;
        Okteta::Address m = range.start();
        int nextBlockEnd = FilteredByteCountSignalLimit;

        while( m <= range.end() )
        {
            int o = 0;
            while( m <= range.end() && o < operandSize )
                result[r++] = model->byte( m++ ) ^ operand[o++];

            if( r >= nextBlockEnd )
            {
                nextBlockEnd += FilteredByteCountSignalLimit;
                emit filteredBytes( r );
            }
        }
    }

    return true;
}

XOrByteArrayFilter::~XOrByteArrayFilter()
{
}
