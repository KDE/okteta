/***************************************************************************
                          kbytecodec.h  -  description
                             -------------------
    begin                : Sam Mai 17 2003
    copyright            : (C) 2003 by Friedrich W. H. Kossebau
    email                : Friedrich.W.H@Kossebau.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License version 2 as published by the Free Software Foundation.       *
 *                                                                         *
 ***************************************************************************/


#ifndef KHE_KBYTECODEC_H
#define KHE_KBYTECODEC_H

#include "khe.h"
#include <qstring.h>

namespace KHE
{

/** class that is able to convert codings to and from
  * hexadecimal, decimal, octal, and binary
  *
  * the buffer will be always filled up to CodingWidth, if not using shortCodingFunction
  * a closing '\0' will be always added
  *
  *@author Friedrich W. H. Kossebau
  */

class KByteCodec
{
  public:
    static KByteCodec *createCodec( KCoding C );


  protected: // hiding, as this class should never be instanciated
    KByteCodec() {}

  public: // API to be implemented
    /** */
    virtual unsigned int encodingWidth() const = 0;
    /** */
    virtual unsigned char digitsFilledLimit() const = 0;

    /** encodes the Char and writes the result to */
    virtual void encode( QString &Digits, unsigned  int Pos, const unsigned char Char ) const = 0;
    /** */
    virtual void encodeShort( QString &Digits, unsigned  int Pos, const unsigned char Char ) const = 0;

    /** */
    virtual bool appendDigit( unsigned char *Byte, const unsigned char Digit ) const = 0;
    /** */
    virtual void removeLastDigit( unsigned char *Byte ) const = 0;
    /** */
    virtual bool isValidDigit( const unsigned char Digit ) const = 0;
    /** */
    virtual bool turnToValue( unsigned char *Digit ) const = 0;


  public:
    /** */
    uint decode( unsigned char *Char, const QString &Digits, uint Pos ) const;
};

}

#endif
