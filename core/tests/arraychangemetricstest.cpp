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

#include "arraychangemetricstest.h"

// test object
#include <arraychangemetrics.h>
// Qt
#include <QtTest/QtTest>

namespace Okteta
{

// local variables
static const Address Offset = 15;

static const Size LengthDiff = 5;
static const Size InsertLength = 15;
static const Size BiggerRemoveLength = InsertLength + LengthDiff;
static const Size SameRemoveLength = InsertLength;
static const Size SmallerRemoveLength = InsertLength - LengthDiff;

static const Size FirstLength = 15;
static const Address SecondOffset = Offset + FirstLength;
static const Size SecondLength = 10;
static const Address SecondEnd = SecondOffset + SecondLength - 1;


void ArrayChangeMetricsTest::testConstructorAsReplacement()
{
    ArrayChangeMetrics change = ArrayChangeMetrics::asReplacement( Offset, SmallerRemoveLength, InsertLength );

    QVERIFY( change.isValid() );
    QCOMPARE( change.offset(), Offset );
    QCOMPARE( change.removeLength(), SmallerRemoveLength );
    QCOMPARE( change.insertLength(), InsertLength );
    QCOMPARE( change.lengthChange(), LengthDiff );
}

void ArrayChangeMetricsTest::testConstructorAsSwapping()
{
    ArrayChangeMetrics change = ArrayChangeMetrics::asSwapping( Offset, SecondOffset, SecondLength );

    QVERIFY( change.isValid() );
    QCOMPARE( change.offset(), Offset );
    QCOMPARE( change.firstLength(), FirstLength );
    QCOMPARE( change.secondStart(), SecondOffset );
    QCOMPARE( change.secondEnd(), SecondEnd );
    QCOMPARE( change.secondLength(), SecondLength );
}

void ArrayChangeMetricsTest::testRevert()
{
    ArrayChangeMetrics replacement = ArrayChangeMetrics::asReplacement( Offset, SmallerRemoveLength, InsertLength );
    replacement.revert();

    QVERIFY( replacement.isValid() );
    QCOMPARE( replacement.offset(), Offset );
    QCOMPARE( replacement.removeLength(), InsertLength );
    QCOMPARE( replacement.insertLength(), SmallerRemoveLength );
    QCOMPARE( replacement.lengthChange(), -LengthDiff );

    replacement.revert();

    QVERIFY( replacement.isValid() );
    QCOMPARE( replacement.offset(), Offset );
    QCOMPARE( replacement.removeLength(), SmallerRemoveLength );
    QCOMPARE( replacement.insertLength(), InsertLength );
    QCOMPARE( replacement.lengthChange(), LengthDiff );


    ArrayChangeMetrics swapping = ArrayChangeMetrics::asSwapping( Offset, SecondOffset, SecondLength );
    swapping.revert();

    QVERIFY( swapping.isValid() );
    QCOMPARE( swapping.offset(), Offset );
    QCOMPARE( swapping.firstLength(), SecondLength );
    QCOMPARE( swapping.secondStart(), Offset+SecondLength );
    QCOMPARE( swapping.secondEnd(), SecondEnd );
    QCOMPARE( swapping.secondLength(), FirstLength );

    swapping.revert();

    QVERIFY( swapping.isValid() );
    QCOMPARE( swapping.offset(), Offset );
    QCOMPARE( swapping.firstLength(), FirstLength );
    QCOMPARE( swapping.secondStart(), SecondOffset );
    QCOMPARE( swapping.secondEnd(), SecondEnd );
    QCOMPARE( swapping.secondLength(), SecondLength );
}

}

QTEST_MAIN( Okteta::ArrayChangeMetricsTest )
