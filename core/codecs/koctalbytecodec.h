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

#ifndef KHE_CORE_KOCTALBYTECODEC_H
#define KHE_CORE_KOCTALBYTECODEC_H

// lib
#include <valuecodec.h>


namespace KHECore
{

class KOctalByteCodec : public ValueCodec
{
  public:
    virtual ~KOctalByteCodec() {}

  public: // ValueCodec API
    virtual unsigned int encodingWidth() const { return 3; }
    virtual unsigned char digitsFilledLimit() const { return 64; }

    virtual void encode( QString &Digits, unsigned  int Pos, const unsigned char Char ) const;
    virtual void encodeShort( QString &Digits, unsigned  int Pos, const unsigned char Char ) const;
    virtual bool appendDigit( unsigned char *Byte, const unsigned char Digit ) const;
    virtual void removeLastDigit( unsigned char *Byte ) const;
    virtual bool isValidDigit( const unsigned char Digit ) const;
    virtual bool turnToValue( unsigned char *Digit ) const;
};

}

#endif
