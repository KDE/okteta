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

#include "invertbytearrayfilter.h"

// Okteta core
#include <kabstractbytearraymodel.h>
// KDE
#include <KLocale>


InvertByteArrayFilter::InvertByteArrayFilter()
 : AbstractByteArrayFilter(
     i18nc("name of the filter; it switches all bits from 0 to 1 and 1 to 0 respectivly",
           "INVERT data") )
{}

AbstractByteArrayFilterParameterSet *InvertByteArrayFilter::parameterSet() { return &mNoParameterSet; }

bool InvertByteArrayFilter::filter( char *result,
                                    KHECore::KAbstractByteArrayModel *model, const KHE::KSection &section ) const
{
    int r = 0;
    int m = section.start();
    int filteredBytesCount = 0;
    while( m <= section.end() )
    {
        result[r++] = ~model->datum( m++ );
        ++filteredBytesCount;
        if( filteredBytesCount >= FilteredByteCountSignalLimit )
        {
            filteredBytesCount = 0;
            emit filteredBytes( m-section.start() );
        }
    }
    return true;
}

InvertByteArrayFilter::~InvertByteArrayFilter() {}
