/***************************************************************************
                          kbytecodec.cpp  -  description
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


// app specific
#include "kbytecodec.h"

using namespace KHE;


const KByteCodec::coding KByteCodec::CodingFunction[6] =
{ KByteCodec::toAscii,
  KByteCodec::toHexadecimal,
  KByteCodec::toDecimal,
  KByteCodec::toOctal,
  KByteCodec::toBinary,
  KByteCodec::toDummy };

const KByteCodec::decoding KByteCodec::DecodingFunction[6] =
{ KByteCodec::fromAscii,
  KByteCodec::fromHexadecimal,
  KByteCodec::fromDecimal,
  KByteCodec::fromOctal,
  KByteCodec::fromBinary,
  KByteCodec::fromDummy };

const unsigned int KByteCodec::CodingWidth[6] = { 1, 2, 3, 3, 8, 0 };

const char KByteCodec::Digit[16] =      { '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F' };
const char KByteCodec::SmallDigit[16] = { '0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f' };

void KByteCodec::toAscii( char *Digits, unsigned char Char )
{
  *Digits = (char)Char;
}

void KByteCodec::toHexadecimal( char *Digits, unsigned char Char )
{
  *Digits++ = Digit[(Char>>4)&0x0F];
  *Digits   = Digit[(Char)   &0x0F];
}

void KByteCodec::toHexadecimalSmall( char *Digits, unsigned char Char )
{
  *Digits++ = SmallDigit[(Char>>4)&0x0F];
  *Digits   = SmallDigit[(Char)   &0x0F];
}

void KByteCodec::toDecimal( char *Digits, unsigned char Char )
{
  unsigned char C = Char / 100;
  *Digits++ = Digit[C];
  Char -= C * 100;
  C = Char / 10;
  *Digits++ = Digit[C];
  Char -= C * 10;
  *Digits =   Digit[Char];
}

void KByteCodec::toOctal( char *Digits, unsigned char Char )
{
  *Digits++ = Digit[(Char>>6)&0x07];
  *Digits++ = Digit[(Char>>3)&0x07];
  *Digits =   Digit[(Char)   &0x07];
}


void KByteCodec::toBinary( char *Digits, unsigned char Char )
{
  for( unsigned char M=1<<7; M>0; M>>=1 )
    *Digits++ = (Char & M) ? Digit[1] : Digit[0];
}

void KByteCodec::toDummy( char */*Digits*/, unsigned char /*Char*/ )
{
}





const unsigned char *KByteCodec::fromAscii( unsigned char *Char, const unsigned char *Digits )
{
  *Char = *Digits++;
  return Digits;
}



const unsigned char *KByteCodec::fromHexadecimal( unsigned char *Char, const unsigned char *Digits )
{
  unsigned char C = 0;
  unsigned char d = 0;
  do
  {
    unsigned char Digit = *Digits;

    if( Digit >= '0' && Digit <= '9' )
      Digit -= '0';
    else if( Digit >= 'A' && Digit <= 'F' )
      Digit = Digit - 'A' + 10;
    else if( Digit >= 'a' && Digit <= 'f' )
      Digit = Digit - 'a' + 10;
    else
      break;

    C <<= 4;
    C += Digit;

    ++Digits;
    ++d;
  }
  while( d < 2 );

  *Char = C;
  return Digits;
}


const unsigned char *KByteCodec::fromDecimal( unsigned char *Char, const unsigned char *Digits )
{
  unsigned char C = 0;
  unsigned char d = 0;
  do
  {
    unsigned char Digit = *Digits;

    if( Digit >= '0' && Digit <= '9' )
      Digit -= '0';
    else
      break;
    if( C == 25 && Digit > 5 )
      break;

    C *= 10;
    C += Digit;

    ++Digits;
    ++d;
  }
  while( d < 3 );

  *Char = C;
  return Digits;
}


const unsigned char *KByteCodec::fromOctal( unsigned char *Char, const unsigned char *Digits )
{
  unsigned char C = 0;
  unsigned char d = 0;
  do
  {
    unsigned char Digit = *Digits;

    if( Digit >= '0' && Digit <= '7' )
      Digit -= '0';
    else
      break;

    C <<= 3;
    C += Digit;

    ++Digits;
    ++d;
  }
  while( d < 3 );

  *Char = C;
  return Digits;
}


const unsigned char *KByteCodec::fromBinary( unsigned char *Char, const unsigned char *Digits )
{
  unsigned char C = 0;
  unsigned char d = 0;
  do
  {
    unsigned char Digit = *Digits;

    if( Digit == '0' && Digit == '1' )
      Digit -= '0';
    else
      break;

    C <<= 1;
    C += Digit;

    ++Digits;
    ++d;
  }
  while( d < 8 );

  *Char = C;
  return Digits;
}


const unsigned char *KByteCodec::fromDummy( unsigned char *Char, const unsigned char *Digits )
{
  *Char = 0;
  return Digits;
}
