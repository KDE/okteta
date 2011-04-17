/*
    This file is part of the Okteta Gui library, made within the KDE community.

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

#include "selectiontest.h"

// test object
#include <selection.h>
// Qt
#include <QtTest/QtTest>


namespace Okteta
{

// local variables
static const Address Start = 15;
static const Address End = 27;

static const Size Width = End - Start + 1;


void SelectionTest::testPlainConstructor()
{
    const Selection selection;
    QVERIFY( !selection.isValid() );
    QVERIFY( !selection.started() );
    QVERIFY( !selection.justStarted() );
}

void SelectionTest::testAnchorConstructor()
{
    const Selection selection( Start );
    QCOMPARE( selection.anchor(), Start );
    QVERIFY( !selection.isValid() );
    QVERIFY( selection.started() );
    QVERIFY( selection.justStarted() );
}


void SelectionTest::testSetStart()
{
    Selection selection;
    selection.setStart( Start );
    QCOMPARE( selection.anchor(), Start );
    QVERIFY( !selection.isValid() );
    QVERIFY( selection.started() );
    QVERIFY( selection.justStarted() );
}


void SelectionTest::testSetStartEnd()
{
    Selection selection;
    selection.setStart( Start );
    selection.setEnd( End );
    QCOMPARE( selection.start(), Start );
    QCOMPARE( selection.end(), End-1 );
    QCOMPARE( selection.anchor(), Start );
    QVERIFY( selection.isValid() );
    QVERIFY( selection.started() );
    QVERIFY( !selection.justStarted() );
    QVERIFY( selection.isForward() );

    selection.setStart( End );
    selection.setEnd( Start );
    QCOMPARE( selection.start(), Start );
    QCOMPARE( selection.end(), End-1 );
    QCOMPARE( selection.anchor(), End );
    QVERIFY( selection.isValid() );
    QVERIFY( selection.started() );
    QVERIFY( !selection.justStarted() );
    QVERIFY( !selection.isForward() );
}


void SelectionTest::testCancel()
{
    Selection selection;
    selection.setStart( Start );
    selection.setEnd( End );
    selection.cancel();
    QVERIFY( !selection.isValid() );
    QVERIFY( !selection.started() );
    QVERIFY( !selection.justStarted() );
}


void SelectionTest::testSetForward()
{
    Selection selection;
    selection.setStart( Start );
    selection.setEnd( End );
    selection.setForward( false );
    QCOMPARE( selection.start(), Start );
    QCOMPARE( selection.end(), End-1 );
    QCOMPARE( selection.anchor(), End );
    QVERIFY( selection.isValid() );
    QVERIFY( selection.started() );
    QVERIFY( !selection.justStarted() );
    QVERIFY( !selection.isForward() );

    selection.setForward( true );
    QCOMPARE( selection.start(), Start );
    QCOMPARE( selection.end(), End-1 );
    QCOMPARE( selection.anchor(), Start );
    QVERIFY( selection.isValid() );
    QVERIFY( selection.started() );
    QVERIFY( !selection.justStarted() );
    QVERIFY( selection.isForward() );
}


void SelectionTest::testReverse()
{
    Selection selection;
    selection.setStart( Start );
    selection.setEnd( End );
    selection.reverse();
    QCOMPARE( selection.start(), Start );
    QCOMPARE( selection.end(), End-1 );
    QCOMPARE( selection.anchor(), End );
    QVERIFY( selection.isValid() );
    QVERIFY( selection.started() );
    QVERIFY( !selection.justStarted() );
    QVERIFY( !selection.isForward() );

    selection.reverse();
    QCOMPARE( selection.start(), Start );
    QCOMPARE( selection.end(), End-1 );
    QCOMPARE( selection.anchor(), Start );
    QVERIFY( selection.isValid() );
    QVERIFY( selection.started() );
    QVERIFY( !selection.justStarted() );
    QVERIFY( selection.isForward() );
}

}

QTEST_MAIN( Okteta::SelectionTest )
