/***************************************************************************
                          ktextcharcodec.cpp  -  description
                             -------------------
    begin                : Sa Nov 27 2004
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


#include "ktextcharcodec.h"

// lib
#include <khechar.h>
// KDE
#include <kglobal.h>
#include <klocale.h>
#include <kcharsets.h>
// Qt
#include <QtCore/QTextCodec>


namespace KHECore {

static const char QTextCodecWhiteSpace = 63;

static struct KEncodingNames {
  KEncoding Encoding;
  const char *Name;
}
const EncodingNames[] = {
{ ISO8859_1Encoding, "ISO-8859-1" },
{ ISO8859_2Encoding, "ISO-8859-2" },
{ ISO8859_3Encoding, "ISO-8859-3" },
{ ISO8859_4Encoding, "ISO-8859-4" },
{ ISO8859_5Encoding, "ISO-8859-5" },
{ ISO8859_6Encoding, "ISO-8859-6" },
{ ISO8859_7Encoding, "ISO-8859-7" },
{ ISO8859_8Encoding, "ISO-8859-8" },
{ ISO8859_8_IEncoding, "ISO-8859-8-I" },
{ ISO8859_9Encoding, "ISO-8859-9" },
{ ISO8859_11Encoding, "TIS-620" }, // was: ISO-8859-11
{ ISO8859_13Encoding, "ISO-8859-13" },
{ ISO8859_15Encoding, "ISO-8859-15" },
{ CP1250Encoding, "windows-1250" },
{ CP1251Encoding, "windows-1251" },
{ CP1252Encoding, "windows-1252" },
{ CP1253Encoding, "windows-1253" },
{ CP1254Encoding, "windows-1254" },
{ CP1255Encoding, "windows-1255" },
{ CP1256Encoding, "windows-1256" },
{ CP1257Encoding, "windows-1257" },
{ CP1258Encoding, "windows-1258" },
{ IBM850Encoding, "IBM850" },
{ IBM866Encoding, "IBM866" },
{ KOI8_REncoding, "KOI8-R" },
{ KOI8_UEncoding, "KOI8-U" } };
//TODO: WS2
static const unsigned int NoOfEncodings = sizeof(EncodingNames)/sizeof(struct KEncodingNames);// = 26;

static bool is8Bit( QTextCodec *Codec )
{
  const QByteArray &Name = Codec->name();
  bool Found = false;
  for( unsigned int i=0; i<NoOfEncodings; ++i )
  {
    if( qstrcmp(Name,EncodingNames[i].Name) == 0 )
    {
      Found = true;
      break;
    }
  }
  return Found;
}

static QTextCodec *createLatin1()
{
  return KGlobal::charsets()->codecForName( EncodingNames[0].Name );
}

/* heuristic seems to be doomed :(
static bool is8Bit( QTextCodec *Codec )
{
  bool Result = true;

  // first test different for 0
  unsigned char c[4];
  c[0] = 0;
  c[1] = c[2] = c[3] = 230;
  QString S = Codec->toUnicode( (const char*)&c,4 );
  int Length = 1;
  QCString CS = Codec->fromUnicode( S, Length );
  //kDebug() << Codec->name() << " "<<Length ;
  if( Length > 0 )
    Result = false;
  // test if all chars survive the recoding
  else
  do
  {
    ++c[0];
    S = Codec->toUnicode( (const char*)&c,4 );
    Length = 1;
    CS = Codec->fromUnicode( S, Length );
    //kDebug() << Codec->name() << " "<<c[0]<<"->"<<CS[0]<<":"<<Length ;
    if( Length != 1 || (CS[0] != (char)c[0] && CS[0] != QTextCodecWhiteSpace) )
    {
      Result = false;
      break;
    }
  }
  while( c[0] < 255 );
  return Result;
}
const QStringList &KTextCharCodec::codecNames()
{
  // first call?
  if( CodecNames.isEmpty() )
{
    const QStringList &CharSets = KGlobal::charsets()->availableEncodingNames();

    for( QStringList::ConstIterator it = CharSets.begin(); it != CharSets.end(); ++it )
{
      bool Found = true;
      QTextCodec* Codec = KGlobal::charsets()->codecForName( *it, Found );
      if( Found && is8Bit(Codec) )
        CodecNames.append( QString::fromLatin1(Codec->name()) );
}
}

  return CodecNames;
}

QString KTextCharCodec::nameOfEncoding( KEncoding C )
{
  KTextCharCodec *Codec = 0;

  const char* N = 0;
  for( unsigned int i=0; i<NoOfEncodings; ++i )
  {
    if( EncodingNames[i].Encoding == C )
    {
      N = EncodingNames[i].Name;
      break;
    }
  }

  if( N != 0 )
  {
    QString CodeName = QString::fromLatin1( N );
  }
  return Codec;
}
 */

KTextCharCodec *KTextCharCodec::createLocalCodec()
{
  QTextCodec *Codec = KGlobal::locale()->codecForEncoding();
  if( !is8Bit(Codec) )
    Codec = createLatin1();
  return new KTextCharCodec( Codec );
}


KTextCharCodec *KTextCharCodec::createCodec( const QString &CodeName )
{
  bool Ok;
  QTextCodec *Codec = KGlobal::charsets()->codecForName( CodeName, Ok );
  if( Ok )
    Ok = is8Bit( Codec );
  return Ok ? new KTextCharCodec( Codec ) : 0;
}


const QStringList &KTextCharCodec::codecNames()
{
  static QStringList TextCodecNames;

  // first call?
  if( TextCodecNames.isEmpty() )
  {
    for( unsigned int i=0; i<NoOfEncodings; ++i )
    {
      bool Found = true;
      QString Name = QString::fromLatin1( EncodingNames[i].Name );
      QTextCodec* Codec = KGlobal::charsets()->codecForName( Name, Found );
      if( Found )
        TextCodecNames.append( QString::fromLatin1(Codec->name()) );
    }
  }

  return TextCodecNames;
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

bool KTextCharCodec::canEncode( const QChar &C ) const
{
  return Codec->canEncode( C );
}

bool KTextCharCodec::encode( char *D, const QChar &C ) const
{
  if( !Codec->canEncode(C) ) // TODO: do we really need the codec?
    return false;
  QByteArray T = Encoder->fromUnicode( QString(C) );
  if (T.size()) {
    *D = T[0];
    return true;
  } else
    return false;
}


KChar KTextCharCodec::decode( char Byte ) const
{
  QString S( Decoder->toUnicode(&Byte,1) );
  return KChar(S.at(0));
}


const QString& KTextCharCodec::name() const
{
  if( Name.isNull() )
    Name = QString::fromLatin1( Codec->name() );
  return Name;
}

}
