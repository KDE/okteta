/***************************************************************************
                          testtextcharcodec.cpp  -  description
                            -------------------
    begin                : Sa Jan 15 2005
    copyright            : (C) 2005 by Friedrich W. H. Kossebau
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
#include <kde/test/slottest.h>
#include <kde/test/testrunner.h>
#include <kinstance.h>
// test specific
#include <codecs/ktextcharcodec.h>
#include "testtextcharcodec.h"

// namespaces
using namespace KDE::Test;
using namespace KHE;

// local variables
static const int Start = 3;
static const int End = 15;

static const int Width = End - Start + 1;


KTextCharCodecTest::KTextCharCodecTest()
  :  SlotTest( "KTextCharCodecTest" ) // TODO: replace this by using the object->className()
{
  Instance = new KInstance("KTextCharCodecTest");
}

KTextCharCodecTest::~KTextCharCodecTest()
{
  delete Instance;
}

//---------------------------------------------------------------------------- Tests -----

void KTextCharCodecTest::testCreateLocal()
{
  KTextCharCodec* Codec = KTextCharCodec::createLocal();
  KT_ASSERT( "createLocal", Codec != 0 );
  delete Codec;
}


void KTextCharCodecTest::testCreate()
{
  KTextCharCodec* Codec = KTextCharCodec::create( "ISO8859-1" );
  KT_ASSERT( "create ISO8859-1", Codec != 0 );
  delete Codec;

  Codec = KTextCharCodec::create( "KOI8-R" );
  KT_ASSERT( "create KOI8-R", Codec != 0 );
  delete Codec;

  Codec = KTextCharCodec::create( "ISO8859-15" );
  KT_ASSERT( "create ISO8859-15", Codec != 0 );
  delete Codec;

  Codec = KTextCharCodec::create( "utf8" );
  KT_ASSERT( "create utf8", Codec == 0 );

  Codec = KTextCharCodec::create( "NonexistingCode" );
  KT_ASSERT( "create Latin1", Codec == 0 );
}


int main( int, char** )
{
  KTextCharCodecTest t;
  return TestRunner( &t ).run();
}


#include "testtextcharcodec.moc"
