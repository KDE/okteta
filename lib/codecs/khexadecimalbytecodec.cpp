/***************************************************************************
                          khexadecimalbytecodec.cpp  -  description
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


// lib specific
#include "khexadecimalbytecodec.h"

using namespace KHE;


static const QChar BigDigit[16] =
{ '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F' };
static const QChar SmallDigit[16] =
{ '0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f' };


KHexadecimalByteCodec::KHexadecimalByteCodec( bool S ) : Digit( S?SmallDigit:BigDigit ) {}

bool KHexadecimalByteCodec::setSmallDigits( bool S )
{
  bool Change = ( S && Digit == BigDigit );
  Digit = S?SmallDigit:BigDigit;
  return Change;
}

bool KHexadecimalByteCodec::smallDigits() const { return Digit != BigDigit; }


void KHexadecimalByteCodec::encode( QString &Digits, unsigned int Pos, unsigned char Char ) const
{
  Digits.at(Pos++) = Digit[Char>>4];
  Digits.at(Pos) = Digit[Char&0x0F];
}

void KHexadecimalByteCodec::encodeShort( QString &Digits, unsigned int Pos, unsigned char Char ) const
{
  unsigned char C;
  if( (C = (Char>>4)) )
    Digits.at(Pos++) = Digit[C];
  Digits.at(Pos) = Digit[Char&0x0F];
}


static inline bool isValidBigDigit( unsigned char Digit )
{
  return (Digit >= 'A' && Digit <= 'F');
}

static inline bool isValidSmallDigit( unsigned char Digit )
{
  return (Digit >= 'a' && Digit <= 'f');
}

static inline bool isValidDecimalDigit( unsigned char Digit )
{
  return Digit >= '0' && Digit <= '9';
}


bool KHexadecimalByteCodec::isValidDigit( unsigned char Digit ) const
{
  return isValidDecimalDigit(Digit) || isValidBigDigit(Digit) || isValidSmallDigit(Digit);
}

bool KHexadecimalByteCodec::turnToValue( unsigned char *Digit ) const
{
  if( isValidDecimalDigit(*Digit) )
    *Digit -= '0';
  else if( isValidBigDigit(*Digit) )
    *Digit -= 'A' - 10;
  else if( isValidSmallDigit(*Digit) )
    *Digit -= 'a' - 10;
  else
    return false;

  return true;
}

bool KHexadecimalByteCodec::appendDigit( unsigned char *Byte, unsigned char Digit ) const
{
  if( turnToValue(&Digit) )
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



void KHexadecimalByteCodec::removeLastDigit( unsigned char *Byte ) const
{
  *Byte >>= 4;
}
