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

 
// c specific
#include <string.h>
// c++ speciofic
#include <iostream>
// app specific
#include "kbufferdrag.h"

using namespace KHE;

const char *KBufferDrag::OctetStream = "application/octet-stream";
const char *KBufferDrag::PlainText = "text/plain";

KBufferDrag::KBufferDrag( const QByteArray &D, QWidget *Source, const char *Name )
  :QDragObject( Source, Name )
{
  setData( D );
}


KBufferDrag::KBufferDrag( QWidget *Source, const char *Name )
 : QDragObject( Source, Name )
{
}


KBufferDrag::~KBufferDrag()
{
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
      TextData.duplicate( Data );
      char *D = TextData.data();
      for( unsigned int i=0; i<TextData.size(); ++i,++D )
      { char B = *D; if( B < 32 && B != '\t' && B != '\n' ) *D = ' '; }
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
