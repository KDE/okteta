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

#include "orbytearrayfilter.h"

// Okteta core
#include <kabstractbytearraymodel.h>
// KDE
#include <KLocale>


OrByteArrayFilter::OrByteArrayFilter()
 : AbstractByteArrayFilter(
     i18nc("name of the filter; it does a logic OR operation",
           "operand OR data") )
{}

AbstractByteArrayFilterParameterSet *OrByteArrayFilter::parameterSet() { return &mParameterSet; }

bool OrByteArrayFilter::filter( char *result,
                                KHECore::KAbstractByteArrayModel *model, const KHE::KSection &section ) const
{
    const QByteArray operand = mParameterSet.operand();
    const int operandSize = operand.size();

    if( mParameterSet.alignAtEnd() )
    {
        int r = section.width();
        int m = section.nextBehindEnd();

        while( m > 0 )
        {
            int o = operandSize;
            while( m > 0 && o > 0 )
                result[(r--)-1] = model->datum( (m--)-1 ) | operand[(o--)-1];
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
                result[r++] = model->datum( m++ ) | operand[o++];
        }
    }

    return true;
}

OrByteArrayFilter::~OrByteArrayFilter()
{
}
