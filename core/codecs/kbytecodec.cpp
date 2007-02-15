/***************************************************************************
                          kbytecodec.cpp  -  description
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


// Qt
#include <QtCore/QString>
// lib specific
#include "kbinarybytecodec.h"
#include "koctalbytecodec.h"
#include "kdecimalbytecodec.h"
#include "khexadecimalbytecodec.h"


namespace KHECore {

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

unsigned int KByteCodec::decode( unsigned char *Char, const QString &Digits, unsigned int Pos ) const
{
  //kDebug() << QString("KByteCodec::decode(%1,%2)").arg(Digits).arg(Pos) << endl;
  const unsigned int OldPos = Pos;
  const unsigned int Left = Digits.size() - Pos;

  unsigned int d = encodingWidth();
  if( Left < d )
    d = Left;

  unsigned char Result = 0;
  do
  {
    if( !appendDigit(&Result,Digits.at(Pos).toLatin1()) )
      break;

    ++Pos;
    --d;
  }
  while( d > 0 );

  *Char = Result;
  return Pos - OldPos;
}

}
