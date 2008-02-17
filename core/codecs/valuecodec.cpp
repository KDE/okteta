/***************************************************************************
                          valuecodec.cpp  -  description
                             -------------------
    begin                : Mo Nov 29 2004
    copyright            : (C) 2004 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License version 2 as published by the Free Software Foundation.       *
 *                                                                         *
 ***************************************************************************/


#include "valuecodec.h"

// lib
#include "kbinarybytecodec.h"
#include "koctalbytecodec.h"
#include "kdecimalbytecodec.h"
#include "khexadecimalbytecodec.h"
// Qt
#include <QtCore/QString>


namespace KHECore {

ValueCodec *ValueCodec::createCodec( KCoding coding )
{
    ValueCodec *result;
    switch( coding )
    {
        case DecimalCoding: result = new KDecimalByteCodec(); break;
        case OctalCoding:   result = new KOctalByteCodec();   break;
        case BinaryCoding:  result = new KBinaryByteCodec();  break;
        case HexadecimalCoding:
        default:            result = new KHexadecimalByteCodec();
    }
    return result;
}

unsigned int ValueCodec::decode( unsigned char *byte, const QString &digits, unsigned int pos ) const
{
    const unsigned int oldPos = pos;
    const unsigned int left = digits.size() - pos;

    unsigned int d = encodingWidth();
    if( left < d )
        d = left;

    unsigned char result = 0;
    do
    {
        if( !appendDigit(&result,digits.at(pos).toLatin1()) )
            break;

        ++pos;
        --d;
    }
    while( d > 0 );

    *byte = result;
    return pos - oldPos;
}

ValueCodec::~ValueCodec() {}

}
