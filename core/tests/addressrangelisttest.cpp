/*
    This file is part of the Okteta Core library, made within the KDE community.

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

#include "addressrangelisttest.h"

// test object
#include <addressrangelist.h>
// Qt
#include <QtTest/QtTest>


namespace Okteta
{

// local variables
static const Address Start1 = 15;
static const Address End1 = 27;
static const Size Width = End1 - Start1 + 1;
static const Size Distance = 10;
static const Address Start2 = End1 + Distance;
static const Address End2 = Start2+Width-1;


void AddressRangeListTest::testSimpleConstructor()
{
    AddressRangeList addressRangeList;
    QVERIFY( addressRangeList.isEmpty() );
}


void AddressRangeListTest::testAddSingleRange()
{
    AddressRangeList addressRangeList;

    AddressRange addressRange( Start1, End1 );
    addressRangeList.addAddressRange( addressRange );
    QCOMPARE( addressRangeList.size(), 1 );
    QCOMPARE( addressRangeList.first(), addressRange );
}


void AddressRangeListTest::testAddJoinableRanges()
{
    AddressRangeList addressRangeList;

    AddressRange addressRange1( Start1, End1 );
    AddressRange addressRange2( Start2, End2 );
    // in front
    AddressRange otherAddressRange( 0, Start1-1 );
    addressRangeList.addAddressRange( addressRange1 );
    addressRangeList.addAddressRange( addressRange2 );
    addressRangeList.addAddressRange( otherAddressRange );

    QCOMPARE( addressRangeList.size(), 2 );
    QCOMPARE( addressRangeList.first(), AddressRange(otherAddressRange.start(),addressRange1.end()) );
    QCOMPARE( addressRangeList.last(), addressRange2 );

    // in the middle, with first
    addressRangeList.clear();
    otherAddressRange.set( End1+1, Start2-2 );
    addressRangeList.addAddressRange( addressRange1 );
    addressRangeList.addAddressRange( addressRange2 );
    addressRangeList.addAddressRange( otherAddressRange );

    QCOMPARE( addressRangeList.size(), 2 );
    QCOMPARE( addressRangeList.first(), AddressRange(addressRange1.start(),otherAddressRange.end()) );
    QCOMPARE( addressRangeList.last(), addressRange2 );

    // in the middle, with both
    addressRangeList.clear();
    otherAddressRange.set( End1+1, Start2-1 );
    addressRangeList.addAddressRange( addressRange1 );
    addressRangeList.addAddressRange( addressRange2 );
    addressRangeList.addAddressRange( otherAddressRange );

    QCOMPARE( addressRangeList.size(), 1 );
    QCOMPARE( addressRangeList.first(), AddressRange(addressRange1.start(),addressRange2.end()) );

    // in the middle, with last
    addressRangeList.clear();
    otherAddressRange.set( End1+2, Start2-1 );
    addressRangeList.addAddressRange( addressRange1 );
    addressRangeList.addAddressRange( addressRange2 );
    addressRangeList.addAddressRange( otherAddressRange );

    QCOMPARE( addressRangeList.size(), 2 );
    QCOMPARE( addressRangeList.first(), addressRange1 );
    QCOMPARE( addressRangeList.last(), AddressRange(otherAddressRange.start(),addressRange2.end()) );

    // behind
    addressRangeList.clear();
    otherAddressRange.setByWidth( End2+1, Width );
    addressRangeList.addAddressRange( addressRange1 );
    addressRangeList.addAddressRange( addressRange2 );
    addressRangeList.addAddressRange( otherAddressRange );

    QCOMPARE( addressRangeList.size(), 2 );
    QCOMPARE( addressRangeList.first(), addressRange1 );
    QCOMPARE( addressRangeList.last(), AddressRange(addressRange2.start(),otherAddressRange.end()) );
}


void AddressRangeListTest::testAddNonJoinableRanges()
{
    AddressRangeList addressRangeList;

    AddressRange addressRange1( Start1, End1 );
    AddressRange addressRange2( Start2, End2 );
    // in front
    AddressRange otherAddressRange( 0, Start1-2 );
    addressRangeList.addAddressRange( addressRange1 );
    addressRangeList.addAddressRange( addressRange2 );
    addressRangeList.addAddressRange( otherAddressRange );

    QCOMPARE( addressRangeList.size(), 3 );
    QCOMPARE( addressRangeList.first(), otherAddressRange );
    QCOMPARE( addressRangeList.last(), addressRange2 );

    // in the middle
    addressRangeList.clear();
    otherAddressRange.set( End1+2, Start2-2 );
    addressRangeList.addAddressRange( addressRange1 );
    addressRangeList.addAddressRange( addressRange2 );
    addressRangeList.addAddressRange( otherAddressRange );

    QCOMPARE( addressRangeList.size(), 3 );
    QCOMPARE( addressRangeList.first(), addressRange1 );
    QCOMPARE( addressRangeList.last(), addressRange2 );

    // behind
    addressRangeList.clear();
    otherAddressRange.setByWidth( End2+2, Width );
    addressRangeList.addAddressRange( addressRange1 );
    addressRangeList.addAddressRange( addressRange2 );
    addressRangeList.addAddressRange( otherAddressRange );

    QCOMPARE( addressRangeList.size(), 3 );
    QCOMPARE( addressRangeList.first(), addressRange1 );
    QCOMPARE( addressRangeList.last(), otherAddressRange );
}

}

QTEST_MAIN( Okteta::AddressRangeListTest )
