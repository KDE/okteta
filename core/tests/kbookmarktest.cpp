/***************************************************************************
                          kbookmarktest.cpp  -  description
                             -------------------
    begin                : Mon Dec 3 2007
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


#include "kbookmarktest.h"

// test object
#include <kbookmark.h>
// Qt
#include <QtTest/QtTest>

using namespace KHECore;


static const int Offset1 = 7;
static const int Offset2 = 23;

void KBookmarkTest::testSimpleConstructor()
{
  KBookmark bookmark;

  QVERIFY( !bookmark.isValid() );
}

void KBookmarkTest::testConstructor()
{
  KBookmark bookmark( Offset1 );

  QVERIFY( bookmark.isValid() );
  QCOMPARE( bookmark.offset(), Offset1 );
}

void KBookmarkTest::testCompare()
{
  KBookmark bookmark1( Offset1 );
  KBookmark bookmark2( Offset1 );
  KBookmark bookmark3( Offset2 );

  QVERIFY( bookmark1 == bookmark2 );
  QVERIFY( !(bookmark1 == bookmark3) );
}

void KBookmarkTest::testMove()
{
  KBookmark bookmark( Offset1 );
  bookmark.move( Offset2 );

  QVERIFY( bookmark.isValid() );
  QCOMPARE( bookmark.offset(), Offset1+Offset2 );
}


QTEST_MAIN( KBookmarkTest )
