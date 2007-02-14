/***************************************************************************
                          kfixedsizebytearraymodeltest.cpp  -  description
                            -------------------
    begin                : Son Mai 7 2006
    copyright            : (C) 2006 by Friedrich W. H. Kossebau
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
#include <QtTest>
// lib specific
#include <kfixedsizebytearraymodel.h>
// test specific
#include "kfixedsizebytearraymodeltest.h"

using namespace KHE;
using namespace KHECore;



// test the compare function
// uses a comparison buffer of the same size
// changes the first, a middle, and the last char to a greater or smaller char and compares
void KFixedSizeByteArrayModelTest::testCompare()
{
  int Size = 60;
  // create a working buffer
  KFixedSizeByteArrayModel Buffer( Size, 'b' );
  KFixedSizeByteArrayModel OtherBuffer( Size );
  OtherBuffer.fill( 'b' );

  int Last = Size-1;
  KSection FullRange(0,Last);
  // same size and equal
  QCOMPARE( Buffer.compare(OtherBuffer), 0 );
  // same size and last smaller
  Buffer.rawData()[Last] = 'c';
  QVERIFY( Buffer.compare(OtherBuffer) > 0 );
  // same size and last greater
  Buffer.rawData()[Last] = 'a';
  QVERIFY( Buffer.compare(OtherBuffer) < 0 );
  // same size and middle smaller
  Buffer.rawData()[Size/2] = 'c';
  QVERIFY( Buffer.compare(OtherBuffer) > 0 );
  // same size and middle greater
  Buffer.rawData()[Size/2] = 'a';
  QVERIFY( Buffer.compare(OtherBuffer) < 0 );
  // same size and first smaller
  Buffer.rawData()[0] = 'c';
  QVERIFY( Buffer.compare(OtherBuffer) > 0 );
  // same size and first greater
  Buffer.rawData()[0] = 'a';
  QVERIFY( Buffer.compare(OtherBuffer) < 0 );
}

/*
void KDataBufferIfTest::testCopy()
{
  // test copyTo TODO: for this we would need a compareTo
//  char[] TestArray = new char[OtherBuffer->size()];
//  OtherBuffer->set( 't' );
//  DataBuffwer
//  QVERIFY( "copy() from begin",  OtherBuffer->compare(Buffer,KSection( 0, CopySize, true ),0) == 0 );
//  delete [] TestArray;

  int Size = 60;
  int CopySize = 20;
  // create a working buffer
  KFixedSizeByteArrayModel Buffer( Size, 'b' );
  KFixedSizeByteArrayModel OtherBuffer( Size );
  OtherBuffer.fill( 'a' );

  int Last = Size-1;
  KSection FullRange(0,Last);

  // copy from
  Buffer.copy( OtherBuffer, 0, CopySize );
  QVERIFY( "copy() from begin",  OtherBuffer->compare(Buffer,0,CopySize,0) == 0 );

  Buffer.fill( '\b' );
  Buffer.copy( *OtherBuffer, TestSection );
  QVERIFY( "copy() from middle",  OtherBuffer->compare(Buffer,0,CopySize,TestSection.start()) == 0 );

  Buffer.fill( '\b' );
  Buffer.copy( *OtherBuffer, E, CopySize );
  QVERIFY( "copy() from before end",  OtherBuffer->compare(Buffer,0,CopySize,E) == 0 );

  // copy to
  Buffer.fill( '\b' );
  OtherBuffer->set( 't' );
  OtherBuffer->copy( Buffer, 0, CopySize );
  QVERIFY( "copy() to begin",  OtherBuffer->compare(Buffer,0,CopySize,0) == 0 );

  Buffer.fill( '\b' );
  OtherBuffer->set( 't' );
  OtherBuffer->copy( Buffer, 0, CopySize, TestSection.start() );
  QVERIFY( "copy() to middle",  OtherBuffer->compare(Buffer,0,CopySize,TestSection.start()) == 0 );

  Buffer.fill( '\b' );
  OtherBuffer->set( 't' );
  E = OtherBuffer->size() - CopySize;
  OtherBuffer->copy( Buffer, 0,CopySize, E );
  QVERIFY( "copy() to before end",  OtherBuffer->compare(Buffer,0,CopySize,E) == 0 );
}
*/


QTEST_MAIN( KFixedSizeByteArrayModelTest )

#include "kfixedsizebytearraymodeltest.moc"
