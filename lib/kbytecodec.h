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


#ifndef KBYTECODEC_H
#define KBYTECODEC_H

#include "khe.h"


namespace KHE
{

/** class that is able to convert codings to and from
  * hexadecimal, decimal, octal, binary and ASCII
  *
  * the buffer will be always filled up to CodingWidth,
  * a closing '\0' has to be added manually (done for performance reason ;)
  *
  *@author Friedrich W. H. Kossebau
  */

class KByteCodec
{
  public:
    /** function template for */
    typedef void (*coding)( char *Digits, unsigned char Char );
    /** function template for the decoding:
      *
      */
    typedef const unsigned char *(*decoding)( unsigned char *Char, const unsigned char *Digits );
    /** */
    static const int MaxCodingWidth = 8;

  private: // hiding, as this class should never be instanciated
    KByteCodec();
    ~KByteCodec();


  public:
    /** */
    static unsigned int codingWidth( KCoding C );
    /** */
    static coding codingFunction( KCoding C );
    /** */
    static decoding decodingFunction( KCoding C );


  public: // toing functions
    /** */
    static void toAscii( char *Digits, unsigned char Char );
    /** */
    static void toHexadecimal( char *Digits, unsigned char Char );
    /** */
    static void toHexadecimalSmall( char *Digits, unsigned char Char );
    /** */
    static void toDecimal( char *Digits, unsigned char Char );
    /** */
    static void toOctal( char *Digits, unsigned char Char );
    /** */
    static void toBinary( char *Digits, unsigned char Char );
    /** */
    static void toDummy( char *Digits, unsigned char Char );

    /** */
    static const unsigned char *fromAscii( unsigned char *Char, const unsigned char *Digits );
    /** */
    static const unsigned char *fromHexadecimal( unsigned char *Char, const unsigned char *Digits );
    /** */
    static const unsigned char *fromDecimal( unsigned char *Char, const unsigned char *Digits );
    /** */
    static const unsigned char *fromOctal( unsigned char *Char, const unsigned char *Digits );
    /** */
    static const unsigned char *fromBinary( unsigned char *Char, const unsigned char *Digits );
    /** */
    static const unsigned char *fromDummy( unsigned char *Char, const unsigned char *Digits );


  protected:
    /** */
    static const unsigned int CodingWidth[6]; //TODO: would sizeof(KCoding} work?
    /** */
    static const coding CodingFunction[6];
    /** */
    static const decoding DecodingFunction[6];


  protected:
    /** buffer with paintable qdigits, should be faster than calculating chars online */
    static const char Digit[16];
    /** buffer with digits a-f lower */
    static const char SmallDigit[16];
};


inline unsigned int KByteCodec::codingWidth( KCoding C )
{ return CodingWidth[C]; }

inline KByteCodec::coding KByteCodec::codingFunction( KCoding C )
{ return CodingFunction[C]; }

inline KByteCodec::decoding KByteCodec::decodingFunction( KCoding C )
{ return DecodingFunction[C]; }

}

#endif
