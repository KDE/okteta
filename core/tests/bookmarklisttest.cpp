/*
    This file is part of the Okteta Core library, made within the KDE community.

    Copyright 2007 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "bookmarklisttest.h"

// test object
#include <bookmarklist.h>
// Qt
#include <QtTest/QtTest>

using namespace Okteta;

static const int Offset1 = 7;
static const int Offset2 = 23;
static const int Offset3 = 45;
static const int Offset4 = 67;
static const int Offset5 = 89;
static const int Distance12 = Offset2 - Offset1;
static const int Distance23 = Offset3 - Offset2;
static const int Distance34 = Offset4 - Offset3;
static const int Distance45 = Offset5 - Offset4;
static const int Distance13 = Distance12 + Distance23;
static const int Distance35 = Distance34 + Distance45;


void BookmarkListTest::testSimpleConstructor()
{
    BookmarkList bookmarkList;

    QVERIFY( bookmarkList.isEmpty() );
}

void BookmarkListTest::testAddRemoveBookmark()
{
    const Bookmark bookmark1( Offset1 );
    const Bookmark bookmark2( Offset2 );

    BookmarkList bookmarkList;

    bookmarkList.addBookmark( bookmark1 );

    QVERIFY( bookmarkList.first() == bookmark1 );
    QVERIFY( !bookmarkList.isEmpty() );

    bookmarkList.addBookmark( bookmark2 );

    QVERIFY( bookmarkList.first() == bookmark1 );
    QVERIFY( !bookmarkList.isEmpty() );
    QCOMPARE( bookmarkList.count(), 2 );

    bookmarkList.removeBookmark( bookmark1 );

    QVERIFY( bookmarkList.first() == bookmark2 );
    QVERIFY( !bookmarkList.isEmpty() );

    bookmarkList.removeBookmark( bookmark2 );

    QVERIFY( bookmarkList.isEmpty() );
}

void BookmarkListTest::testContains()
{
    const Bookmark bookmark1( Offset1 );
    const Bookmark bookmark2( Offset2 );

    BookmarkList bookmarkList;

    QVERIFY( !bookmarkList.contains(bookmark1.offset()) );
    QVERIFY( !bookmarkList.contains(bookmark2.offset()) );

    bookmarkList.addBookmark( bookmark1 );
    QVERIFY( bookmarkList.contains(bookmark1.offset()) );
    QVERIFY( !bookmarkList.contains(bookmark2.offset()) );

    bookmarkList.addBookmark( bookmark2 );
    QVERIFY( bookmarkList.contains(bookmark1.offset()) );
    QVERIFY( bookmarkList.contains(bookmark2.offset()) );

    bookmarkList.removeBookmark( bookmark2 );
    QVERIFY( bookmarkList.contains(bookmark1.offset()) );
    QVERIFY( !bookmarkList.contains(bookmark2.offset()) );

    bookmarkList.removeBookmark( bookmark1 );
    QVERIFY( !bookmarkList.contains(bookmark1.offset()) );
    QVERIFY( !bookmarkList.contains(bookmark2.offset()) );
}
// TODO: these functions (in a variant) are now part of BookmarksConstIterator
#if 0
void BookmarkListTest::testPreviousFrom()
{
    const Bookmark bookmark1( Offset1 );
    const Bookmark bookmark2( Offset2 );

    BookmarkList bookmarkList;

    bookmarkList.addBookmark( bookmark1 );
    bookmarkList.addBookmark( bookmark2 );

    QVERIFY( bookmarkList.previousFrom(Offset1-1) == bookmarkList.end() );
    QVERIFY( *bookmarkList.previousFrom(Offset1) == bookmark1 );
    QVERIFY( *bookmarkList.previousFrom(Offset2-1) == bookmark1 );
    QVERIFY( *bookmarkList.previousFrom(Offset2) == bookmark2 );
    QVERIFY( *bookmarkList.previousFrom(Offset2+1) == bookmark2 );
}

void BookmarkListTest::testNextFrom()
{
    const Bookmark bookmark1( Offset1 );
    const Bookmark bookmark2( Offset2 );

    BookmarkList bookmarkList;

    bookmarkList.addBookmark( bookmark1 );
    bookmarkList.addBookmark( bookmark2 );

    QVERIFY( *bookmarkList.nextFrom(Offset1-1) == bookmark1 );
    QVERIFY( *bookmarkList.nextFrom(Offset1) == bookmark1 );
    QVERIFY( *bookmarkList.nextFrom(Offset1+1) == bookmark2 );
    QVERIFY( *bookmarkList.nextFrom(Offset2) == bookmark2 );
    QVERIFY( bookmarkList.nextFrom(Offset2+1) == bookmarkList.end() );
}
#endif
void BookmarkListTest::testAdjustToReplaced()
{
    const Bookmark bookmark1( Offset1 );
    const Bookmark bookmark2( Offset2 );
    const Bookmark bookmark3( Offset3 );

    BookmarkList bookmarkList;

    // replace after last -> no changes
    static const int behindOffset3 = Offset3 + 1;
    bookmarkList.addBookmark( bookmark1 );
    bookmarkList.addBookmark( bookmark2 );
    bookmarkList.addBookmark( bookmark3 );
    bool adjusted = bookmarkList.adjustToReplaced( behindOffset3, 20, 19 );

    QCOMPARE( bookmarkList.count(), 3 );
    QVERIFY( !bookmarkList.isEmpty() );
    QVERIFY( !adjusted );

    // replace beore first with same length -> no changes
    bookmarkList.clear();
    bookmarkList.addBookmark( bookmark1 );
    bookmarkList.addBookmark( bookmark2 );
    bookmarkList.addBookmark( bookmark3 );

    adjusted = bookmarkList.adjustToReplaced( 0, 5, 5 );

    QCOMPARE( bookmarkList.count(), 3 );
    QVERIFY( !bookmarkList.isEmpty() );
    QVERIFY( !adjusted );

    // replace after 1 and before 3 with same length
    static const int replaceLength = Distance13 - 1;
    static const int behindOffset1 = Offset1 + 1;
    bookmarkList.clear();
    bookmarkList.addBookmark( bookmark1 );
    bookmarkList.addBookmark( bookmark2 );
    bookmarkList.addBookmark( bookmark3 );
    bookmarkList.adjustToReplaced( behindOffset1, replaceLength, replaceLength );

    QVERIFY( bookmarkList.first() == bookmark1 );
    QVERIFY( bookmarkList.last() == bookmark3 );
    QCOMPARE( bookmarkList.count(), 2 );
    QVERIFY( !bookmarkList.isEmpty() );

    // replace after 1 and before 3 with longer length
    static const int longer = 5;
    bookmarkList.clear();
    bookmarkList.addBookmark( bookmark1 );
    bookmarkList.addBookmark( bookmark2 );
    bookmarkList.addBookmark( bookmark3 );
    bookmarkList.adjustToReplaced( behindOffset1, replaceLength, replaceLength+longer );

    QVERIFY( bookmarkList.first() == bookmark1 );
    QCOMPARE( bookmarkList.last().offset(), bookmark3.offset()+longer );
    QCOMPARE( bookmarkList.count(), 2 );
    QVERIFY( !bookmarkList.isEmpty() );

    // replace after 1 and before 3 with shorter length
    static const int shorter = 5;
    bookmarkList.clear();
    bookmarkList.addBookmark( bookmark1 );
    bookmarkList.addBookmark( bookmark2 );
    bookmarkList.addBookmark( bookmark3 );
    bookmarkList.adjustToReplaced( behindOffset1, replaceLength, replaceLength-shorter );

    QVERIFY( bookmarkList.first() == bookmark1 );
    QCOMPARE( bookmarkList.last().offset(), bookmark3.offset()-shorter );
    QCOMPARE( bookmarkList.count(), 2 );
    QVERIFY( !bookmarkList.isEmpty() );
}

void BookmarkListTest::testAdjustToSwapped()
{
    const Bookmark bookmark1( Offset1 );
    const Bookmark bookmark2( Offset2 );
    const Bookmark bookmark3_1( Offset3-1 );
    const Bookmark bookmark3( Offset3 );
    const Bookmark bookmark4( Offset4 );

    BookmarkList bookmarkList;

    // move all between 1 and 3 right before 4
    static const int secondLength = Distance34;
    static const int firstLength = Distance13-1;
    static const int behindOffset1 = Offset1 + 1;
    bookmarkList.addBookmark( bookmark1 );
    bookmarkList.addBookmark( bookmark2 );
    bookmarkList.addBookmark( bookmark3_1 );
    bookmarkList.addBookmark( bookmark3 );
    bookmarkList.addBookmark( bookmark4 );
    bool adjusted = bookmarkList.adjustToSwapped( behindOffset1, Offset3, secondLength );

    QList<int> newOffsets;
    newOffsets << bookmark1.offset() << bookmark3.offset()-firstLength << bookmark2.offset()+secondLength
               << bookmark3_1.offset()+secondLength<<bookmark4.offset();

    QVERIFY( adjusted );
    QVERIFY( !bookmarkList.isEmpty() );
    QCOMPARE( bookmarkList.count(), 5 );
    QList<int>::ConstIterator oit = newOffsets.constBegin();
    foreach( const Bookmark &bookmark, bookmarkList )
        QCOMPARE( bookmark.offset(), *oit++ );
}



QTEST_MAIN( BookmarkListTest )
