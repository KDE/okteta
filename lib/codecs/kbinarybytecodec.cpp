/***************************************************************************
                          kbinarybytecodec.cpp  -  description
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
#include "kbinarybytecodec.h"

using namespace KHE;


void KBinaryByteCodec::encode( QString &Digits, unsigned int Pos, const unsigned char Char ) const
{
  for( unsigned char M=1<<7; M>0; M>>=1 )
    Digits.at(Pos++) = (Char & M) ? '1' : '0';
}

void KBinaryByteCodec::encodeShort( QString &Digits, unsigned int Pos, unsigned char Char ) const
{
  unsigned char M = 1<<7;
  // find first set bit
  for( ; M>0; M>>=1 )
    if( Char & M )
      break;
  // now set the
  for( ; M>0; M>>=1 )
    Digits.at(Pos++) = (Char & M) ? '1' : '0';
}


bool KBinaryByteCodec::isValidDigit( unsigned char Digit ) const
{
  return Digit == '0' || Digit == '1';
}


bool KBinaryByteCodec::turnToValue( unsigned char *Digit ) const
{
  if( isValidDigit(*Digit) )
  {
    *Digit -= '0';
    return true;
  }
  return false;
}


bool KBinaryByteCodec::appendDigit( unsigned char *Byte, unsigned char Digit ) const
{
  if( turnToValue(&Digit) )
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


void KBinaryByteCodec::removeLastDigit( unsigned char *Byte ) const
{
  *Byte >>= 1;
}
