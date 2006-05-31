/***************************************************************************
                          kcharcodectest.cpp  -  description
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


// qt specific
#include <QtTest>
// kde specific
#include <kinstance.h>
// lib specific
#include <kcharcodec.h>
// test specific
#include "kcharcodectest.h"

// namespaces
using namespace KHECore;


void KCharCodecTest::initTestCase()
{
  Instance = new KInstance( "KTextCharCodecTest" );
}

void KCharCodecTest::cleanupTestCase()
{
  delete Instance;
}

//---------------------------------------------------------------------------- Tests -----


void KCharCodecTest::testCodecNames()
{
  QStringList List( KCharCodec::codecNames() );

  for( int i=0; i<List.size(); ++i )
  {
    // name exists
    const QString &Name = List[i];
    KCharCodec* Codec = KCharCodec::createCodec( Name );
    QCOMPARE( Codec->name(), Name );
    delete Codec;
  }
}


QTEST_MAIN( KCharCodecTest )

#include "kcharcodectest.moc"
