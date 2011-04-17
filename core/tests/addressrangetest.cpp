/*
    This file is part of the Okteta Core library, made within the KDE community.

    Copyright 2006,2008 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "addressrangetest.h"

// test object
#include <addressrange.h>
// Qt
#include <QtTest/QtTest>


// TODO: find out how to test a template class with Qt/KDE,
//  so e.g. NumberRange can be tested with more than AddressRange
namespace Okteta
{

// local variables
static const Address Start = 15;
static const Address End = 27;

static const Size Width = End - Start + 1;


void AddressRangeTest::testConstructorByEnd()
{
    AddressRange addressRange( Start, End );
    QCOMPARE( addressRange.start(), Start );
    QCOMPARE( addressRange.end(),   End );
    QCOMPARE( addressRange.width(), Width );
}

void AddressRangeTest::testConstructorByWidth()
{
    AddressRange addressRange = AddressRange::fromWidth( Start, Width );

    QCOMPARE( addressRange.start(), Start );
    QCOMPARE( addressRange.end(), End );
    QCOMPARE( addressRange.width(), Width );

    addressRange = AddressRange::fromWidth( Width );

    QCOMPARE( addressRange.start(), 0 );
    QCOMPARE( addressRange.end(), Width-1 );
    QCOMPARE( addressRange.width(), Width );
}

void AddressRangeTest::testSimpleConstructor()
{
    AddressRange addressRange;
    QVERIFY( addressRange.isEmpty() );
}

void AddressRangeTest::testCopyConstructor()
{
    const AddressRange otherAddressRange( Start, End );

    AddressRange addressRange( otherAddressRange );
    QCOMPARE( addressRange.start(), Start );
    QCOMPARE( addressRange.end(),   End );
    QCOMPARE( addressRange.width(), Width );
    QVERIFY( !addressRange.isEmpty() );
}


void AddressRangeTest::testSetGetStart()
{
    AddressRange addressRange;
    addressRange.setStart( Start );
    QCOMPARE( addressRange.start(), Start );
}

void AddressRangeTest::testSetGetEnd()
{
    AddressRange addressRange;
    addressRange.setEnd( End );
    QCOMPARE( addressRange.end(), End );
}

void AddressRangeTest::testSetGetWidth()
{
    AddressRange addressRange;
    // end, width by width
    addressRange.setStart( Start );
    addressRange.setEndByWidth( Width );
    QCOMPARE( addressRange.end(), End );
    QCOMPARE( addressRange.width(), Width );
    // start, width by width
    addressRange.setEnd( End );
    addressRange.setStartByWidth( Width );
    QCOMPARE( addressRange.start(), Start );
    QCOMPARE( addressRange.width(), Width );
}

void AddressRangeTest::testIsEmpty()
{
    AddressRange addressRange( Start, End );
    QVERIFY( !addressRange.isEmpty() );
    addressRange.unset();
    QVERIFY( addressRange.isEmpty() );
}

void AddressRangeTest::testCompare()
{
    AddressRange addressRange( Start, End );
    // same
    AddressRange otherAddressRange( Start, End );
    QVERIFY( addressRange == otherAddressRange );
    // different start
    otherAddressRange.set(Start+1, End);
    QVERIFY( !(addressRange == otherAddressRange) );
    // different end
    otherAddressRange.set(Start, End+1);
    QVERIFY( !(addressRange == otherAddressRange) );
}

void AddressRangeTest::testIncludes()
{
    AddressRange addressRange( Start, End );
    // at start
    Address i = addressRange.start();
    QVERIFY( addressRange.includes(i) );
    // before start
    --i;
    QVERIFY( !addressRange.includes(i) );
    // at end
    i = addressRange.end();
    QVERIFY( addressRange.includes(i) );
    // behind end
    ++i;
    QVERIFY( !addressRange.includes(i) );
}


void AddressRangeTest::testLocalIndex()
{
    AddressRange addressRange( Start, End );

    Address localIndex = addressRange.localIndex( Start );
    QCOMPARE( localIndex, 0 );

    localIndex = addressRange.localIndex( End );
    QCOMPARE( localIndex, Width-1 );
}


void AddressRangeTest::testLocalRange()
{
    AddressRange addressRange( Start, End );

    AddressRange localRange = addressRange.localRange( AddressRange(Start,End) );
    QCOMPARE( localRange.start(), 0 );
    QCOMPARE( localRange.end(), Width-1 );

    localRange = addressRange.localRange( AddressRange(Start+1,End) );
    QCOMPARE( localRange.start(), 1 );
    QCOMPARE( localRange.end(), Width-1 );

    localRange = addressRange.localRange( AddressRange(Start,End-1) );
    QCOMPARE( localRange.start(), 0 );
    QCOMPARE( localRange.end(), Width-2 );

    localRange = addressRange.localRange( AddressRange(Start+1,End-1) );
    QCOMPARE( localRange.start(), 1 );
    QCOMPARE( localRange.end(), Width-2 );
}


void AddressRangeTest::testSubRange()
{
    AddressRange addressRange( Start, End );

    AddressRange subRange = addressRange.subRange( AddressRange::fromWidth(Width) );
    QCOMPARE( subRange.start(), Start );
    QCOMPARE( subRange.end(), End );

    subRange = addressRange.subRange( AddressRange(1,Width-1) );
    QCOMPARE( subRange.start(), Start+1 );
    QCOMPARE( subRange.end(), End );

    subRange = addressRange.subRange( AddressRange(0,Width-2) );
    QCOMPARE( subRange.start(), Start );
    QCOMPARE( subRange.end(), End-1 );

    subRange = addressRange.subRange( AddressRange(1,Width-2) );
    QCOMPARE( subRange.start(), Start+1 );
    QCOMPARE( subRange.end(), End-1 );
}


void AddressRangeTest::testAdaptToReplacement()
{
    // adaptToReplacement, same length, behind
    AddressRange addressRange( Start, End );
    AddressRange otherAddressRange( addressRange );
    addressRange.adaptToReplacement( End+1, Width, Width );
    QCOMPARE( addressRange, otherAddressRange );

    // adaptToReplacement, same length, before
    addressRange.set( Start, End );
    otherAddressRange.set( addressRange );
    addressRange.adaptToReplacement( Start-2, 1, 1 );
    QCOMPARE( addressRange, otherAddressRange );

    // adaptToReplacement, same length, -1,-1
    addressRange.set( Start-1, End-1 );
    otherAddressRange.set( Start-1, Start-1 );
    addressRange.adaptToReplacement( Start, Width, Width );
    QCOMPARE( addressRange, otherAddressRange );

    // adaptToReplacement, same length, -1,0
    addressRange.set( Start-1, End );
    otherAddressRange.set( addressRange );
    addressRange.adaptToReplacement( Start, Width, Width );
    QCOMPARE( addressRange, otherAddressRange );

    // adaptToReplacement, same length, -1,+1
    addressRange.set( Start-1, End+1 );
    otherAddressRange.set( addressRange );
    addressRange.adaptToReplacement( Start, Width, Width );
    QCOMPARE( addressRange, otherAddressRange );

    // adaptToReplacement, same length, 0,-1
    addressRange.set( Start, End-1 );
    otherAddressRange.set( addressRange );
    addressRange.adaptToReplacement( Start, Width, Width );
    QVERIFY( !addressRange.isValid() );

    // adaptToReplacement, same length, 0,0
    addressRange.set( Start, End );
    otherAddressRange.set( addressRange );
    addressRange.adaptToReplacement( Start, Width, Width );
    QCOMPARE( addressRange, otherAddressRange );

    // adaptToReplacement, same length, 0,+1
    addressRange.set( Start, End+1 );
    otherAddressRange.set( addressRange );
    addressRange.adaptToReplacement( Start, Width, Width );
    QCOMPARE( addressRange, otherAddressRange );

    // adaptToReplacement, same length, +1,-1
    addressRange.set( Start+1, End-1 );
    addressRange.adaptToReplacement( Start, Width, Width );
    QVERIFY( !addressRange.isValid() );

    // adaptToReplacement, same length, +1,0
    addressRange.set( Start+1, End );
    addressRange.adaptToReplacement( Start, Width, Width );
    QVERIFY( !addressRange.isValid() );

    // adaptToReplacement, same length, +1,+1
    addressRange.set( Start+1, End+1 );
    otherAddressRange.set( End+1, End+1 );
    addressRange.adaptToReplacement( Start, Width, Width );
    QCOMPARE( addressRange, otherAddressRange );


    // adaptToReplacement, less length, behind
    addressRange.set( Start, End );
    otherAddressRange.set( addressRange );
    addressRange.adaptToReplacement( End+1, Width, Width-2 );
    QCOMPARE( addressRange, otherAddressRange );

    // adaptToReplacement, less length, before
    addressRange.set( Start, End );
    otherAddressRange.set( Start-2, End-2 );
    addressRange.adaptToReplacement( Start-Width, Width, Width-2 );
    QCOMPARE( addressRange, otherAddressRange );

    // adaptToReplacement, less length, -1,-1
    addressRange.set( Start-1, End-1 );
    otherAddressRange.set( Start-1, Start-1 );
    addressRange.adaptToReplacement( Start, Width, Width-2 );
    QCOMPARE( addressRange, otherAddressRange );

    // adaptToReplacement, less length, -1,0
    addressRange.set( Start-1, End );
    otherAddressRange.set( Start-1, End-2 );
    addressRange.adaptToReplacement( Start, Width, Width-2 );
    QCOMPARE( addressRange, otherAddressRange );

    // adaptToReplacement, less length, -1,+1
    addressRange.set( Start-1, End+1 );
    otherAddressRange.set( Start-1, End-1 );
    addressRange.adaptToReplacement( Start, Width, Width-2 );
    QCOMPARE( addressRange, otherAddressRange );

    // adaptToReplacement, less length, 0,-1
    addressRange.set( Start, End-1 );
    addressRange.adaptToReplacement( Start, Width, Width-2 );
    QVERIFY( !addressRange.isValid() );

    // adaptToReplacement, less length, 0,0
    addressRange.set( Start, End );
    otherAddressRange.set( Start, End-2 );
    addressRange.adaptToReplacement( Start, Width, Width-2 );
    QCOMPARE( addressRange, otherAddressRange );

    // adaptToReplacement, less length, 0,+1
    addressRange.set( Start, End+1 );
    otherAddressRange.set( Start, End-1 );
    addressRange.adaptToReplacement( Start, Width, Width-2 );
    QCOMPARE( addressRange, otherAddressRange );

    // adaptToReplacement, less length, +1,-1
    addressRange.set( Start+1, End-1 );
    addressRange.adaptToReplacement( Start, Width, Width-2 );
    QVERIFY( !addressRange.isValid() );

    // adaptToReplacement, less length, +1,0
    addressRange.set( Start+1, End );
    addressRange.adaptToReplacement( Start, Width, Width-2 );
    QVERIFY( !addressRange.isValid() );

    // adaptToReplacement, less length, +1,+1
    addressRange.set( Start+1, End+1 );
    otherAddressRange.set( End-1, End-1 );
    addressRange.adaptToReplacement( Start, Width, Width-2 );
    QCOMPARE( addressRange, otherAddressRange );


    // adaptToReplacement, greater length, behind
    addressRange.set( Start, End );
    otherAddressRange.set( addressRange );
    addressRange.adaptToReplacement( End+1, Width, Width+2 );
    QCOMPARE( addressRange, otherAddressRange );

    // adaptToReplacement, greater length, before
    addressRange.set( Start, End );
    otherAddressRange.set( Start+2, End+2 );
    addressRange.adaptToReplacement( Start-Width, Width, Width+2 );
    QCOMPARE( addressRange, otherAddressRange );

    // adaptToReplacement, greater length, -1,-1
    addressRange.set( Start-1, End-1 );
    otherAddressRange.set( Start-1, Start-1 );
    addressRange.adaptToReplacement( Start, Width, Width+2 );
    QCOMPARE( addressRange, otherAddressRange );

    // adaptToReplacement, greater length, -1,0
    addressRange.set( Start-1, End );
    otherAddressRange.set( Start-1, End+2 );
    addressRange.adaptToReplacement( Start, Width, Width+2 );
    QCOMPARE( addressRange, otherAddressRange );

    // adaptToReplacement, greater length, -1,+1
    addressRange.set( Start-1, End+1 );
    otherAddressRange.set( Start-1, End+3 );
    addressRange.adaptToReplacement( Start, Width, Width+2 );
    QCOMPARE( addressRange, otherAddressRange );

    // adaptToReplacement, greater length, 0,-1
    addressRange.set( Start, End-1 );
    addressRange.adaptToReplacement( Start, Width, Width+2 );
    QVERIFY( !addressRange.isValid() );

    // adaptToReplacement, greater length, 0,0
    addressRange.set( Start, End );
    otherAddressRange.set( Start, End+2 );
    addressRange.adaptToReplacement( Start, Width, Width+2 );
    QCOMPARE( addressRange, otherAddressRange );

    // adaptToReplacement, greater length, 0,+1
    addressRange.set( Start, End+1 );
    otherAddressRange.set( Start, End+3 );
    addressRange.adaptToReplacement( Start, Width, Width+2 );
    QCOMPARE( addressRange, otherAddressRange );

    // adaptToReplacement, greater length, +1,-1
    addressRange.set( Start+1, End-1 );
    addressRange.adaptToReplacement( Start, Width, Width+2 );
    QVERIFY( !addressRange.isValid() );

    // adaptToReplacement, greater length, +1,0
    addressRange.set( Start+1, End );
    addressRange.adaptToReplacement( Start, Width, Width+2 );
    QVERIFY( !addressRange.isValid() );

    // adaptToReplacement, greater length, +1,+1
    addressRange.set( Start+1, End+1 );
    otherAddressRange.set( End+3, End+3 );
    addressRange.adaptToReplacement( Start, Width, Width+2 );
    QCOMPARE( addressRange, otherAddressRange );


    // adaptToReplacement, insert, behind
    addressRange.set( Start-1, Start-1 );
    otherAddressRange.set( Start-1, End );
    addressRange.adaptToReplacement( Start, 0, Width );
    QCOMPARE( addressRange, otherAddressRange );

    // adaptToReplacement, insert, middle
    addressRange.set( Start-1, Start );
    otherAddressRange.set( Start-1, End+1 );
    addressRange.adaptToReplacement( Start, 0, Width );
    QCOMPARE( addressRange, otherAddressRange );

    // adaptToReplacement, insert, before
    addressRange.set( Start, Start );
    otherAddressRange.set( End+1, End+1 );
    addressRange.adaptToReplacement( Start, 0, Width );
    QCOMPARE( addressRange, otherAddressRange );
}


void AddressRangeTest::testSplitAt()
{
    AddressRange addressRange( Start, End );

    // split at start
    AddressRange splitAddressRange = addressRange.splitAt( Start );
    QVERIFY( !addressRange.isValid() );
    QCOMPARE( splitAddressRange.start(), Start );
    QCOMPARE( splitAddressRange.end(),   End );

    // split at one after start
    addressRange.set( Start, End );
    splitAddressRange = addressRange.splitAt( Start+1 );
    QCOMPARE( addressRange.start(), Start );
    QCOMPARE( addressRange.end(), Start );
    QCOMPARE( splitAddressRange.start(), Start+1 );
    QCOMPARE( splitAddressRange.end(),   End );

    // split at mid
    const Address Mid = (Start+End)/2;
    addressRange.set( Start, End );
    splitAddressRange = addressRange.splitAt( Mid );
    QCOMPARE( addressRange.start(), Start );
    QCOMPARE( addressRange.end(), Mid-1 );
    QCOMPARE( splitAddressRange.start(), Mid );
    QCOMPARE( splitAddressRange.end(),   End );

    // split at one before end
    addressRange.set( Start, End );
    splitAddressRange = addressRange.splitAt( End );
    QCOMPARE( addressRange.start(), Start );
    QCOMPARE( addressRange.end(), End-1 );
    QCOMPARE( splitAddressRange.start(), End );
    QCOMPARE( splitAddressRange.end(),   End );

    // split at start so the split is the full
    addressRange.set( Start, End );
    splitAddressRange = addressRange.splitAt( End+1 );
    QCOMPARE( addressRange.start(), Start );
    QCOMPARE( addressRange.end(), End );
    QVERIFY( !splitAddressRange.isValid() );
}

void AddressRangeTest::testSplitAtLocal()
{
    AddressRange addressRange( Start, End );

    // split at start
    AddressRange splitAddressRange = addressRange.splitAtLocal( 0 );
    QVERIFY( !addressRange.isValid() );
    QCOMPARE( splitAddressRange.start(), Start );
    QCOMPARE( splitAddressRange.end(),   End );

    // split at one after start
    addressRange.set( Start, End );
    splitAddressRange = addressRange.splitAtLocal( 1 );
    QCOMPARE( addressRange.start(), Start );
    QCOMPARE( addressRange.end(), Start );
    QCOMPARE( splitAddressRange.start(), Start+1 );
    QCOMPARE( splitAddressRange.end(),   End );

    // split at mid
    const Address Mid = Width/2;
    addressRange.set( Start, End );
    splitAddressRange = addressRange.splitAtLocal( Mid );
    QCOMPARE( addressRange.start(), Start );
    QCOMPARE( addressRange.end(), Start+Mid-1 );
    QCOMPARE( splitAddressRange.start(), Start+Mid );
    QCOMPARE( splitAddressRange.end(),   End );

    // split at one before end
    addressRange.set( Start, End );
    splitAddressRange = addressRange.splitAtLocal( Width-1 );
    QCOMPARE( addressRange.start(), Start );
    QCOMPARE( addressRange.end(), End-1 );
    QCOMPARE( splitAddressRange.start(), End );
    QCOMPARE( splitAddressRange.end(),   End );

    // split at start so the split is the full
    addressRange.set( Start, End );
    splitAddressRange = addressRange.splitAtLocal( Width );
    QCOMPARE( addressRange.start(), Start );
    QCOMPARE( addressRange.end(), End );
    QVERIFY( !splitAddressRange.isValid() );
}

void AddressRangeTest::testRemove()
{
    const Address Mid = (Start+End)/2;
    AddressRange addressRange( Start, End );

    // remove none at start
    AddressRange removeAddressRange( Start, Start-1  );
    AddressRange splitAddressRange = addressRange.remove( removeAddressRange );
    QVERIFY( !addressRange.isValid() );
    QCOMPARE( splitAddressRange.start(), Start );
    QCOMPARE( splitAddressRange.end(),   End );

    // remove one at start
    addressRange.set( Start, End );
    removeAddressRange.set( Start, Start );
    splitAddressRange = addressRange.remove( removeAddressRange );
    QVERIFY( !addressRange.isValid() );
    QCOMPARE( splitAddressRange.start(), Start+1 );
    QCOMPARE( splitAddressRange.end(),   End );

    // remove many at start
    addressRange.set( Start, End );
    removeAddressRange.set( Start, Mid );
    splitAddressRange = addressRange.remove( removeAddressRange );
    QVERIFY( !addressRange.isValid() );
    QCOMPARE( splitAddressRange.start(), Mid+1 );
    QCOMPARE( splitAddressRange.end(),   End );

    // remove all except last
    addressRange.set( Start, End );
    removeAddressRange.set( Start, End-1 );
    splitAddressRange = addressRange.remove( removeAddressRange );
    QVERIFY( !addressRange.isValid() );
    QCOMPARE( splitAddressRange.start(), End );
    QCOMPARE( splitAddressRange.end(),   End );

    // remove at mid
    addressRange.set( Start, End );
    removeAddressRange.set( Mid-1, Mid+1 );
    splitAddressRange = addressRange.remove( removeAddressRange );
    QCOMPARE( addressRange.start(), Start );
    QCOMPARE( addressRange.end(), Mid-2 );
    QCOMPARE( splitAddressRange.start(), Mid+2 );
    QCOMPARE( splitAddressRange.end(),   End );

    // remove none at end
    addressRange.set( Start, End );
    removeAddressRange.set( End+1, End );
    splitAddressRange = addressRange.remove( removeAddressRange );
    QCOMPARE( addressRange.start(), Start );
    QCOMPARE( addressRange.end(),   End );
    QVERIFY( !splitAddressRange.isValid() );

    // remove one at end
    addressRange.set( Start, End );
    removeAddressRange.set( End, End );
    splitAddressRange = addressRange.remove( removeAddressRange );
    QCOMPARE( addressRange.start(), Start );
    QCOMPARE( addressRange.end(),   End-1 );
    QVERIFY( !splitAddressRange.isValid() );

    // remove many at end
    addressRange.set( Start, End );
    removeAddressRange.set( Mid, End );
    splitAddressRange = addressRange.remove( removeAddressRange );
    QCOMPARE( addressRange.start(), Start );
    QCOMPARE( addressRange.end(),   Mid-1 );
    QVERIFY( !splitAddressRange.isValid() );

    // remove all except first
    addressRange.set( Start, End );
    removeAddressRange.set( Start+1, End );
    splitAddressRange = addressRange.remove( removeAddressRange );
    QCOMPARE( addressRange.start(), Start );
    QCOMPARE( addressRange.end(),   Start );
    QVERIFY( !splitAddressRange.isValid() );

    // remove all
    addressRange.set( Start, End );
    removeAddressRange.set( Start, End );
    splitAddressRange = addressRange.remove( removeAddressRange );
    QVERIFY( !addressRange.isValid() );
    QVERIFY( !splitAddressRange.isValid() );
}

void AddressRangeTest::testRemoveLocal()
{
    const Address Mid = Width/2;
    AddressRange addressRange( Start, End );

    // remove none at start
    AddressRange removeAddressRange( 0, 0-1  );
    AddressRange splitAddressRange = addressRange.removeLocal( removeAddressRange );
    QVERIFY( !addressRange.isValid() );
    QCOMPARE( splitAddressRange.start(), Start );
    QCOMPARE( splitAddressRange.end(),   End );

    // remove one at start
    addressRange.set( Start, End );
    removeAddressRange.set( 0, 0 );
    splitAddressRange = addressRange.removeLocal( removeAddressRange );
    QVERIFY( !addressRange.isValid() );
    QCOMPARE( splitAddressRange.start(), Start+1 );
    QCOMPARE( splitAddressRange.end(),   End );

    // remove many at start
    addressRange.set( Start, End );
    removeAddressRange.set( 0, Mid );
    splitAddressRange = addressRange.removeLocal( removeAddressRange );
    QVERIFY( !addressRange.isValid() );
    QCOMPARE( splitAddressRange.start(), Start+Mid+1 );
    QCOMPARE( splitAddressRange.end(),   End );

    // remove all except last
    addressRange.set( Start, End );
    removeAddressRange.set( 0, Width-2 );
    splitAddressRange = addressRange.removeLocal( removeAddressRange );
    QVERIFY( !addressRange.isValid() );
    QCOMPARE( splitAddressRange.start(), End );
    QCOMPARE( splitAddressRange.end(),   End );

    // remove at mid
    addressRange.set( Start, End );
    removeAddressRange.set( Mid-1, Mid+1 );
    splitAddressRange = addressRange.removeLocal( removeAddressRange );
    QCOMPARE( addressRange.start(), Start );
    QCOMPARE( addressRange.end(), Start+Mid-2 );
    QCOMPARE( splitAddressRange.start(), Start+Mid+2 );
    QCOMPARE( splitAddressRange.end(),   End );

    // remove none at end
    addressRange.set( Start, End );
    removeAddressRange.set( Width, Width-1 );
    splitAddressRange = addressRange.removeLocal( removeAddressRange );
    QCOMPARE( addressRange.start(), Start );
    QCOMPARE( addressRange.end(),   End );
    QVERIFY( !splitAddressRange.isValid() );

    // remove one at end
    addressRange.set( Start, End );
    removeAddressRange.set( Width-1, Width-1 );
    splitAddressRange = addressRange.removeLocal( removeAddressRange );
    QCOMPARE( addressRange.start(), Start );
    QCOMPARE( addressRange.end(),   End-1 );
    QVERIFY( !splitAddressRange.isValid() );

    // remove many at end
    addressRange.set( Start, End );
    removeAddressRange.set( Mid, Width-1 );
    splitAddressRange = addressRange.removeLocal( removeAddressRange );
    QCOMPARE( addressRange.start(), Start );
    QCOMPARE( addressRange.end(),   Start+Mid-1 );
    QVERIFY( !splitAddressRange.isValid() );

    // remove all except first
    addressRange.set( Start, End );
    removeAddressRange.set( 1, Width-1 );
    splitAddressRange = addressRange.removeLocal( removeAddressRange );
    QCOMPARE( addressRange.start(), Start );
    QCOMPARE( addressRange.end(),   Start );
    QVERIFY( !splitAddressRange.isValid() );

    // remove all
    addressRange.set( Start, End );
    removeAddressRange.set( 0, Width-1 );
    splitAddressRange = addressRange.removeLocal( removeAddressRange );
    QVERIFY( !addressRange.isValid() );
    QVERIFY( !splitAddressRange.isValid() );
}

void AddressRangeTest::testStartForInclude()
{
    // testStartForInclude, same length, start at start
    const AddressRange addressRange( Start, End );
    AddressRange otherAddressRange( addressRange );
    QCOMPARE( addressRange.startForInclude(otherAddressRange), otherAddressRange.start() );

    // testStartForInclude, same length, start before start
    otherAddressRange.moveBy( -1 );
    QCOMPARE( addressRange.startForInclude(otherAddressRange), otherAddressRange.start() );

    // testStartForInclude, same length, end behind end
    otherAddressRange.moveBy( 2 );
    QCOMPARE( addressRange.startForInclude(otherAddressRange), otherAddressRange.start() );

    // testStartForInclude, smaller length, start at start
    otherAddressRange.set( Start, End-1 );
    Size Diff = addressRange.width() - otherAddressRange.width();
    QCOMPARE( addressRange.startForInclude(otherAddressRange), otherAddressRange.start() );

    // testStartForInclude, smaller length, start before start
    otherAddressRange.moveBy( -1 );
    QCOMPARE( addressRange.startForInclude(otherAddressRange), otherAddressRange.start() );

    // testStartForInclude, smaller length, end behind end
    otherAddressRange.moveBy( 2 );
    QCOMPARE( addressRange.startForInclude(otherAddressRange), otherAddressRange.start()-Diff );

/* // undefined in startForInclude
    // testStartForInclude, greater length, start at start
    otherAddressRange.set( Start, End+1 );
    Diff = addressRange.width() - otherAddressRange.width();
    QCOMPARE( addressRange.startForInclude(otherAddressRange), otherAddressRange.start() );

    // testStartForInclude, smaller length, start before start
    otherAddressRange.moveBy( -1 );
    QCOMPARE( addressRange.startForInclude(otherAddressRange), otherAddressRange.start() );

    // testStartForInclude, smaller length, end behind end
    otherAddressRange.moveBy( 2 );
    QCOMPARE( addressRange.startForInclude(otherAddressRange), otherAddressRange.start()-Diff );
*/
}


void AddressRangeTest::testIsJoinable()
{
    // testStartForInclude, same length, start at start
    const AddressRange addressRange( Start, End );
    AddressRange otherAddressRange( addressRange );
    QVERIFY( addressRange.isJoinable(otherAddressRange) );

    // adaptToReplacement, insert, before
    otherAddressRange.set( End+1, End+Width );
    QVERIFY( addressRange.isJoinable(otherAddressRange) );

    // Overlapping, right shifted
    otherAddressRange.set( Start+1, End+1 );
    QVERIFY( addressRange.isJoinable(otherAddressRange) );

    // Overlapping, left shifted
    otherAddressRange.set( Start-1, End-1 );
    QVERIFY( addressRange.isJoinable(otherAddressRange) );

    // Overlapping, 1 includes 2
    otherAddressRange.set( Start+1, End-1 );
    QVERIFY( addressRange.isJoinable(otherAddressRange) );

    // Overlapping, 2 includes 1
    otherAddressRange.set( Start-1, End+1 );
    QVERIFY( addressRange.isJoinable(otherAddressRange) );

    // Overlapping, identic
    otherAddressRange.set( addressRange );
    QVERIFY( addressRange.isJoinable(otherAddressRange) );

    // Coupled, first, then second
    otherAddressRange.set( End+1, End+Width );
    QVERIFY( addressRange.isJoinable(otherAddressRange) );

    // Coupled, second, then first
    QVERIFY( otherAddressRange.isJoinable(addressRange) );

    // NonOverlapping, first, then second
    otherAddressRange.set( End+2, End+Width+1 );
    QVERIFY( !addressRange.isJoinable(otherAddressRange) );

    // NonOverlapping, second, then first
    QVERIFY( !otherAddressRange.isJoinable(addressRange) );
}

}

QTEST_MAIN( Okteta::AddressRangeTest )
