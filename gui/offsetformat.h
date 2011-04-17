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

#ifndef OKTETA_OFFSETFORMAT_H
#define OKTETA_OFFSETFORMAT_H


// lib
#include "oktetagui_export.h"

namespace Okteta
{

/**
  *@author Friedrich W. H. Kossebau
  */

class OKTETAGUI_EXPORT OffsetFormat
{
  public:
    /** */
    typedef void (*print)( char *Buffer, unsigned int Offset );
    /** */
    enum Format { Hexadecimal=0, Decimal, None };
    /** */
    static const int MaxFormatWidth = 9;

  private:
    OffsetFormat();
    ~OffsetFormat();

  public:
    /** */
    static unsigned int codingWidth( int i );
    /** */
    static print printFunction( int i );

  public:
    static void printHexadecimalOffset( char *Buffer, unsigned int Offset );
    static void printHexadecimalSmallOffset( char *Buffer, unsigned int Offset );
    static void printDecimalOffset( char *Buffer, unsigned int Offset );

  protected:
    /** */
    static const unsigned int CodingWidth[2]; //TODO: would sizeof(Coding} work?
    /** */
    static const print PrintFunction[2];
};


inline unsigned int OffsetFormat::codingWidth( int i )
{ return CodingWidth[i]; }

inline OffsetFormat::print OffsetFormat::printFunction( int i )
{ return PrintFunction[i]; }


}

#endif
