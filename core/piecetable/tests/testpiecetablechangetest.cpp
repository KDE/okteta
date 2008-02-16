/***************************************************************************
                          testpiecetablechangetest.cpp  -  description
                             -------------------
    begin                : Mon Feb 11 2008
    copyright            : 2008 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License version 2 as published by the Free Software Foundation.       *
 *                                                                         *
 ***************************************************************************/


#include "testpiecetablechangetest.h"

// test object
#include "testpiecetablechange.h"
//
#include <piecetable/piecetable.h>
//
#include <ksectionlist.h>
// Qt
#include <QtTest/QtTest>

namespace KPieceTable
{

// local variables

void TestPieceTableChangeTest::testSimpleConstructor()
{
    const int typeId = 0;
    const QString description( "0" );
    TestPieceTableChange change( typeId, description );

    QCOMPARE( change.type(), typeId );
    QCOMPARE( change.description(), description );
}

void TestPieceTableChangeTest::testMerge()
{
    const int type0Id = 0;
    const QString description0( "0" );
    const int type1Id = 1;
    const QString description1( "1" );

    TestPieceTableChange change( type0Id, description0 );

    // merge with different
    TestPieceTableChange otherDifferentChange( type1Id, description1 );
    bool result = change.merge( &otherDifferentChange );

    QVERIFY( !result );
    QCOMPARE( change.type(), type0Id );
    QCOMPARE( change.description(), description0 );

    // merge with same
    TestPieceTableChange otherSameChange( type0Id, description1 );
    result = change.merge( &otherSameChange );

    QVERIFY( result );
    QCOMPARE( change.type(), type0Id );
    QCOMPARE( change.description(), description0+description1 );
}

}

QTEST_MAIN( KPieceTable::TestPieceTableChangeTest )
