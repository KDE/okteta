/***************************************************************************
                          ktextcharcodectest.cpp  -  description
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
// kde specific
#include <kcomponentdata.h>
// lib specific
#include <codecs/ktextcharcodec.h>
// test specific
#include "ktextcharcodectest.h"


using namespace KHECore;


void KTextCharCodecTest::initTestCase()
{
  Instance = new KComponentData( "KTextCharCodecTest" );
}

void KTextCharCodecTest::cleanupTestCase()
{
  delete Instance;
}

//---------------------------------------------------------------------------- Tests -----

void KTextCharCodecTest::testCreateLocalCodec()
{
  KTextCharCodec* Codec = KTextCharCodec::createLocalCodec();
  QVERIFY( Codec != 0 );
  delete Codec;
}


void KTextCharCodecTest::testCreateCodec()
{
  // create ISO8859-1
  KTextCharCodec* Codec = KTextCharCodec::createCodec( "ISO8859-1" );
  QVERIFY( Codec != 0 );
  delete Codec;
  // create KOI8-R
  Codec = KTextCharCodec::createCodec( "KOI8-R" );
  QVERIFY( Codec != 0 );
  delete Codec;
  // create ISO8859-15
  Codec = KTextCharCodec::createCodec( "ISO8859-15" );
  QVERIFY( Codec != 0 );
  delete Codec;
  // create utf8
  Codec = KTextCharCodec::createCodec( "utf8" );
  QVERIFY( Codec == 0 );
  // create Latin1
  Codec = KTextCharCodec::createCodec( "NonexistingCode" );
  QVERIFY( Codec == 0 );
}


QTEST_MAIN( KTextCharCodecTest )

#include "ktextcharcodectest.moc"
