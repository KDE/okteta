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


//#include <iostream>
// app specific
#include "kbytecodec.h"

using namespace KHE;


const KByteCodec::coding KByteCodec::CodingFunction[NoOfCodings] =
{ KByteCodec::toHexadecimal,
  KByteCodec::toDecimal,
  KByteCodec::toOctal,
  KByteCodec::toBinary,
  KByteCodec::toDummy };

const KByteCodec::coding KByteCodec::ShortCodingFunction[NoOfCodings] =
{ KByteCodec::toShortHexadecimal,
  KByteCodec::toShortDecimal,
  KByteCodec::toShortOctal,
  KByteCodec::toShortBinary,
  KByteCodec::toDummy };

const KByteCodec::decoding KByteCodec::DecodingFunction[NoOfCodings] =
{ KByteCodec::fromHexadecimal,
  KByteCodec::fromDecimal,
  KByteCodec::fromOctal,
  KByteCodec::fromBinary,
  KByteCodec::fromDummy };

const KByteCodec::adding KByteCodec::AddingFunction[NoOfCodings] =
{ KByteCodec::addToHexadecimal,
  KByteCodec::addToDecimal,
  KByteCodec::addToOctal,
  KByteCodec::addToBinary,
  KByteCodec::addToDummy };

const KByteCodec::removingLastDigit KByteCodec::RemovingLastDigitFunction[NoOfCodings] =
{ KByteCodec::removeLastHexadecimalDigit,
  KByteCodec::removeLastDecimalDigit,
  KByteCodec::removeLastOctalDigit,
  KByteCodec::removeLastBinaryDigit,
  KByteCodec::removeLastDummyDigit };

const KByteCodec::validingDigit KByteCodec::ValidingDigitFunction[NoOfCodings] =
{ KByteCodec::isValidHexadecimalDigit,
  KByteCodec::isValidDecimalDigit,
  KByteCodec::isValidOctalDigit,
  KByteCodec::isValidBinaryDigit,
  KByteCodec::isValidDummyDigit };

const unsigned int KByteCodec::CodingWidth[NoOfCodings] = { 2, 3, 3, 8, 0 };

const char KByteCodec::Digit[MaxNoOfDigits] =      { '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F' };
const char KByteCodec::SmallDigit[MaxNoOfDigits] = { '0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f' };

const unsigned char KByteCodec::DigitsFilledLimit[NoOfCodings] = { 16,26,64,128,0 };

void KByteCodec::toHexadecimal( char *Digits, unsigned char Char )
{
  *Digits++ = Digit[Char>>4];
  *Digits++ = Digit[Char&0x0F];
  *Digits   = '\0';
}

void KByteCodec::toHexadecimalSmall( char *Digits, unsigned char Char )
{
  *Digits++ = SmallDigit[Char>>4];
  *Digits++ = SmallDigit[Char&0x0F];
  *Digits   = '\0';
}

void KByteCodec::toDecimal( char *Digits, unsigned char Char )
{
  unsigned char C = Char / 100;
  *Digits++ = Digit[C];
  Char -= C * 100;
  C = Char / 10;
  *Digits++ = Digit[C];
  Char -= C * 10;
  *Digits++ = Digit[Char];
  *Digits   = '\0';
}

void KByteCodec::toOctal( char *Digits, unsigned char Char )
{
  *Digits++ = Digit[(Char>>6)];
  *Digits++ = Digit[(Char>>3)&0x07];
  *Digits++ = Digit[(Char)   &0x07];
  *Digits   = '\0';
}


void KByteCodec::toBinary( char *Digits, unsigned char Char )
{
  for( unsigned char M=1<<7; M>0; M>>=1 )
    *Digits++ = (Char & M) ? Digit[1] : Digit[0];
  *Digits   = '\0';
}

void KByteCodec::toShortHexadecimal( char *Digits, unsigned char Char )
{
  unsigned char C;
  if( (C = (Char>>4)) )
    *Digits++ = Digit[C];
  *Digits++ = Digit[Char&0x0F];
  *Digits   = '\0';
}

void KByteCodec::toShortHexadecimalSmall( char *Digits, unsigned char Char )
{
  unsigned char C;
  if( (C = (Char>>4)) )
    *Digits++ = SmallDigit[C];
  *Digits++ = SmallDigit[(Char)   &0x0F];
  *Digits   = '\0';
}

void KByteCodec::toShortDecimal( char *Digits, unsigned char Char )
{
  unsigned char C;
  if( (C = Char / 100) )
  {
    *Digits++ = Digit[C];
    Char -= C * 100;
   }
  if( (C = Char / 10) )
  {
    *Digits++ = Digit[C];
    Char -= C * 10;
  }
  *Digits++ = Digit[Char];
  *Digits   = '\0';
}

void KByteCodec::toShortOctal( char *Digits, unsigned char Char )
{
  unsigned char C;
  if( (C = (Char>>6)&0x07) )
    *Digits++ = Digit[C];
  if( (C = (Char>>3)&0x07) )
    *Digits++ = Digit[C];
  *Digits++ = Digit[(Char)&0x07];
  *Digits   = '\0';
}


void KByteCodec::toShortBinary( char *Digits, unsigned char Char )
{
  unsigned char M = 1<<7;
  // find first set bit
  for( ; M>0; M>>=1 )
    if( Char & M )
      break;
  // now set the
  for( ; M>0; M>>=1 )
    *Digits++ = (Char & M) ? '1' : '0';
  *Digits   = '\0';
}

void KByteCodec::toDummy( char */*Digits*/, unsigned char /*Char*/ )
{
}



const unsigned char *KByteCodec::fromHexadecimal( unsigned char *Char, const unsigned char *Digits )
{
  // remove leading 0s
  while( *Digits == '0' ) { ++Digits; }

  unsigned char C = 0;
  unsigned char d = 0;
  do
  {
    if( !addToHexadecimal(&C,*Digits) )
      break;

    ++Digits;
    ++d;
  }
  while( d < 2 );

  *Char = C;
  return Digits;
}


const unsigned char *KByteCodec::fromDecimal( unsigned char *Char, const unsigned char *Digits )
{
  // remove leading 0s
  while( *Digits == '0' ) { ++Digits; }

  unsigned char C = 0;
  unsigned char d = 0;
  do
  {
    if( !addToDecimal(&C,*Digits) )
      break;

    ++Digits;
    ++d;
  }
  while( d < 3 );

  *Char = C;
  return Digits;
}


const unsigned char *KByteCodec::fromOctal( unsigned char *Char, const unsigned char *Digits )
{
  // remove leading 0s
  while( *Digits == '0' ) { ++Digits; }

  unsigned char C = 0;
  unsigned char d = 0;
  do
  {
    if( !addToOctal(&C,*Digits) )
      break;

    ++Digits;
    ++d;
  }
  while( d < 3 );

  *Char = C;
  return Digits;
}


const unsigned char *KByteCodec::fromBinary( unsigned char *Char, const unsigned char *Digits )
{
  // remove leading 0s
  while( *Digits == '0' ) { ++Digits; }

  unsigned char C = 0;
  unsigned char d = 0;
  do
  {
    if( !addToBinary(&C,*Digits) )
      break;

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


bool KByteCodec::isValidHexadecimalDigit( unsigned char Digit )
{
  return isValidDecimalDigit(Digit) || isValidBigHexadecimalDigit(Digit) || isValidSmallHexadecimalDigit(Digit);
}

bool KByteCodec::isValidBigHexadecimalDigit( unsigned char Digit )
{
  return (Digit >= 'A' && Digit <= 'F');
}

bool KByteCodec::isValidSmallHexadecimalDigit( unsigned char Digit )
{
  return (Digit >= 'a' && Digit <= 'f');
}

bool KByteCodec::isValidDecimalDigit( unsigned char Digit )
{
  return Digit >= '0' && Digit <= '9';
}
bool KByteCodec::isValidOctalDigit( unsigned char Digit )
{
  return Digit >= '0' && Digit <= '7';
}
bool KByteCodec::isValidBinaryDigit( unsigned char Digit )
{
  return Digit == '0' || Digit == '1';
}
bool KByteCodec::isValidDummyDigit( unsigned char /*Digit*/ )
{
  return false;
}


bool KByteCodec::turnToHexadecimalValue( unsigned char *Digit )
{
  if( isValidDecimalDigit(*Digit) )
    *Digit -= '0';
  else if( isValidBigHexadecimalDigit(*Digit) )
    *Digit -= 'A' - 10;
  else if( isValidSmallHexadecimalDigit(*Digit) )
    *Digit -= 'a' - 10;
  else
    return false;

  return true;
}

bool KByteCodec::turnToDecimalValue( unsigned char *Digit )
{
  if( isValidDecimalDigit(*Digit) )
  {
    *Digit -= '0';
    return true;
  }
  return false;
}

bool KByteCodec::turnToOctalValue( unsigned char *Digit )
{
  if( isValidOctalDigit(*Digit) )
  {
    *Digit -= '0';
    return true;
  }
  return false;
}

bool KByteCodec::turnToBinaryValue( unsigned char *Digit )
{
  if( isValidBinaryDigit(*Digit) )
  {
    *Digit -= '0';
    return true;
  }
  return false;
}

bool KByteCodec::turnToDummyValue( unsigned char */*Digit*/ )
{
  return false;
}

bool KByteCodec::addToHexadecimal( unsigned char *Byte, unsigned char Digit )
{
  if( turnToHexadecimalValue(&Digit) )
  {
    unsigned char B = *Byte;
    if( B < 16 )
    {
      B <<= 4;
      B += Digit;
      *Byte = B;
      return true;
    }
  }
  return false;
}


bool KByteCodec::addToDecimal( unsigned char *Byte, unsigned char Digit )
{
  if( turnToDecimalValue(&Digit) )
  {
    unsigned char B = *Byte;
    if( B < 26 )
    {
      B *= 10;
      if( (255-B) >= Digit )
      {
        B += Digit;
        *Byte = B;
        return true;
      }
    }
  }
  return false;
}


bool KByteCodec::addToOctal( unsigned char *Byte, unsigned char Digit )
{
  if( turnToOctalValue(&Digit) )
  {
    unsigned char B = *Byte;
    if( B < 64 )
    {
      B <<= 3;
      B += Digit;
      *Byte = B;
      return true;
    }
  }
  return false;
}


bool KByteCodec::addToBinary( unsigned char *Byte, unsigned char Digit )
{
  if( turnToBinaryValue(&Digit) )
  {
    unsigned char B = *Byte;
    if( B < 128 )
    {
      B <<= 1;
      B += Digit;
      *Byte = B;
      return true;
    }
  }
  return false;
}


bool KByteCodec::addToDummy( unsigned char */*Byte*/, unsigned char /*Digit*/ )
{
  return false;
}


void KByteCodec::removeLastHexadecimalDigit( unsigned char *Byte )
{
  *Byte >>= 4;
}
void KByteCodec::removeLastDecimalDigit( unsigned char *Byte )
{
  *Byte /= 10;
}
void KByteCodec::removeLastOctalDigit( unsigned char *Byte )
{
  *Byte >>= 3;
}
void KByteCodec::removeLastBinaryDigit( unsigned char *Byte )
{
  *Byte >>= 1;
}
void KByteCodec::removeLastDummyDigit( unsigned char */*Byte*/ )
{
}
