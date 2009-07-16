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

#include "kbinarybytecodec.h"

// Qt
#include <QtCore/QString>


namespace Okteta {

void KBinaryByteCodec::encode( QString &Digits, unsigned int Pos, const unsigned char Char ) const
{
  for( unsigned char M=1<<7; M>0; M>>=1 )
    Digits[Pos++]/*.at(Pos++)*/ = (Char & M) ? '1' : '0';
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
    Digits[Pos++] = (Char & M) ? '1' : '0';
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

}
