/***************************************************************************
                          kdecimalbytecodec.cpp  -  description
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
#include "kdecimalbytecodec.h"

using namespace KHE;


void KDecimalByteCodec::encode( QString &Digits, unsigned int Pos, unsigned char Char ) const
{
  unsigned char C = Char / 100;
  Digits.at(Pos++) = '0'+C;
  Char -= C * 100;
  C = Char / 10;
  Digits.at(Pos++) = '0'+C;
  Char -= C * 10;
  Digits.at(Pos) = '0'+Char;
}


void KDecimalByteCodec::encodeShort( QString &Digits, unsigned int Pos, unsigned char Char ) const
{
  unsigned char C;
  if( (C = Char / 100) )
  {
    Digits.at(Pos++) = '0'+C;
    Char -= C * 100;
  }
  if( (C = Char / 10) )
  {
    Digits.at(Pos++) = '0'+C;
    Char -= C * 10;
  }
  Digits.at(Pos) = '0'+Char;
}



bool KDecimalByteCodec::isValidDigit( unsigned char Digit ) const
{
  return Digit >= '0' && Digit <= '9';
}

bool KDecimalByteCodec::turnToValue( unsigned char *Digit ) const
{
  if( isValidDigit(*Digit) )
  {
    *Digit -= '0';
    return true;
  }
  return false;
}


bool KDecimalByteCodec::appendDigit( unsigned char *Byte, unsigned char Digit ) const
{
  if( turnToValue(&Digit) )
  {
    unsigned char B = *Byte;
    if( B < 26 )
    {
      B *= 10;
      if( Digit <= 255-B )
      {
        B += Digit;
        *Byte = B;
        return true;
      }
    }
  }
  return false;
}

void KDecimalByteCodec::removeLastDigit( unsigned char *Byte ) const
{
  *Byte /= 10;
}
