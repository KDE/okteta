/***************************************************************************
                          kbytecodec.cpp  -  description
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
#include "koctalbytecodec.h"
#include "kdecimalbytecodec.h"
#include "khexadecimalbytecodec.h"

using namespace KHE;


KByteCodec *KByteCodec::createCodec( KCoding C )
{
  KByteCodec *Codec;
  switch( C )
  {
    case DecimalCoding: Codec = new KDecimalByteCodec(); break;
    case OctalCoding: Codec = new KOctalByteCodec(); break;
    case BinaryCoding: Codec = new KBinaryByteCodec(); break;
    case HexadecimalCoding:
    default:  Codec = new KHexadecimalByteCodec();
  }
  return Codec;
}

unsigned int KByteCodec::decode( unsigned char *Char, const QString &Digits, uint Pos ) const
{
  //kdDebug() << QString("KByteCodec::decode(%1,%2)").arg(Digits).arg(Pos) << endl;
  const uint P = Pos;

  // remove leading 0s
  while( Digits.at(Pos) == '0' ) { ++Pos; }

  unsigned char C = 0;
  unsigned int d = encodingWidth();
  do
  {
    if( !appendDigit(&C,Digits.at(Pos)) )
      break;

    ++Pos;
    --d;
  }
  while( d > 0 );

  *Char = C;
  return Pos - P;
}
