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
    /**
     * @return number of digits the greatest byte gets decoded into
     */
    virtual unsigned int encodingWidth() const = 0;
    /**
     * @return largest value which could get another digit appended and stay below 256.
     */
    virtual unsigned char digitsFilledLimit() const = 0;

    /**
     * encodes the byte using full coding width, prefixing with 0s if needed,
     * and writes the result to digits
     * @param digits 
     * @param pos offset in digits where to write the code to
     * @param byte data to encode
     */
    virtual void encode( QString &digits, unsigned int pos, const unsigned char byte ) const = 0;
    /**
     * encodes the byte and writes the result to digits, no leading 0s
     * @param digits
     * @param pos offset in digits where to write the code to
     * @param byte data to encode
     */
    virtual void encodeShort( QString &digits, unsigned int pos, const unsigned char byte ) const = 0;

    /**
     * encodes the byte and writes the result to digits, no leading 0s
     * @param digits
     * @param pos offset in digits where to write the code to
     * @return true if successfull, false otherwise
     */
    virtual bool appendDigit( unsigned char *byte, const unsigned char digit ) const = 0;
    /**
     * tries to remove the last (least significant) digit from byte
     * @param byte value to remove the last digit from
     * @return true if successfull, false otherwise
     */
    virtual void removeLastDigit( unsigned char *byte ) const = 0;
    /**
     * checks if the given digit is used in the encoding
     * @param digit value to check
     * @return true if digit is valid, false otherwise
     */
    virtual bool isValidDigit( const unsigned char digit ) const = 0;
    /**
     * turns the digit into a byte with the value of the digit
     * @param digit digit to turn into the value
     * @return true if successfull, false otherwise
     */
    virtual bool turnToValue( unsigned char *digit ) const = 0;


  public:
    /** */
    unsigned int decode( unsigned char *byte, const QString &digits, unsigned int pos ) const;
};

inline ValueCodec::ValueCodec() {}

}

#endif
