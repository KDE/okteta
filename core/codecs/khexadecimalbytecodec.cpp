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

#include "khexadecimalbytecodec.h"

// Qt
#include <QtCore/QString>


namespace Okteta {

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
  Digits[Pos++] = Digit[Char>>4];
  Digits[Pos] = Digit[Char&0x0F];
}

void KHexadecimalByteCodec::encodeShort( QString &Digits, unsigned int Pos, unsigned char Char ) const
{
  unsigned char C;
  if( (C = (Char>>4)) )
    Digits[Pos++] = Digit[C];
  Digits[Pos] = Digit[Char&0x0F];
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

}
