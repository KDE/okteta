/*
    This file is part of the Okteta Core library, made within the KDE community.

    Copyright 2006 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "fixedsizebytearraymodeltest.h"

// test object
#include <fixedsizebytearraymodel.h>
// Qt
#include <QtTest/QtTest>


namespace Okteta
{

// test the compare function
// uses a comparison buffer of the same size
// changes the first, a middle, and the last char to a greater or smaller char and compares
void FixedSizeByteArrayModelTest::testCompare()
{
  const Size BaseSize = 60;
  // create a working buffer
  FixedSizeByteArrayModel Buffer( BaseSize, 'b' );
  FixedSizeByteArrayModel OtherBuffer( BaseSize );
  OtherBuffer.fill( 'b' );

  const Address Last = BaseSize-1;
  AddressRange FullRange(0,Last);
  // same size and equal
  QCOMPARE( Buffer.compare(OtherBuffer), 0 );
  // same size and last smaller
  Buffer.rawData()[Last] = 'c';
  QVERIFY( Buffer.compare(OtherBuffer) > 0 );
  // same size and last greater
  Buffer.rawData()[Last] = 'a';
  QVERIFY( Buffer.compare(OtherBuffer) < 0 );
  // same size and middle smaller
  Buffer.rawData()[BaseSize/2] = 'c';
  QVERIFY( Buffer.compare(OtherBuffer) > 0 );
  // same size and middle greater
  Buffer.rawData()[BaseSize/2] = 'a';
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
//  QVERIFY( "copy() from begin",  OtherBuffer->compare(Buffer,AddressRange( 0, CopySize, true ),0) == 0 );
//  delete [] TestArray;

  int BaseSize = 60;
  int CopySize = 20;
  // create a working buffer
  FixedSizeByteArrayModel Buffer( BaseSize, 'b' );
  FixedSizeByteArrayModel OtherBuffer( BaseSize );
  OtherBuffer.fill( 'a' );

  int Last = BaseSize-1;
  AddressRange FullRange(0,Last);

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
}

QTEST_MAIN( Okteta::FixedSizeByteArrayModelTest )
