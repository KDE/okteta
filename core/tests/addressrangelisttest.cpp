/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2006 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "addressrangelisttest.hpp"

// test object
#include <addressrangelist.hpp>
// Qt
#include <QTest>

namespace Okteta {

// local variables
static constexpr Address Start1 = 15;
static constexpr Address End1 = 27;
static constexpr Size Width = End1 - Start1 + 1;
static constexpr Size Distance = 10;
static constexpr Address Start2 = End1 + Distance;
static constexpr Address End2 = Start2 + Width - 1;

void AddressRangeListTest::testSimpleConstructor()
{
    AddressRangeList addressRangeList;
    QVERIFY(addressRangeList.isEmpty());
}

void AddressRangeListTest::testAddSingleRange()
{
    AddressRangeList addressRangeList;

    AddressRange addressRange(Start1, End1);
    addressRangeList.addAddressRange(addressRange);
    QCOMPARE(addressRangeList.size(), 1);
    QCOMPARE(addressRangeList.first(), addressRange);
}

void AddressRangeListTest::testAddJoinableRanges()
{
    AddressRangeList addressRangeList;

    AddressRange addressRange1(Start1, End1);
    AddressRange addressRange2(Start2, End2);
    // in front
    AddressRange otherAddressRange(0, Start1 - 1);
    addressRangeList.addAddressRange(addressRange1);
    addressRangeList.addAddressRange(addressRange2);
    addressRangeList.addAddressRange(otherAddressRange);

    QCOMPARE(addressRangeList.size(), 2);
    QCOMPARE(addressRangeList.first(), AddressRange(otherAddressRange.start(), addressRange1.end()));
    QCOMPARE(addressRangeList.last(), addressRange2);

    // in the middle, with first
    addressRangeList.clear();
    otherAddressRange.set(End1 + 1, Start2 - 2);
    addressRangeList.addAddressRange(addressRange1);
    addressRangeList.addAddressRange(addressRange2);
    addressRangeList.addAddressRange(otherAddressRange);

    QCOMPARE(addressRangeList.size(), 2);
    QCOMPARE(addressRangeList.first(), AddressRange(addressRange1.start(), otherAddressRange.end()));
    QCOMPARE(addressRangeList.last(), addressRange2);

    // in the middle, with both
    addressRangeList.clear();
    otherAddressRange.set(End1 + 1, Start2 - 1);
    addressRangeList.addAddressRange(addressRange1);
    addressRangeList.addAddressRange(addressRange2);
    addressRangeList.addAddressRange(otherAddressRange);

    QCOMPARE(addressRangeList.size(), 1);
    QCOMPARE(addressRangeList.first(), AddressRange(addressRange1.start(), addressRange2.end()));

    // in the middle, with last
    addressRangeList.clear();
    otherAddressRange.set(End1 + 2, Start2 - 1);
    addressRangeList.addAddressRange(addressRange1);
    addressRangeList.addAddressRange(addressRange2);
    addressRangeList.addAddressRange(otherAddressRange);

    QCOMPARE(addressRangeList.size(), 2);
    QCOMPARE(addressRangeList.first(), addressRange1);
    QCOMPARE(addressRangeList.last(), AddressRange(otherAddressRange.start(), addressRange2.end()));

    // behind
    addressRangeList.clear();
    otherAddressRange.setByWidth(End2 + 1, Width);
    addressRangeList.addAddressRange(addressRange1);
    addressRangeList.addAddressRange(addressRange2);
    addressRangeList.addAddressRange(otherAddressRange);

    QCOMPARE(addressRangeList.size(), 2);
    QCOMPARE(addressRangeList.first(), addressRange1);
    QCOMPARE(addressRangeList.last(), AddressRange(addressRange2.start(), otherAddressRange.end()));
}

void AddressRangeListTest::testAddNonJoinableRanges()
{
    AddressRangeList addressRangeList;

    AddressRange addressRange1(Start1, End1);
    AddressRange addressRange2(Start2, End2);
    // in front
    AddressRange otherAddressRange(0, Start1 - 2);
    addressRangeList.addAddressRange(addressRange1);
    addressRangeList.addAddressRange(addressRange2);
    addressRangeList.addAddressRange(otherAddressRange);

    QCOMPARE(addressRangeList.size(), 3);
    QCOMPARE(addressRangeList.first(), otherAddressRange);
    QCOMPARE(addressRangeList.last(), addressRange2);

    // in the middle
    addressRangeList.clear();
    otherAddressRange.set(End1 + 2, Start2 - 2);
    addressRangeList.addAddressRange(addressRange1);
    addressRangeList.addAddressRange(addressRange2);
    addressRangeList.addAddressRange(otherAddressRange);

    QCOMPARE(addressRangeList.size(), 3);
    QCOMPARE(addressRangeList.first(), addressRange1);
    QCOMPARE(addressRangeList.last(), addressRange2);

    // behind
    addressRangeList.clear();
    otherAddressRange.setByWidth(End2 + 2, Width);
    addressRangeList.addAddressRange(addressRange1);
    addressRangeList.addAddressRange(addressRange2);
    addressRangeList.addAddressRange(otherAddressRange);

    QCOMPARE(addressRangeList.size(), 3);
    QCOMPARE(addressRangeList.first(), addressRange1);
    QCOMPARE(addressRangeList.last(), otherAddressRange);
}

}

QTEST_GUILESS_MAIN(Okteta::AddressRangeListTest)
