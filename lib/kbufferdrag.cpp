/***************************************************************************
                          kbufferdrag.cpp  -  description
                             -------------------
    begin                : Mon Jul 07 2003
    copyright            : (C) 2003 by Friedrich W. H. Kossebau
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
#include <qcstring.h>
#include <qtextcodec.h>
// kde specific
#include <kglobal.h>
#include <klocale.h>
// lib specific
#include "kbordercoltextexport.h"
#include "koffsetcoltextexport.h"
#include "kvaluecoltextexport.h"
#include "kcharcoltextexport.h"
#include "kcharcodec.h"
#include "kbufferdrag.h"


using namespace std;
using namespace KHE;

static const char OctetStream[] =        "application/octet-stream";
static const char TextPlainUTF8[] =      "text/plain;charset=UTF-8";
static const char TextPlain[] =          "text/plain";
static const char TextPlainLocalStub[] = "text/plain;charset=";

//static const char *BaseTypes[3] = { OctetStream, TextPlainUTF8, TextPlain };

// creates the name for the local text/plain
static const char *localTextPlain()
{
  static QCString TextPlainLocal;

  if( TextPlainLocal.isNull() )
  {
    TextPlainLocal = QCString(KGlobal::locale()->encoding()).lower();
    // remove the whitespaces
    int s;
    while( (s=TextPlainLocal.find(' ')) >= 0 )
      TextPlainLocal.remove( s, 1 );

    TextPlainLocal.prepend( TextPlainLocalStub );
  }

  return TextPlainLocal;
}

// tries to create a codec by the given charset description
static QTextCodec* codecForCharset( const QCString& Desc )
{
  int i = Desc.find( "charset=" );
  if( i >= 0 )
  {
    QCString CharSetName = Desc.mid( i+8 );
    // remove any further attributes
    if( (i=CharSetName.find( ';' )) >= 0 )
      CharSetName = CharSetName.left( i );

    // try to find codec
    return QTextCodec::codecForName( CharSetName );
  }
  // no charset=, use locale
  return KGlobal::locale()->codecForEncoding();
}



KBufferDrag::KBufferDrag( const QByteArray &D, KCoordRange Range,
                          const KOffsetColumn *OC, const KValueColumn *HC, const KCharColumn *TC,
                          QChar SC, QChar UC, const QString &CN,
                          QWidget *Source, const char *Name )
  :QDragObject( Source, Name ),
   CoordRange( Range ),
   NoOfCol( 0 ),
   SubstituteChar( SC ),
   UndefinedChar( UC ),
   CodecName( CN )
{
  setData( D );

  // print column wise?
  if( HC || TC )
  {
    if( OC )
    {
      Columns[NoOfCol++] = new KOffsetColTextExport( OC );
      Columns[NoOfCol++] = new KBorderColTextExport();
    }
    if( HC )
      Columns[NoOfCol++] = new KValueColTextExport( HC, Data.data(), CoordRange );
    if( TC )
    {
      if( HC ) Columns[NoOfCol++] = new KBorderColTextExport();
      Columns[NoOfCol++] = new KCharColTextExport( TC, Data.data(), CoordRange, CodecName );
    }
  }
}


KBufferDrag::~KBufferDrag()
{
  for( uint i=0; i<NoOfCol; ++i )
    delete Columns[i];
}



void KBufferDrag::setData( const QByteArray &D )
{
  Data = D;
}


const char *KBufferDrag::format( int i ) const
{
  return( i == 0 ? OctetStream :
          i == 1 ? TextPlainUTF8 :
          i == 2 ? TextPlain :
          i == 3 ? localTextPlain() :
                   0 );
}


QByteArray KBufferDrag::encodedData( const char *Format ) const
{
  if( Format != 0 )
  {
    // octet stream wanted?
    if( qstrcmp(Format,OctetStream) == 0 )
      return( Data );

    // plain text wanted?
    if( qstrncmp(Format,TextPlain,10) == 0 )
    {
      QCString Output;
      QTextCodec *TextCodec = codecForCharset( QCString(Format).lower() );
      if( TextCodec == 0 )
        return Output;

      QString Text;
      // plain copy?
      if( NoOfCol == 0 )
      {
        // duplicate the data and substitute all non-printable items with a space
        KCharCodec *CharCodec = KCharCodec::createCodec( CodecName );
        static const QChar Tab('\t');
        static const QChar Return('\n');
        uint Size = Data.size();
        Text.setLength( Size );

        for( uint i=0; i<Size; ++i )
        { 
          KHEChar B = CharCodec->decode( Data[i] );

          Text.at(i) = B.isUndefined() ? KHEChar(UndefinedChar) :
              (!B.isPrint() && B != Tab && B != Return ) ? KHEChar(SubstituteChar) : B;
        }
        // clean up
        delete CharCodec;
      }
      // formatted copy
      else
      {
        // initialize: one for the line's newline \n
        uint NeededChars = 1;
        for( uint i=0; i<NoOfCol; ++i )
          NeededChars += Columns[i]->charsPerLine();
        // scale with the number of lines
        NeededChars *= CoordRange.lines();
        // find out needed size
        Text.reserve( NeededChars );

        // now fill
        int l = CoordRange.start().line();
        for( uint i=0; i<NoOfCol; ++i )
          Columns[i]->printFirstLine( Text, l );
        Text.append('\n');
        for( ++l; l<=CoordRange.end().line(); ++l )
        {
          for( uint i=0; i<NoOfCol; ++i )
            Columns[i]->printNextLine( Text );
          Text.append('\n');
        }
      }
      // generate the ouput
      Output = TextCodec->fromUnicode( Text );
      // fix end
      //if( TextCodec->mibEnum() != 1000 )
      //{
        // Don't include NUL in size (QCString::resize() adds NUL)
      //  ((QByteArray&)Output).resize( Output.length() );
      //}
      return Output;
    }
  }

  // return empty dummy
  return QByteArray();
}



bool KBufferDrag::canDecode( const QMimeSource* Source )
{
  bool c =( Source->provides(OctetStream) /*|| Source->provides(TextPlain)*/ );
  return c;
//  return( Source->provides(OctetStream) /*|| Source->provides(TextPlain)*/ );
}


bool KBufferDrag::decode( const QMimeSource* Source, QByteArray &Dest )
{
//   Dest = Source->encodedData( MediaString );
//   return Dest.size() != 0;

  bool CanDecode = Source->provides( OctetStream );
  if( CanDecode )
    Dest = Source->encodedData( OctetStream );

  return CanDecode;
}

#include "kbufferdrag.moc"
