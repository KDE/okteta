/*
    This file is part of the Okteta Core library, part of the KDE project.

    Copyright 2004 Friedrich W. H. Kossebau <kossebau@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library. If not, see <http://www.gnu.org/licenses/>.
*/

#include "koctalbytecodec.h"

// Qt
#include <QtCore/QString>


namespace Okteta {

void KOctalByteCodec::encode( QString &Digits, unsigned int Pos, Byte Char ) const
{
  Digits[Pos++] = '0'+(Char>>6);
  Digits[Pos++] = '0'+((Char>>3)&0x07);
  Digits[Pos] = '0'+((Char)   &0x07);
}


void KOctalByteCodec::encodeShort( QString &Digits, unsigned int Pos, Byte Char ) const
{
  unsigned char C;
  if( (C = (Char>>6)&0x07) )
    Digits[Pos++] = '0'+C;
  unsigned char C2;
  if( (C2 = (Char>>3)&0x07) || C )
    Digits[Pos++] = '0'+C2;
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
    if( B < 32 )
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
