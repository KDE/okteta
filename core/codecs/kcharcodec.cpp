/***************************************************************************
                          kcharcodec.cpp  -  description
                             -------------------
    begin                : Do Nov 25 2004
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


#include "kcharcodec.h"

// lib
#include "ktextcharcodec.h"
#include "kebcdic1047charcodec.h"
// Qt
#include <QtCore/QStringList>


namespace KHECore {


const QStringList &KCharCodec::codecNames()
{
  static QStringList CodecNames;
  // first call?
  if( CodecNames.isEmpty() )
  {
    CodecNames = KTextCharCodec::codecNames();
    CodecNames.append( KEBCDIC1047CharCodec::codecName() );
  }

  return CodecNames;
}


KCharCodec *KCharCodec::createCodec( const QString &Name )
{
  KCharCodec *Codec = 0;

  if( KTextCharCodec::codecNames().indexOf(Name) != -1 )
    Codec = KTextCharCodec::createCodec( Name );
  else if( KEBCDIC1047CharCodec::codecName() == Name )
    Codec = KEBCDIC1047CharCodec::create();

  // ensure at least a codec
  if( Codec == 0 )
    Codec = KTextCharCodec::createLocalCodec();

  return Codec;
}


KCharCodec *KCharCodec::createCodec( KEncoding C )
{
  KCharCodec *Codec;
  if( C == EBCDIC1047Encoding )
    Codec = KEBCDIC1047CharCodec::create();
  else if( C == ISO8859_1Encoding )
    Codec = KTextCharCodec::createCodec( "ISO-8859-1" );
  // LocalEncoding
  else
    Codec = 0;

  // ensure at least a codec
  if( Codec == 0 )
    Codec = KTextCharCodec::createLocalCodec();

  return Codec;
}

}
