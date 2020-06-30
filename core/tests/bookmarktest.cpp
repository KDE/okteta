/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2007 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bookmarktest.hpp"

// test object
#include <bookmark.hpp>
// Qt
#include <QTest>

using namespace Okteta;

static constexpr int Offset1 = 7;
static constexpr int Offset2 = 23;

void BookmarkTest::testSimpleConstructor()
{
    Bookmark bookmark;

    QVERIFY(!bookmark.isValid());
}

void BookmarkTest::testConstructor()
{
    Bookmark bookmark(Offset1);

    QVERIFY(bookmark.isValid());
    QCOMPARE(bookmark.offset(), Offset1);
}

void BookmarkTest::testCompare()
{
    Bookmark bookmark1(Offset1);
    Bookmark bookmark2(Offset1);
    Bookmark bookmark3(Offset2);

    QVERIFY(bookmark1 == bookmark2);
    QVERIFY(!(bookmark1 == bookmark3));
}

void BookmarkTest::testMove()
{
    Bookmark bookmark(Offset1);
    bookmark.move(Offset2);

    QVERIFY(bookmark.isValid());
    QCOMPARE(bookmark.offset(), Offset1 + Offset2);
}

QTEST_GUILESS_MAIN(BookmarkTest)
