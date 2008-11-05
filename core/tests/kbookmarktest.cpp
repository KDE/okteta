/*
    This file is part of the Okteta Core library, part of the KDE project.

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

#include "kbookmarktest.h"

// test object
#include <bookmark.h>
// Qt
#include <QtTest/QtTest>

using namespace KHECore;


static const int Offset1 = 7;
static const int Offset2 = 23;

void KBookmarkTest::testSimpleConstructor()
{
  Bookmark bookmark;

  QVERIFY( !bookmark.isValid() );
}

void KBookmarkTest::testConstructor()
{
  Bookmark bookmark( Offset1 );

  QVERIFY( bookmark.isValid() );
  QCOMPARE( bookmark.offset(), Offset1 );
}

void KBookmarkTest::testCompare()
{
  Bookmark bookmark1( Offset1 );
  Bookmark bookmark2( Offset1 );
  Bookmark bookmark3( Offset2 );

  QVERIFY( bookmark1 == bookmark2 );
  QVERIFY( !(bookmark1 == bookmark3) );
}

void KBookmarkTest::testMove()
{
  Bookmark bookmark( Offset1 );
  bookmark.move( Offset2 );

  QVERIFY( bookmark.isValid() );
  QCOMPARE( bookmark.offset(), Offset1+Offset2 );
}


QTEST_MAIN( KBookmarkTest )
