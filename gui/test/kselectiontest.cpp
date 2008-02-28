/*
    This file is part of the Okteta Gui library, part of the KDE project.

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

#include "kselectiontest.h"

// test object
#include <kselection.h>
// Qt
#include <QtTest/QtTest>

using namespace KHE;
using namespace KHEUI;

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
