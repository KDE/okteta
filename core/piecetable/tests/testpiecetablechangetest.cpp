/*
    This file is part of the Okteta Core library, made within the KDE community.

    Copyright 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "testpiecetablechangetest.h"

// test object
#include "testpiecetablechange.h"
//
#include <piecetable/piecetable.h>
// Qt
#include <QtTest/QtTest>

namespace KPieceTable
{

// local variables

void TestPieceTableChangeTest::testSimpleConstructor()
{
    const int typeId = 0;
    const QString description = QLatin1String( "0" );
    TestPieceTableChange change( typeId, description );

    QCOMPARE( change.type(), typeId );
    QCOMPARE( change.description(), description );
}

void TestPieceTableChangeTest::testMerge()
{
    const int type0Id = 0;
    const QString description0 = QLatin1String( "0" );
    const int type1Id = 1;
    const QString description1 = QLatin1String( "1" );

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
    QCOMPARE( change.description(), QString(description0+description1) );
}

}

QTEST_MAIN( KPieceTable::TestPieceTableChangeTest )
