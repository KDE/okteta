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

static const char QTextCodecWhiteSpace = 63;

static QTextCodec *createLatin1()
{
  return KGlobal::charsets()->codecForName( "ISO8859-1" );
}

static bool is8Bit( QTextCodec *Codec )
{
  for( unsigned int c=1; c<256; ++c )
  {
    unsigned char A = (unsigned char)c;
    QString S = Codec->toUnicode( (const char*)&A,1 );
    int Length = 1;
    QCString CS = Codec->fromUnicode( S, Length );
    if( Length == 0 || (CS[0] != (char)c && CS[0] != QTextCodecWhiteSpace) )
      return false;
  }
  return true;
}


KTextCharCodec *KTextCharCodec::createLocal()
{
  QTextCodec *Codec = KGlobal::locale()->codecForEncoding();
  if( !is8Bit(Codec) )
    Codec = createLatin1();
  return new KTextCharCodec( Codec );
}


KTextCharCodec *KTextCharCodec::create( const QString &CodeName )
{ 
  bool Ok;
  QTextCodec *Codec = KGlobal::charsets()->codecForName( CodeName, Ok );
  if( Ok )
    Ok = is8Bit( Codec );
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
