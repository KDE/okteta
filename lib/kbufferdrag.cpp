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

#include <iostream>
using namespace std;
// c specific
#include <string.h>
// c++ speciofic
#include <iostream>
// app specific
#include "kbufferdrag.h"
#include "kbordercoltextexport.h"
#include "koffsetcoltextexport.h"
#include "kvaluecoltextexport.h"
#include "kcharcoltextexport.h"

using namespace KHE;

const char *KBufferDrag::OctetStream = "application/octet-stream";
const char *KBufferDrag::PlainText =   "text/plain";

KBufferDrag::KBufferDrag( const QByteArray &D, KCoordRange Range,
                          const KOffsetColumn *OC, const KValueColumn *HC, const KCharColumn *TC,
                          char SC,
                          QWidget *Source, const char *Name )
  :QDragObject( Source, Name ),
   CoordRange( Range ),
   SubstituteChar( SC )
{
  setData( D );

  Columns = new KColTextExportPtr[5];
  NoOfCol = 0;
  if( HC )
  {
    Columns[NoOfCol++] = new KOffsetColTextExport( OC );
    Columns[NoOfCol++] = new KBorderColTextExport();
    Columns[NoOfCol++] = new KValueColTextExport( HC, Data.data(), CoordRange );
    if( TC )
    {
      Columns[NoOfCol++] = new KBorderColTextExport();
      Columns[NoOfCol++] = new KCharColTextExport( TC, Data.data(), CoordRange );
    }
  }
}


KBufferDrag::KBufferDrag( QWidget *Source, const char *Name )
 : QDragObject( Source, Name )
{
}


KBufferDrag::~KBufferDrag()
{
  for( int i=0; i<NoOfCol; ++i )
    delete Columns[i];
  delete [] Columns;
}


void KBufferDrag::setData( const QByteArray &D )
{
  Data = D;
}


const char *KBufferDrag::format( int i ) const
{
  return( i == 0 ? OctetStream : i == 1 ? PlainText : 0L );
}


QByteArray KBufferDrag::encodedData( const char *Format ) const
{
  if( Format != 0L )
  {
    if( strcmp(Format,OctetStream) == 0 )
    {
//       std::cout << "using " << OctetStream << std::endl;
      return( Data );
    }
    if( strcmp(Format,PlainText) == 0 )
    {
//       std::cout << "using " << PlainText << std::endl;
      QByteArray TextData;
      if( NoOfCol == 0 )
      {
        // duplicate the data and subsitute all non-printable items with a space
        TextData.duplicate( Data );
        char *D = TextData.data();
        for( unsigned int i=0; i<TextData.size(); ++i,++D )
        { unsigned char B = *D; if( B < 32 && B != '\t' && B != '\n' ) *D = SubstituteChar; }
      }
      else
      {
        // initialize: one for the newline \n
        int NeededMemory = 1;
        for( int i=0; i<NoOfCol; ++i )
        {
          NeededMemory += Columns[i]->charsPerLine();
//           cout << "MEM:" << NeededMemory << endl;
        }
        // scale with the number of lines
        NeededMemory *= CoordRange.lines();
//         cout << "FMEM:" << NeededMemory << endl;
        // find out needed size
        /*if( !*/TextData.resize( NeededMemory );// )
//           cout <<"Duh"<<endl;
//         cout << "A" << endl;
        // now fill
        char *D = TextData.data();
        int l = CoordRange.start().line();
//         cout << "startline: "<<l<<endl;
        for( int i=0; i<NoOfCol; ++i )
          Columns[i]->printFirstLine( &D, l );
        *D++ = '\n';
//         cout << "firstline done"<<endl;
        for( ++l; l<=CoordRange.end().line(); ++l )
        {
          for( int i=0; i<NoOfCol; ++i )
            Columns[i]->printNextLine( &D );
          *D++ = '\n';
//           cout << "line"<<l<<" done"<<endl;
        }
//         cout << "done. should:"<<TextData.size()<< " is:"<<D-TextData.data()<<endl;
      }
      return( TextData );
    }
  }

  // return empty dummy
  return QByteArray();
}



bool KBufferDrag::canDecode( const QMimeSource* Source )
{
  return( Source->provides(OctetStream) /*|| Source->provides(PlainText)*/ );
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
