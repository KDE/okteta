/***************************************************************************
                          testcharcodec.cpp  -  description
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
#include <kcharcodec.h>
#include "testcharcodec.h"

// namespaces
using namespace KDE::Test;
using namespace KHE;

// local variables
static const int Start = 3;
static const int End = 15;

static const int Width = End - Start + 1;


KCharCodecTest::KCharCodecTest()
  :  SlotTest( "KCharCodecTest" ) // TODO: replace this by using the object->className()
{
  Instance = new KInstance("KTextCharCodecTest");
}

KCharCodecTest::~KCharCodecTest()
{
  delete Instance;
}

//---------------------------------------------------------------------------- Tests -----


void KCharCodecTest::testCodecNames()
{
  QStringList List( KCharCodec::codecNames() );

  for( unsigned int i=0; i<List.size(); ++i )
  {
    const QString Name( List[i] );
    KCharCodec* Codec = KCharCodec::createCodec( Name );
    KT_ASSERT_EQUALS( "name exists", Codec->name(), Name );
    delete Codec;
  }
}


int main( int, char** )
{
  KCharCodecTest TextCharCodecTest;
  TestRunner Runner;
  Runner.add( &TextCharCodecTest );

  return Runner.run();
}


#include "testcharcodec.moc"
