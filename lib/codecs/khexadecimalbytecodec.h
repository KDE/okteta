/***************************************************************************
                          khexadecimalbytecodec.h  -  description
                             -------------------
    begin                : Mo Nov 29 2004
    copyright            : (C) 2004 by Friedrich W. H. Kossebau
    email                : Friedrich.W.H@Kossebau.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License version 2 as published by the Free Software Foundation.       *
 *                                                                         *
 ***************************************************************************/


#ifndef KHE_KHEXADECIMALBYTECODEC_H
#define KHE_KHEXADECIMALBYTECODEC_H

// lib specific
#include "kbytecodec.h"

namespace KHE
{

/** class that is able to convert codings to and from hexadecimal
   *
   * the buffer will be always filled up to CodingWidth, if not using shortCodingFunction
   * a closing '\0' will be always added
   *
   *@author Friedrich W. H. Kossebau
 */

class KHexadecimalByteCodec : public KByteCodec
{
  public:
    KHexadecimalByteCodec( bool S = false );

  public:
    bool setSmallDigits( bool S );
    bool smallDigits() const;

  public: // API to be implemented
    /** */
    virtual unsigned int encodingWidth() const { return 2; }
    /** */
    virtual unsigned char digitsFilledLimit() const { return 16; }

    /** encodes the Char and writes the result to */
    virtual void encode( QString &Digits, unsigned  int Pos, const unsigned char Char ) const;
    /** */
    virtual void encodeShort( QString &Digits, unsigned  int Pos, const unsigned char Char ) const;
    /** */
    virtual bool appendDigit( unsigned char *Byte, const unsigned char Digit ) const;
    /** */
    virtual void removeLastDigit( unsigned char *Byte ) const;
    /** */
    virtual bool isValidDigit( const unsigned char Digit ) const;
    /** */
    virtual bool turnToValue( unsigned char *Digit ) const;

  protected:
    const QChar* Digit;
};

}

#endif
