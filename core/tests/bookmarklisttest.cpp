/*
    SPDX-FileCopyrightText: 2007 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bookmarklisttest.hpp"

// test object
#include <bookmarklist.hpp>
// Qt
#include <QList>
#include <QTest>
// Std
#include <utility>

using namespace Okteta;

static constexpr Address Offset1 = 7;
static constexpr Address Offset2 = 23;
static constexpr Address Offset3 = 45;
static constexpr Address Offset4 = 67;
static constexpr Size Distance12 = Offset2 - Offset1;
static constexpr Size Distance23 = Offset3 - Offset2;
static constexpr Size Distance34 = Offset4 - Offset3;
static constexpr Size Distance13 = Distance12 + Distance23;

void BookmarkListTest::testSimpleConstructor()
{
    BookmarkList bookmarkList;

    QVERIFY(bookmarkList.isEmpty());
}

void BookmarkListTest::testAddRemoveBookmark()
{
    const Bookmark bookmark1(Offset1);
    const Bookmark bookmark2(Offset2);

    BookmarkList bookmarkList;

    bookmarkList.addBookmark(bookmark1);

    QVERIFY(bookmarkList.first() == bookmark1);
    QVERIFY(!bookmarkList.isEmpty());

    bookmarkList.addBookmark(bookmark2);

    QVERIFY(bookmarkList.first() == bookmark1);
    QVERIFY(!bookmarkList.isEmpty());
    QCOMPARE(bookmarkList.size(), 2);

    bookmarkList.removeBookmark(bookmark1);

    QVERIFY(bookmarkList.first() == bookmark2);
    QVERIFY(!bookmarkList.isEmpty());

    bookmarkList.removeBookmark(bookmark2);

    QVERIFY(bookmarkList.isEmpty());
}

void BookmarkListTest::testContains()
{
    const Bookmark bookmark1(Offset1);
    const Bookmark bookmark2(Offset2);

    BookmarkList bookmarkList;

    QVERIFY(!bookmarkList.contains(bookmark1.offset()));
    QVERIFY(!bookmarkList.contains(bookmark2.offset()));

    bookmarkList.addBookmark(bookmark1);
    QVERIFY(bookmarkList.contains(bookmark1.offset()));
    QVERIFY(!bookmarkList.contains(bookmark2.offset()));

    bookmarkList.addBookmark(bookmark2);
    QVERIFY(bookmarkList.contains(bookmark1.offset()));
    QVERIFY(bookmarkList.contains(bookmark2.offset()));

    bookmarkList.removeBookmark(bookmark2);
    QVERIFY(bookmarkList.contains(bookmark1.offset()));
    QVERIFY(!bookmarkList.contains(bookmark2.offset()));

    bookmarkList.removeBookmark(bookmark1);
    QVERIFY(!bookmarkList.contains(bookmark1.offset()));
    QVERIFY(!bookmarkList.contains(bookmark2.offset()));
}
// TODO: these functions (in a variant) are now part of BookmarksConstIterator
#if 0
void BookmarkListTest::testPreviousFrom()
{
    const Bookmark bookmark1(Offset1);
    const Bookmark bookmark2(Offset2);

    BookmarkList bookmarkList;

    bookmarkList.addBookmark(bookmark1);
    bookmarkList.addBookmark(bookmark2);

    QVERIFY(bookmarkList.previousFrom(Offset1 - 1) == bookmarkList.end());
    QVERIFY(*bookmarkList.previousFrom(Offset1) == bookmark1);
    QVERIFY(*bookmarkList.previousFrom(Offset2 - 1) == bookmark1);
    QVERIFY(*bookmarkList.previousFrom(Offset2) == bookmark2);
    QVERIFY(*bookmarkList.previousFrom(Offset2 + 1) == bookmark2);
}

void BookmarkListTest::testNextFrom()
{
    const Bookmark bookmark1(Offset1);
    const Bookmark bookmark2(Offset2);

    BookmarkList bookmarkList;

    bookmarkList.addBookmark(bookmark1);
    bookmarkList.addBookmark(bookmark2);

    QVERIFY(*bookmarkList.nextFrom(Offset1 - 1) == bookmark1);
    QVERIFY(*bookmarkList.nextFrom(Offset1) == bookmark1);
    QVERIFY(*bookmarkList.nextFrom(Offset1 + 1) == bookmark2);
    QVERIFY(*bookmarkList.nextFrom(Offset2) == bookmark2);
    QVERIFY(bookmarkList.nextFrom(Offset2 + 1) == bookmarkList.end());
}
#endif
void BookmarkListTest::testAdjustToReplaced()
{
    const Bookmark bookmark1(Offset1);
    const Bookmark bookmark2(Offset2);
    const Bookmark bookmark3(Offset3);

    BookmarkList bookmarkList;

    // replace after last -> no changes
    constexpr Address behindOffset3 = Offset3 + 1;
    bookmarkList.addBookmark(bookmark1);
    bookmarkList.addBookmark(bookmark2);
    bookmarkList.addBookmark(bookmark3);
    bool adjusted = bookmarkList.adjustToReplaced(behindOffset3, 20, 19);

    QCOMPARE(bookmarkList.size(), 3);
    QVERIFY(!bookmarkList.isEmpty());
    QVERIFY(!adjusted);

    // replace before first with same length -> no changes
    bookmarkList.clear();
    bookmarkList.addBookmark(bookmark1);
    bookmarkList.addBookmark(bookmark2);
    bookmarkList.addBookmark(bookmark3);

    adjusted = bookmarkList.adjustToReplaced(0, 5, 5);

    QCOMPARE(bookmarkList.size(), 3);
    QVERIFY(!bookmarkList.isEmpty());
    QVERIFY(!adjusted);

    // replace after 1 and before 3 with same length
    constexpr Size replaceLength = Distance13 - 1;
    constexpr Address behindOffset1 = Offset1 + 1;
    bookmarkList.clear();
    bookmarkList.addBookmark(bookmark1);
    bookmarkList.addBookmark(bookmark2);
    bookmarkList.addBookmark(bookmark3);
    bookmarkList.adjustToReplaced(behindOffset1, replaceLength, replaceLength);

    QVERIFY(bookmarkList.first() == bookmark1);
    QVERIFY(bookmarkList.last() == bookmark3);
    QCOMPARE(bookmarkList.size(), 2);
    QVERIFY(!bookmarkList.isEmpty());

    // replace after 1 and before 3 with longer length
    constexpr Size longer = 5;
    bookmarkList.clear();
    bookmarkList.addBookmark(bookmark1);
    bookmarkList.addBookmark(bookmark2);
    bookmarkList.addBookmark(bookmark3);
    bookmarkList.adjustToReplaced(behindOffset1, replaceLength, replaceLength + longer);

    QVERIFY(bookmarkList.first() == bookmark1);
    QCOMPARE(bookmarkList.last().offset(), bookmark3.offset() + longer);
    QCOMPARE(bookmarkList.size(), 2);
    QVERIFY(!bookmarkList.isEmpty());

    // replace after 1 and before 3 with shorter length
    constexpr Size shorter = 5;
    bookmarkList.clear();
    bookmarkList.addBookmark(bookmark1);
    bookmarkList.addBookmark(bookmark2);
    bookmarkList.addBookmark(bookmark3);
    bookmarkList.adjustToReplaced(behindOffset1, replaceLength, replaceLength - shorter);

    QVERIFY(bookmarkList.first() == bookmark1);
    QCOMPARE(bookmarkList.last().offset(), bookmark3.offset() - shorter);
    QCOMPARE(bookmarkList.size(), 2);
    QVERIFY(!bookmarkList.isEmpty());
}

void BookmarkListTest::testAdjustToSwapped()
{
    const Bookmark bookmark1(Offset1);
    const Bookmark bookmark2(Offset2);
    const Bookmark bookmark3_1(Offset3 - 1);
    const Bookmark bookmark3(Offset3);
    const Bookmark bookmark4(Offset4);

    BookmarkList bookmarkList;

    // move all between 1 and 3 right before 4
    constexpr Size secondLength = Distance34;
    constexpr Size firstLength = Distance13 - 1;
    constexpr Address behindOffset1 = Offset1 + 1;
    bookmarkList.addBookmark(bookmark1);
    bookmarkList.addBookmark(bookmark2);
    bookmarkList.addBookmark(bookmark3_1);
    bookmarkList.addBookmark(bookmark3);
    bookmarkList.addBookmark(bookmark4);
    bool adjusted = bookmarkList.adjustToSwapped(behindOffset1, Offset3, secondLength);

    QList<Address> newOffsets;
    newOffsets << bookmark1.offset() << bookmark3.offset() - firstLength << bookmark2.offset() + secondLength
               << bookmark3_1.offset() + secondLength << bookmark4.offset();

    QVERIFY(adjusted);
    QVERIFY(!bookmarkList.isEmpty());
    QCOMPARE(bookmarkList.size(), 5);
    auto oit = newOffsets.constBegin();
    for (const Bookmark& bookmark : std::as_const(bookmarkList)) {
        QCOMPARE(bookmark.offset(), *oit++);
    }
}

QTEST_GUILESS_MAIN(BookmarkListTest)

#include "moc_bookmarklisttest.cpp"
