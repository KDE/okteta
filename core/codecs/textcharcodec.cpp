/*
    This file is part of the Okteta Core library, made within the KDE community.

    Copyright 2004,2011 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "textcharcodec.h"

// lib
#include <character.h>
// KDE
#include <kglobal.h>
#include <klocale.h>
#include <kcharsets.h>
// Qt
#include <QtCore/QTextCodec>


namespace Okteta
{

// static const char QTextCodecWhiteSpace = 63;

static struct EncodingData
{
    CharCoding encodingId;
    const char* name;
}
const encodingDataList[] =
{
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
    { ISO8859_14Encoding, "ISO-8859-14" },
    { ISO8859_15Encoding, "ISO-8859-15" },
    { ISO8859_16Encoding, "ISO-8859-16" },
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
    { IBM874Encoding, "IBM874" },
    { KOI8_REncoding, "KOI8-R" },
    { KOI8_UEncoding, "KOI8-U" }
};
//TODO: WS2
static const unsigned int encodingDataListSize =
    sizeof(encodingDataList)/sizeof(struct EncodingData);


static bool is8Bit( QTextCodec* codec )
{
    bool result = false;

    const QByteArray& codecName = codec->name();
    for( unsigned int i = 0; i < encodingDataListSize; ++i )
    {
        if( qstrcmp(codecName,encodingDataList[i].name) == 0 )
        {
            result = true;
            break;
        }
    }

    return result;
}

static QTextCodec* createLatin1()
{
    return KGlobal::charsets()->codecForName( QLatin1String(encodingDataList[0].name) );
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
const QStringList &TextCharCodec::codecNames()
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

QString TextCharCodec::nameOfEncoding( CharCoding _char )
{
  TextCharCodec *Codec = 0;

  const char* N = 0;
  for( unsigned int i=0; i<NoOfEncodings; ++i )
  {
    if( EncodingNames[i].Encoding == _char )
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

TextCharCodec* TextCharCodec::createLocalCodec()
{
    QTextCodec* codec = KGlobal::locale()->codecForEncoding();
    if( ! is8Bit(codec) )
        codec = createLatin1();
    return new TextCharCodec( codec );
}


TextCharCodec* TextCharCodec::createCodec( const QString& codecName )
{
    bool isOk = false;
    QTextCodec* codec = KGlobal::charsets()->codecForName( codecName, isOk );
    if( isOk )
        isOk = is8Bit( codec );
    return isOk ? new TextCharCodec( codec ) : 0;
}


const QStringList& TextCharCodec::codecNames()
{
    static QStringList textCodecNames;

    // first call?
    if( textCodecNames.isEmpty() )
    {
        KCharsets* charsets = KGlobal::charsets();
        for( unsigned int i = 0; i < encodingDataListSize; ++i )
        {
            bool isCodecFound = false;
            const QString codecName = QString::fromLatin1( encodingDataList[i].name );
            QTextCodec* codec = charsets->codecForName( codecName, isCodecFound );
            if( isCodecFound )
                textCodecNames.append( QString::fromLatin1(codec->name()) );
            }
    }

    return textCodecNames;
}


TextCharCodec::TextCharCodec( QTextCodec* textCodec )
  : mCodec( textCodec ),
    mDecoder( textCodec->makeDecoder() ),
    mEncoder( textCodec->makeEncoder() )
{
}

bool TextCharCodec::canEncode( const QChar& _char ) const
{
    return mCodec->canEncode( _char );
}

bool TextCharCodec::encode( Byte* byte, const QChar& _char ) const
{
    if( !mCodec->canEncode(_char) ) // TODO: do we really need the codec?
        return false;

    const QByteArray encoded = mEncoder->fromUnicode( QString(_char) );
    if( encoded.size() > 0 )
    {
        *byte = encoded.at( 0 );
        return true;
    } else
        return false;
}


Character TextCharCodec::decode( Byte byte ) const
{
    // QTextCodecs "use this codepoint when input data cannot be represented in Unicode." (Qt docs)
    static const QChar replacementChar = QChar( QChar::ReplacementCharacter );
    const QString string =
        mDecoder->toUnicode( reinterpret_cast<const char*>(&byte), 1 );
    const QChar qchar = string.at( 0 );
    const bool isDecoded = (qchar != replacementChar);
    return Character( qchar, ! isDecoded );
}


const QString& TextCharCodec::name() const
{
    if( mName.isNull() )
        mName = QString::fromLatin1( mCodec->name() );

    return mName;
}

TextCharCodec::~TextCharCodec()
{
    delete mDecoder;
    delete mEncoder;
}

}
