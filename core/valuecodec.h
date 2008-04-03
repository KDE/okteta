/*
    This file is part of the Okteta Core library, part of the KDE project.

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

#ifndef KHE_CORE_VALUECODEC_H
#define KHE_CORE_VALUECODEC_H


// lib
#include "khe.h"
#include "oktetacore_export.h"

class QString;


namespace KHECore
{

/** class that is able to convert codings to and from
  * hexadecimal, decimal, octal, and binary
  *
  * the buffer will be always filled up to CodingWidth, if not using shortCodingFunction
  * a closing '\0' will be always added
  *
  *@author Friedrich W. H. Kossebau
  */

class OKTETACORE_EXPORT ValueCodec
{
  public:
    static ValueCodec *createCodec( KCoding coding );

  public:
    virtual ~ValueCodec();

  protected:
    ValueCodec();

  public: // API to be implemented
    /** */
    virtual unsigned int encodingWidth() const = 0;
    /** */
    virtual unsigned char digitsFilledLimit() const = 0;

    /** encodes the byte and writes the result to */
    virtual void encode( QString &digits, unsigned int pos, const unsigned char byte ) const = 0;
    /** */
    virtual void encodeShort( QString &digits, unsigned int pos, const unsigned char byte ) const = 0;

    /** */
    virtual bool appendDigit( unsigned char *byte, const unsigned char digit ) const = 0;
    /** */
    virtual void removeLastDigit( unsigned char *byte ) const = 0;
    /** */
    virtual bool isValidDigit( const unsigned char digit ) const = 0;
    /** */
    virtual bool turnToValue( unsigned char *digit ) const = 0;


  public:
    /** */
    unsigned int decode( unsigned char *byte, const QString &digits, unsigned int pos ) const;
};

inline ValueCodec::ValueCodec() {}

}

#endif
