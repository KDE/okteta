/***************************************************************************
                          kselectiontest.cpp -  description
                             -------------------
    begin                : Mon Mai 22 2006
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
// test specific
#include <kselection.h>
#include "kselectiontest.h"

using namespace KHE;

// local variables
static const int Start = 15;
static const int End = 27;

static const int Width = End - Start + 1;


void KSelectionTest::testPlainConstructor()
{
  KSelection Selection;
  QVERIFY( !Selection.isValid() );
  QVERIFY( !Selection.started() );
  QVERIFY( !Selection.justStarted() );
}

void KSelectionTest::testAnchorConstructor()
{
  KSelection Selection( Start );
  QCOMPARE( Selection.anchor(), Start );
  QVERIFY( !Selection.isValid() );
  QVERIFY( Selection.started() );
  QVERIFY( Selection.justStarted() );
}


void KSelectionTest::testSetStart()
{
  KSelection Selection;
  Selection.setStart( Start );
  QCOMPARE( Selection.anchor(), Start );
  QVERIFY( !Selection.isValid() );
  QVERIFY( Selection.started() );
  QVERIFY( Selection.justStarted() );
}


void KSelectionTest::testSetStartEnd()
{
  KSelection Selection;
  Selection.setStart( Start );
  Selection.setEnd( End );
  QCOMPARE( Selection.start(), Start );
  QCOMPARE( Selection.end(), End-1 );
  QCOMPARE( Selection.anchor(), Start );
  QVERIFY( Selection.isValid() );
  QVERIFY( Selection.started() );
  QVERIFY( !Selection.justStarted() );
  QVERIFY( Selection.isForward() );

  Selection.setStart( End );
  Selection.setEnd( Start );
  QCOMPARE( Selection.start(), Start );
  QCOMPARE( Selection.end(), End-1 );
  QCOMPARE( Selection.anchor(), End );
  QVERIFY( Selection.isValid() );
  QVERIFY( Selection.started() );
  QVERIFY( !Selection.justStarted() );
  QVERIFY( !Selection.isForward() );
}


void KSelectionTest::testCancel()
{
  KSelection Selection;
  Selection.setStart( Start );
  Selection.setEnd( End );
  Selection.cancel();
  QVERIFY( !Selection.isValid() );
  QVERIFY( !Selection.started() );
  QVERIFY( !Selection.justStarted() );
}


void KSelectionTest::testSetForward()
{
  KSelection Selection;
  Selection.setStart( Start );
  Selection.setEnd( End );
  Selection.setForward( false );
  QCOMPARE( Selection.start(), Start );
  QCOMPARE( Selection.end(), End-1 );
  QCOMPARE( Selection.anchor(), End );
  QVERIFY( Selection.isValid() );
  QVERIFY( Selection.started() );
  QVERIFY( !Selection.justStarted() );
  QVERIFY( !Selection.isForward() );

  Selection.setForward( true );
  QCOMPARE( Selection.start(), Start );
  QCOMPARE( Selection.end(), End-1 );
  QCOMPARE( Selection.anchor(), Start );
  QVERIFY( Selection.isValid() );
  QVERIFY( Selection.started() );
  QVERIFY( !Selection.justStarted() );
  QVERIFY( Selection.isForward() );
}


void KSelectionTest::testReverse()
{
  KSelection Selection;
  Selection.setStart( Start );
  Selection.setEnd( End );
  Selection.reverse();
  QCOMPARE( Selection.start(), Start );
  QCOMPARE( Selection.end(), End-1 );
  QCOMPARE( Selection.anchor(), End );
  QVERIFY( Selection.isValid() );
  QVERIFY( Selection.started() );
  QVERIFY( !Selection.justStarted() );
  QVERIFY( !Selection.isForward() );

  Selection.reverse();
  QCOMPARE( Selection.start(), Start );
  QCOMPARE( Selection.end(), End-1 );
  QCOMPARE( Selection.anchor(), Start );
  QVERIFY( Selection.isValid() );
  QVERIFY( Selection.started() );
  QVERIFY( !Selection.justStarted() );
  QVERIFY( Selection.isForward() );
}


QTEST_MAIN( KSelectionTest )

#include "kselectiontest.moc"
