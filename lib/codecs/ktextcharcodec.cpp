/***************************************************************************
                          ktextcharcodec.cpp  -  description
                             -------------------
    begin                : Sa Nov 27 2004
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


// qt specific
#include "qtextcodec.h"
// kde specific
#include <kglobal.h>
#include <klocale.h>
#include <kcharsets.h>
// lib specific
#include "ktextcharcodec.h"


using namespace KHE;


KTextCharCodec *KTextCharCodec::createLocal()
{
  return new KTextCharCodec( KGlobal::locale()->codecForEncoding() );
}


KTextCharCodec *KTextCharCodec::create( const QString &CodeName )
{ 
  bool Ok;
  QTextCodec *Codec = KGlobal::charsets()->codecForName( CodeName, Ok );
  return Ok ? new KTextCharCodec( Codec ) : 0;
}


KTextCharCodec::KTextCharCodec( QTextCodec *C )
  : Codec( C ),
    Decoder( C->makeDecoder() ),
    Encoder( C->makeEncoder() )
{}

KTextCharCodec::~KTextCharCodec()
{
  delete Decoder;
  delete Encoder;
}

bool KTextCharCodec::encode( char *D, const QChar &C ) const
{
  if( !Codec->canEncode(C) ) // TODO: do we really need the codec?
    return false;
  int dummy;
  char T = Encoder->fromUnicode( C, dummy )[0];

  *D = T;
  return true;
}


KHEChar KTextCharCodec::decode( char Byte ) const
{
  return (KHEChar)Decoder->toUnicode( &Byte, 1 ).at(0);
}
