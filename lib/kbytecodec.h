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
    /** function template for */
    typedef void (*coding)( char *Digits, unsigned char Char );
    /** function template for the decoding:
      * will stop either when a wrong digit is accessed (including \0) or the maximum coding width has been reached
      * returns a pointer to the char behind the last decoded
      */
    typedef const unsigned char *(*decoding)( unsigned char *Char, const unsigned char *Digits );
    /** */
    typedef bool (*appending)( unsigned char *Byte, unsigned char Digit );
    /** */
    typedef void (*removingLastDigit)( unsigned char *Byte );
    /** */
    typedef bool (*validingDigit)( unsigned char Digit );
    /** */
    static const int MaxCodingWidth = 8;
    /** */
    static const int MaxNoOfDigits = 16;

  private: // hiding, as this class should never be instanciated
    KByteCodec();
    ~KByteCodec();


  public:
    /** */
    static unsigned int codingWidth( KCoding C );
    /** */
    static unsigned char digitsFilledLimit( KCoding C );
    /** */
    static coding codingFunction( KCoding C );
    /** */
    static coding shortCodingFunction( KCoding C );
    /** */
    static decoding decodingFunction( KCoding C );
    /** */
    static appending appendingFunction( KCoding C );
    /** */
    static removingLastDigit removingLastDigitFunction( KCoding C );
    /** */
    static validingDigit validingDigitFunction( KCoding C );

  public: // toing functions
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
    static void toShortHexadecimal( char *Digits, unsigned char Char );
    /** */
    static void toShortHexadecimalSmall( char *Digits, unsigned char Char );
    /** */
    static void toShortDecimal( char *Digits, unsigned char Char );
    /** */
    static void toShortOctal( char *Digits, unsigned char Char );
    /** */
    static void toShortBinary( char *Digits, unsigned char Char );
    /** */

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


    static bool appendToHexadecimal( unsigned char *Byte, unsigned char Digit );
    static bool appendToDecimal( unsigned char *Byte, unsigned char Digit );
    static bool appendToOctal( unsigned char *Byte, unsigned char Digit );
    static bool appendToBinary( unsigned char *Byte, unsigned char Digit );
    static bool appendToDummy( unsigned char *Byte, unsigned char Digit );

    static void removeLastHexadecimalDigit( unsigned char *Byte );
    static void removeLastDecimalDigit( unsigned char *Byte );
    static void removeLastOctalDigit( unsigned char *Byte );
    static void removeLastBinaryDigit( unsigned char *Byte );
    static void removeLastDummyDigit( unsigned char */*Byte*/ );

    static bool turnToHexadecimalValue( unsigned char *Digit );
    static bool turnToDecimalValue( unsigned char *Digit );
    static bool turnToOctalValue( unsigned char *Digit );
    static bool turnToBinaryValue( unsigned char *Digit );
    static bool turnToDummyValue( unsigned char *Digit );

    static bool isValidBigHexadecimalDigit( unsigned char Digit );
    static bool isValidSmallHexadecimalDigit( unsigned char Digit );

    static bool isValidHexadecimalDigit( unsigned char Digit );
    static bool isValidDecimalDigit( unsigned char Digit );
    static bool isValidOctalDigit( unsigned char Digit );
    static bool isValidBinaryDigit( unsigned char Digit );
    static bool isValidDummyDigit( unsigned char Digit );

  protected:
    /** */
    static const unsigned int CodingWidth[NoOfCodings];
    /** */
    static const unsigned char DigitsFilledLimit[NoOfCodings];
    /** */
    static const coding CodingFunction[NoOfCodings];
    /** */
    static const coding ShortCodingFunction[NoOfCodings];
    /** */
    static const decoding DecodingFunction[NoOfCodings];
    /** */
    static const appending AppendingFunction[NoOfCodings];
    /** */
    static const removingLastDigit RemovingLastDigitFunction[NoOfCodings];
    /** */
    static const validingDigit ValidingDigitFunction[NoOfCodings];


  protected:
    /** buffer with paintable digits, should be faster than calculating chars online */
    static const char Digit[MaxNoOfDigits];
    /** buffer with digits a-f lower */
    static const char SmallDigit[MaxNoOfDigits];
};


inline unsigned int KByteCodec::codingWidth( KCoding C )
{ return CodingWidth[C]; }

inline unsigned char KByteCodec::digitsFilledLimit( KCoding C )
{ return DigitsFilledLimit[ C]; }

inline KByteCodec::coding KByteCodec::codingFunction( KCoding C )
{ return CodingFunction[C]; }

inline KByteCodec::coding KByteCodec::shortCodingFunction( KCoding C )
{ return ShortCodingFunction[C]; }

inline KByteCodec::decoding KByteCodec::decodingFunction( KCoding C )
{ return DecodingFunction[C]; }

inline KByteCodec::appending KByteCodec::appendingFunction( KCoding C )
{ return AppendingFunction[C]; }

inline KByteCodec::removingLastDigit KByteCodec::removingLastDigitFunction( KCoding C )
{ return RemovingLastDigitFunction[C]; }

inline KByteCodec::validingDigit KByteCodec::validingDigitFunction( KCoding C )
{ return ValidingDigitFunction[C]; }

}

#endif
