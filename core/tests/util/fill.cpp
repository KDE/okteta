/***************************************************************************
                          fill.cpp  -  description
                            -------------------
    begin                : Wed Mai 30 2007
    copyright            : 2007 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
***************************************************************************/

/***************************************************************************
*                                                                         *
*   This library is free software; you can redistribute it and/or         *
*   modify it under the terms of the GNU Library General Public           *
*   License version 2 as published by the Free Software Foundation.       *
*                                                                         *
***************************************************************************/


#include "fill.h"

// 
#include <kabstractbytearraymodel.h>
//
#include <ksection.h>
// Qt
#include <QtCore/QByteArray>


using namespace KHE;
using namespace KHECore;


// fills the buffer with char from b to e
void textureByteArrayModel( KAbstractByteArrayModel *byteArrayModel,
                            unsigned char b, unsigned char e,
                            unsigned int From, int To )
{
    const int size = byteArrayModel->size();

    // check
    if( To == -1 || To >= size )
      To = size-1;
    if( b > e )
      b = e;

    // do
    unsigned char c = b;
    for( int i=From; i<=To; ++i )
    {
        byteArrayModel->setDatum(i, c );
        if( c == e )
            c = b;
        else
            c++;
    }
}


void textureByteArrayModel( KAbstractByteArrayModel *byteArrayModel,
                            unsigned char b, unsigned char e,
                            const KSection &section )
{
    textureByteArrayModel( byteArrayModel, b, e, section.start(), section.end() );
}


void textureByteArray( QByteArray *byteArray,
                       unsigned char b, unsigned char e,
                       unsigned int From, int To )
{
    const int size = byteArray->size();

    // check
    if( To == -1 || To >= size )
      To = size-1;
    if( b > e )
      b = e;

    // do
    unsigned char c = b;
    for( int i=From; i<=To; ++i )
    {
        (*byteArray)[i] = c;
        if( c == e )
            c = b;
        else
            c++;
    }
}


void textureByteArray( QByteArray *byteArray,
                       unsigned char b, unsigned char e,
                       const KSection &section )
{
    textureByteArray( byteArray, b, e, section.start(), section.end() );
}

/*
static void list( KAbstractByteArrayModel *byteArrayModel, const char* Name )
{
  unsigned int size = byteArrayModel->size();
  for( unsigned int i=0; i<size; ++i )
  {
    kdDebug() << Name<<":"<<i<<":"<<byteArrayModel->datum(i) << endl;
  }
}
*/
