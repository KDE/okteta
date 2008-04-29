/*
    This file is part of the Okteta Gui library, part of the KDE project.

    Copyright 2003 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "kbytearraydrag.h"

// lib
#include "kbordercoltextexport.h"
#include "koffsetcoltextexport.h"
#include "kvaluecoltextexport.h"
#include "kcharcoltextexport.h"
#include "kcharcodec.h"
// Okteta core
#include <khechar.h>
// KDE
#include <kglobal.h>
#include <klocale.h>
// Qt
#include <QtCore/QTextCodec>


using namespace std;

namespace KHEUI {

static const char OctetStream[] =        "application/octet-stream";
static const char TextPlainUTF8[] =      "text/plain;charset=UTF-8";
static const char TextPlain[] =          "text/plain";
//static const char TextPlainLocalStub[] = "text/plain;charset=";

//static const char *BaseTypes[3] = { OctetStream, TextPlainUTF8, TextPlain };

/*
// creates the name for the local text/plain
static const char *localTextPlain()
{
  static QByteArray TextPlainLocal;

  if( TextPlainLocal.isNull() )
  {
    TextPlainLocal = QByteArray(KGlobal::locale()->encoding()).toLower();
    // remove the whitespaces
    int s;
    while( (s=TextPlainLocal.find(' ')) >= 0 )
      TextPlainLocal.remove( s, 1 );

    TextPlainLocal.prepend( TextPlainLocalStub );
  }

  return TextPlainLocal;
}

// tries to create a codec by the given charset description
static QTextCodec* codecForCharset( const QByteArray& Desc )
{
  int i = Desc.indexOf( "charset=" );
  if( i >= 0 )
  {
    QByteArray CharSetName = Desc.mid( i+8 );
    // remove any further attributes
    int indexOfFurther = CharSetName.indexOf( ';' );
    if( indexOfFurther >= 0 )
      CharSetName = CharSetName.left( indexOfFurther );

    // try to find codec
    return QTextCodec::codecForName( CharSetName );
  }
  // no charset=, use locale
  return KGlobal::locale()->codecForEncoding();
}
*/


KByteArrayDrag::KByteArrayDrag( const QByteArray &D, const KCoordRange &Range,
                          const OffsetColumnRenderer *OC, const ValueByteArrayColumnRenderer *HC, const CharByteArrayColumnRenderer *TC,
                          QChar SC, QChar UC, const QString &CN )
  :BufferCopy( D ),
   CoordRange( Range ),
   NoOfCol( 0 ),
   SubstituteChar( SC ),
   UndefinedChar( UC ),
   CodecName( CN )
{
  //setData( "application/octet-stream", D );

  // print column wise?
  if( HC || TC )
  {
    if( OC )
    {
      Columns[NoOfCol++] = new KOffsetColTextExport( OC );
      Columns[NoOfCol++] = new KBorderColTextExport();
    }
    if( HC )
      Columns[NoOfCol++] = new KValueColTextExport( HC, BufferCopy.data(), CoordRange );
    if( TC )
    {
      if( HC ) Columns[NoOfCol++] = new KBorderColTextExport();
      Columns[NoOfCol++] = new KCharColTextExport( TC, BufferCopy.data(), CoordRange, CodecName );
    }
  }
}


KByteArrayDrag::~KByteArrayDrag()
{
  for( uint i=0; i<NoOfCol; ++i )
    delete Columns[i];
}


QStringList KByteArrayDrag::formats() const
{
  QStringList list;
  list += QLatin1String( TextPlainUTF8 );
  list += QLatin1String( TextPlain );
  list += QLatin1String( OctetStream );
  //list += QLatin1String( localTextPlain() );
  return list;
}


QString KByteArrayDrag::createTextCopy() const
{
  QString Result;
  // duplicate the data and substitute all non-printable items with a space
  KHECore::KCharCodec *CharCodec = KHECore::KCharCodec::createCodec( CodecName );
  static const QChar Tab('\t');
  static const QChar Return('\n');
  uint Size = BufferCopy.size();
  Result.resize( Size );

  for( uint i=0; i<Size; ++i )
  {
    KHECore::KChar B = CharCodec->decode( BufferCopy[i] );

    Result[i] = B.isUndefined() ? KHECore::KChar(UndefinedChar) :
        (!B.isPrint() && B != Tab && B != Return ) ? KHECore::KChar(SubstituteChar) : B;
  }
  // clean up
  delete CharCodec;

  return Result;
}


QString KByteArrayDrag::createColumnCopy() const
{
  QString Result;
  // initialize: one for the line's newline \n
  uint NeededChars = 1;
  for( uint i=0; i<NoOfCol; ++i )
    NeededChars += Columns[i]->charsPerLine();
  // scale with the number of lines
  NeededChars *= CoordRange.lines();
  // find out needed size
  Result.reserve( NeededChars );

  // now fill
  int l = CoordRange.start().line();
  for( uint i=0; i<NoOfCol; ++i )
    Columns[i]->printFirstLine( &Result, l );
  Result.append('\n');
  for( ++l; l<=CoordRange.end().line(); ++l )
  {
    for( uint i=0; i<NoOfCol; ++i )
      Columns[i]->printNextLine( &Result );
    Result.append( '\n' );
  }
  return Result;
}


QVariant KByteArrayDrag::retrieveData( const QString &mimetype, QVariant::Type type ) const
{
  // octet stream wanted?
  if( mimetype==QLatin1String(OctetStream) && type==QVariant::ByteArray )
    return QVariant( BufferCopy );

  // plain text wanted?
  if( mimetype==QLatin1String(TextPlain) && type==QVariant::String )
    return (NoOfCol==0) ? createTextCopy() : createColumnCopy();

  // return empty dummy
  return QVariant(type);
}

}
