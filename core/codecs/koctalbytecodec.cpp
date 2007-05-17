/***************************************************************************
                          koctalbytecodec.cpp  -  description
                             -------------------
    begin                : Mo Nov 29 2004
    copyright            : (C) 2004 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License version 2 as published by the Free Software Foundation.       *
 *                                                                         *
 ***************************************************************************/


#include "koctalbytecodec.h"

// Qt
#include <QtCore/QString>


namespace KHECore {

void KOctalByteCodec::encode( QString &Digits, unsigned int Pos, unsigned char Char ) const
{
  Digits[Pos++] = '0'+(Char>>6);
  Digits[Pos++] = '0'+((Char>>3)&0x07);
  Digits[Pos] = '0'+((Char)   &0x07);
}


void KOctalByteCodec::encodeShort( QString &Digits, unsigned int Pos, unsigned char Char ) const
{
  unsigned char C;
  if( (C = (Char>>6)&0x07) )
    Digits[Pos++] = '0'+C;
  if( (C = (Char>>3)&0x07) )
    Digits[Pos++] = '0'+C;
  Digits[Pos] = '0'+((Char)&0x07);
}


bool KOctalByteCodec::isValidDigit( unsigned char Digit ) const
{
  return Digit >= '0' && Digit <= '7';
}


bool KOctalByteCodec::turnToValue( unsigned char *Digit ) const
{
  if( isValidDigit(*Digit) )
  {
    *Digit -= '0';
    return true;
  }
  return false;
}


bool KOctalByteCodec::appendDigit( unsigned char *Byte, unsigned char Digit ) const
{
  if( turnToValue(&Digit) )
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


void KOctalByteCodec::removeLastDigit( unsigned char *Byte ) const
{
  *Byte >>= 3;
}

}
