/***************************************************************************
                          testplainbufferdatabufferif.cpp  -  description
                            -------------------
    begin                : Sam Mai 1 2004
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


// lib specific
#include <kplainbuffer.h>
// app specific
#include "testplainbufferdatabufferif.h"


using namespace KHE;


static const int PlainBufferSize = 60;

// all test imply that the buffer ca
// readonly 
KPlainBufferDataBufferIfTest::KPlainBufferDataBufferIfTest()
 : KDataBufferIfTest( "KPlainBufferDataBufferIfTest" )
{
  TestSection.set( 10,40 );

  KPlainBuffer *PlainBuffer = new KPlainBuffer( PlainBufferSize );
  char *Data = PlainBuffer->data();
  for( char i=0; i<PlainBufferSize; ++i )
    Data[i] = i+1;
  PlainBuffer->setReadOnly( false );

  DataBuffer = PlainBuffer;
}


KPlainBufferDataBufferIfTest::~KPlainBufferDataBufferIfTest()
{
  delete DataBuffer;
}

void KPlainBufferDataBufferIfTest::setup()
{
}

void KPlainBufferDataBufferIfTest::tearDown()
{
}


#include "testplainbufferdatabufferif.moc"
