/***************************************************************************
                          testfixedsizebuffer.cpp  -  description
                            -------------------
    begin                : Son Mai 2 2004
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


// kde specific
#include <kde/test/testrunner.h>
// test specific
#include <kfixedsizebuffer.h>
#include "testfixedsizebufferdatabufferif.h"
#include "testfixedsizebuffer.h"

using namespace KDE::Test;
using namespace KHE;


// all test imply that the buffer ca
// readonly 
KFixedSizeBufferTest::KFixedSizeBufferTest()
 : SlotTest( "KFixedSizeBufferTest" )
{
}

KFixedSizeBufferTest::~KFixedSizeBufferTest()
{
}

// test the compare function
// uses a comparison buffer of the same size
// changes the first, a middle, and the last char to a greater or smaller char and compares
void KFixedSizeBufferTest::testCompare()
{
  int Size = 60;
  // create a working buffer
  KFixedSizeBuffer Buffer( Size, 'b' );
  KFixedSizeBuffer OtherBuffer( Size );
  OtherBuffer.fill( 'b' );

  int Last = Size-1;
  KSection FullRange(0,Last);
  //
  KT_ASSERT_EQUALS( "compare() same size and equal",  Buffer.compare(OtherBuffer), 0 );

  Buffer.rawData()[Last] = 'c';
  KT_ASSERT( "compare() same size and last smaller",  Buffer.compare(OtherBuffer) > 0 );

  Buffer.rawData()[Last] = 'a';
  KT_ASSERT( "compare() same size and last greater",  Buffer.compare(OtherBuffer) < 0 );

  Buffer.rawData()[Size/2] = 'c';
  KT_ASSERT( "compare() same size and middle smaller",  Buffer.compare(OtherBuffer) > 0 );

  Buffer.rawData()[Size/2] = 'a';
  KT_ASSERT( "compare() same size and middle greater",  Buffer.compare(OtherBuffer) < 0 );

  Buffer.rawData()[0] = 'c';
  KT_ASSERT( "compare() same size and first smaller",  Buffer.compare(OtherBuffer) > 0 );

  Buffer.rawData()[0] = 'a';
  KT_ASSERT( "compare() same size and first greater",  Buffer.compare(OtherBuffer) < 0 );
}

/*
void KDataBufferIfTest::testCopy()
{
  // test copyTo TODO: for this we would need a compareTo
//  char[] TestArray = new char[OtherBuffer->size()];
//  OtherBuffer->set( 't' );
//  DataBuffwer
//  KT_ASSERT( "copy() from begin",  OtherBuffer->compare(Buffer,KSection( 0, CopySize, true ),0) == 0 );
//  delete [] TestArray;

  int Size = 60;
  int CopySize = 20;
  // create a working buffer
  KFixedSizeBuffer Buffer( Size, 'b' );
  KFixedSizeBuffer OtherBuffer( Size );
  OtherBuffer.fill( 'a' );

  int Last = Size-1;
  KSection FullRange(0,Last);

  // copy from
  Buffer.copy( OtherBuffer, 0, CopySize );
  KT_ASSERT( "copy() from begin",  OtherBuffer->compare(Buffer,0,CopySize,0) == 0 );

  Buffer.fill( '\b' );
  Buffer.copy( *OtherBuffer, TestSection );
  KT_ASSERT( "copy() from middle",  OtherBuffer->compare(Buffer,0,CopySize,TestSection.start()) == 0 );

  Buffer.fill( '\b' );
  Buffer.copy( *OtherBuffer, E, CopySize );
  KT_ASSERT( "copy() from before end",  OtherBuffer->compare(Buffer,0,CopySize,E) == 0 );

  // copy to
  Buffer.fill( '\b' );
  OtherBuffer->set( 't' );
  OtherBuffer->copy( Buffer, 0, CopySize );
  KT_ASSERT( "copy() to begin",  OtherBuffer->compare(Buffer,0,CopySize,0) == 0 );

  Buffer.fill( '\b' );
  OtherBuffer->set( 't' );
  OtherBuffer->copy( Buffer, 0, CopySize, TestSection.start() );
  KT_ASSERT( "copy() to middle",  OtherBuffer->compare(Buffer,0,CopySize,TestSection.start()) == 0 );

  Buffer.fill( '\b' );
  OtherBuffer->set( 't' );
  E = OtherBuffer->size() - CopySize;
  OtherBuffer->copy( Buffer, 0,CopySize, E );
  KT_ASSERT( "copy() to before end",  OtherBuffer->compare(Buffer,0,CopySize,E) == 0 );
}
*/

int main( int, char** )
{
  KFixedSizeBufferTest FixedSizeBufferTest;
  KFixedSizeBufferDataBufferIfTest DataBufferIfTest;

  TestRunner Runner;
  Runner.add( &FixedSizeBufferTest );
  Runner.add( &DataBufferIfTest );

  return Runner.run();
}

#include "testfixedsizebuffer.moc"
