/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "arraychangemetricstest.hpp"

// test object
#include <arraychangemetrics.hpp>
// Qt
#include <QTest>

namespace Okteta {

// local variables
static constexpr Address Offset = 15;

static constexpr Size LengthDiff = 5;
static constexpr Size InsertLength = 15;
static constexpr Size SmallerRemoveLength = InsertLength - LengthDiff;

static constexpr Size FirstLength = 15;
static constexpr Address SecondOffset = Offset + FirstLength;
static constexpr Size SecondLength = 10;
static constexpr Address SecondEnd = SecondOffset + SecondLength - 1;

void ArrayChangeMetricsTest::testConstructorAsReplacement()
{
    ArrayChangeMetrics change = ArrayChangeMetrics::asReplacement(Offset, SmallerRemoveLength, InsertLength);

    QVERIFY(change.isValid());
    QCOMPARE(change.offset(), Offset);
    QCOMPARE(change.removeLength(), SmallerRemoveLength);
    QCOMPARE(change.insertLength(), InsertLength);
    QCOMPARE(change.lengthChange(), LengthDiff);
}

void ArrayChangeMetricsTest::testConstructorAsSwapping()
{
    ArrayChangeMetrics change = ArrayChangeMetrics::asSwapping(Offset, SecondOffset, SecondLength);

    QVERIFY(change.isValid());
    QCOMPARE(change.offset(), Offset);
    QCOMPARE(change.firstLength(), FirstLength);
    QCOMPARE(change.secondStart(), SecondOffset);
    QCOMPARE(change.secondEnd(), SecondEnd);
    QCOMPARE(change.secondLength(), SecondLength);
}

void ArrayChangeMetricsTest::testRevert()
{
    ArrayChangeMetrics replacement = ArrayChangeMetrics::asReplacement(Offset, SmallerRemoveLength, InsertLength);
    replacement.revert();

    QVERIFY(replacement.isValid());
    QCOMPARE(replacement.offset(), Offset);
    QCOMPARE(replacement.removeLength(), InsertLength);
    QCOMPARE(replacement.insertLength(), SmallerRemoveLength);
    QCOMPARE(replacement.lengthChange(), -LengthDiff);

    replacement.revert();

    QVERIFY(replacement.isValid());
    QCOMPARE(replacement.offset(), Offset);
    QCOMPARE(replacement.removeLength(), SmallerRemoveLength);
    QCOMPARE(replacement.insertLength(), InsertLength);
    QCOMPARE(replacement.lengthChange(), LengthDiff);

    ArrayChangeMetrics swapping = ArrayChangeMetrics::asSwapping(Offset, SecondOffset, SecondLength);
    swapping.revert();

    QVERIFY(swapping.isValid());
    QCOMPARE(swapping.offset(), Offset);
    QCOMPARE(swapping.firstLength(), SecondLength);
    QCOMPARE(swapping.secondStart(), Offset + SecondLength);
    QCOMPARE(swapping.secondEnd(), SecondEnd);
    QCOMPARE(swapping.secondLength(), FirstLength);

    swapping.revert();

    QVERIFY(swapping.isValid());
    QCOMPARE(swapping.offset(), Offset);
    QCOMPARE(swapping.firstLength(), FirstLength);
    QCOMPARE(swapping.secondStart(), SecondOffset);
    QCOMPARE(swapping.secondEnd(), SecondEnd);
    QCOMPARE(swapping.secondLength(), SecondLength);
}

}

QTEST_GUILESS_MAIN(Okteta::ArrayChangeMetricsTest)
