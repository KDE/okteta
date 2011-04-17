/*
    This file is part of the Okteta Core library, made within the KDE community.

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

#ifndef OKTETA_VALUECODEC_H
#define OKTETA_VALUECODEC_H

// lib
#include "oktetacore.h"
#include "oktetacore_export.h"
#include "byte.h"

class QString;


namespace Okteta
{

/** Class that is able to convert codings to and from
  * hexadecimal, decimal, octal, and binary.
  *
  * The buffer will be always filled up to CodingWidth, if not using shortCodingFunction.
  *
  *@author Friedrich W. H. Kossebau
  */

class OKTETACORE_EXPORT ValueCodec
{
  public:
    static ValueCodec* createCodec( ValueCoding valueCoding );

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
    virtual Byte digitsFilledLimit() const = 0;

    /**
     * Encodes the byte using full coding width, prefixing with 0s if needed,
     * and writes the result to digits
     * @param digits string to write the digits into
     * @param pos offset in digits where to write the code to
     * @param byte data to encode
     */
    virtual void encode( QString& digits, unsigned int pos, Byte byte ) const = 0;
    // TODO: make this QString* digits
    /**
     * Encodes the byte and writes the result to digits, no leading 0s.
     * @param digits string to write the digits into
     * @param pos offset in digits where to write the code to
     * @param byte data to encode
     */
    virtual void encodeShort( QString& digits, unsigned int pos, Byte byte ) const = 0;
    // TODO: make this QString* digits

    /**
     * Tries to increase the byte value by appending a digit to the digits of
     * the current encoding.
     * @param digit digit to append to encoding of current value.
     * @param pos offset in digits where to write the code to
     * @return true if successful, false otherwise
     */
    virtual bool appendDigit( Byte* byte, unsigned char digit ) const = 0;
    /**
     * Tries to remove the last (least significant) digit from byte.
     * @param byte value to remove the last digit from
     * @return true if successful, false otherwise
     */
    virtual void removeLastDigit( Byte* byte ) const = 0;
    /**
     * Checks if the given digit is used in the encoding.
     * @param digit value to check
     * @return true if digit is valid, false otherwise
     */
    virtual bool isValidDigit( unsigned char digit ) const = 0;
    /**
     * Turns the digit into a byte with the value of the digit.
     * @param digit digit to turn into the value
     * @return true if successful, false otherwise
     */
    virtual bool turnToValue( unsigned char* digit ) const = 0;


  public:
    /**
     * Tries to decode the digits in the text into a byte.
     * @param byte pointer to the variable to store the result in
     * @param text string to turn into the value
     * @param pos offset in the text to start with decoding
     * @return used chars of the string for the decoding, beginning with pos
     */
    unsigned int decode( Byte* byte, const QString& text, unsigned int pos = 0 ) const;
};


inline ValueCodec::ValueCodec() {}

}

#endif
