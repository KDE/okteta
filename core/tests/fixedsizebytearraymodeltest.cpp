/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2006 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "fixedsizebytearraymodeltest.hpp"

// test object
#include <fixedsizebytearraymodel.hpp>
// Qt
#include <QTest>

namespace Okteta {

// test the compare function
// uses a comparison buffer of the same size
// changes the first, a middle, and the last char to a greater or smaller char and compares
void FixedSizeByteArrayModelTest::testCompare()
{
    const Size BaseSize = 60;
    // create a working buffer
    FixedSizeByteArrayModel Buffer(BaseSize, 'b');
    FixedSizeByteArrayModel OtherBuffer(BaseSize);
    OtherBuffer.fill('b');

    const Address Last = BaseSize - 1;
    AddressRange FullRange(0, Last);
    // same size and equal
    QCOMPARE(Buffer.compare(OtherBuffer), 0);
    // same size and last smaller
    Buffer.rawData()[Last] = 'c';
    QVERIFY(Buffer.compare(OtherBuffer) > 0);
    // same size and last greater
    Buffer.rawData()[Last] = 'a';
    QVERIFY(Buffer.compare(OtherBuffer) < 0);
    // same size and middle smaller
    Buffer.rawData()[BaseSize / 2] = 'c';
    QVERIFY(Buffer.compare(OtherBuffer) > 0);
    // same size and middle greater
    Buffer.rawData()[BaseSize / 2] = 'a';
    QVERIFY(Buffer.compare(OtherBuffer) < 0);
    // same size and first smaller
    Buffer.rawData()[0] = 'c';
    QVERIFY(Buffer.compare(OtherBuffer) > 0);
    // same size and first greater
    Buffer.rawData()[0] = 'a';
    QVERIFY(Buffer.compare(OtherBuffer) < 0);
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

QTEST_GUILESS_MAIN(Okteta::FixedSizeByteArrayModelTest)
