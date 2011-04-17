/*
    This file is part of the Okteta Gui library, made within the KDE community.

    Copyright 2003 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "offsetformat.h"

// C
#include <stdio.h>

#ifdef __GNUC__
// Will remove this again very soon, Alex
#warning This warning is just for testing LABELS in cmake/cdash
#endif

namespace Okteta {

const unsigned int OffsetFormat::CodingWidth[2] = { 9, 10 };

const OffsetFormat::print OffsetFormat::PrintFunction[2] =
{ OffsetFormat::printHexadecimalOffset, OffsetFormat::printDecimalOffset };



void OffsetFormat::printHexadecimalOffset( char *Buffer, unsigned int Offset )
{
  sprintf( Buffer, "%04X:%04X", Offset>>16, Offset&0x0000FFFF );
}


void OffsetFormat::printHexadecimalSmallOffset( char *Buffer, unsigned int Offset )
{
  sprintf( Buffer, "%04x:%04x", Offset>>16, Offset&0x0000FFFF );
}


void OffsetFormat::printDecimalOffset( char *Buffer, unsigned int Offset )
{
  sprintf( Buffer, "%010u", Offset );
}

}
