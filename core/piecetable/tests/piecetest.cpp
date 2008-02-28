/*
    This file is part of the Okteta Core library, part of the KDE project.

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

#include "piecetest.h"

// test object
#include <piecetable/piece.h>
// Qt
#include <QtTest/QtTest>

using namespace KHE;
using namespace KPieceTable ;

// local variables
static const int Start = 15;
static const int End = 27;

static const int Width = End - Start + 1;

void PieceTest::testSimpleConstructor()
{
    Piece piece;
    QVERIFY( !piece.isValid() );
}

void PieceTest::testFullConstructor()
{
    const KSection storageSection( Start, End );
    Piece piece( storageSection, Piece::ChangeStorage );

    QCOMPARE( piece.start(), Start );
    QCOMPARE( piece.end(), storageSection.end() );
    QCOMPARE( piece.storageId(), (int)Piece::ChangeStorage );
    QVERIFY( piece.isValid() );
}


void PieceTest::testSplitAt()
{
    const KSection storageSection( Start, End );
    Piece piece( storageSection, Piece::ChangeStorage );

    // split at start
    Piece splitPiece = piece.splitAt( Start );
    QVERIFY( !piece.isValid() );
    QCOMPARE( splitPiece.start(), Start );
    QCOMPARE( splitPiece.end(),   End );
    QCOMPARE( splitPiece.storageId(), (int)Piece::ChangeStorage );

    // split at one after start
    piece.set( Start, End );
    splitPiece = piece.splitAt( Start+1 );
    QCOMPARE( piece.start(), Start );
    QCOMPARE( piece.end(), Start );
    QCOMPARE( piece.storageId(), (int)Piece::ChangeStorage );
    QCOMPARE( splitPiece.start(), Start+1 );
    QCOMPARE( splitPiece.end(),   End );
    QCOMPARE( splitPiece.storageId(), (int)Piece::ChangeStorage );

    // split at mid
    const int Mid = (Start+End)/2;
    piece.set( Start, End );
    splitPiece = piece.splitAt( Mid );
    QCOMPARE( piece.start(), Start );
    QCOMPARE( piece.end(), Mid-1 );
    QCOMPARE( piece.storageId(), (int)Piece::ChangeStorage );
    QCOMPARE( splitPiece.start(), Mid );
    QCOMPARE( splitPiece.end(),   End );
    QCOMPARE( splitPiece.storageId(), (int)Piece::ChangeStorage );

    // split at one before width
    piece.set( Start, End );
    splitPiece = piece.splitAt( End );
    QCOMPARE( piece.start(), Start );
    QCOMPARE( piece.end(), End-1 );
    QCOMPARE( piece.storageId(), (int)Piece::ChangeStorage );
    QCOMPARE( splitPiece.start(), End );
    QCOMPARE( splitPiece.end(),   End );
    QCOMPARE( splitPiece.storageId(), (int)Piece::ChangeStorage );

    // split at start so the split is the full
    piece.set( Start, End );
    splitPiece = piece.splitAt( End+1 );
    QCOMPARE( piece.start(), Start );
    QCOMPARE( piece.end(), End );
    QCOMPARE( piece.storageId(), (int)Piece::ChangeStorage );
    QVERIFY( !splitPiece.isValid() );
}

void PieceTest::testSplitAtLocal()
{
    const KSection storageSection( Start, End );
    Piece piece( storageSection, Piece::ChangeStorage );

    // split at start
    Piece splitPiece = piece.splitAtLocal( 0 );
    QVERIFY( !piece.isValid() );
    QCOMPARE( splitPiece.start(), Start );
    QCOMPARE( splitPiece.end(),   End );
    QCOMPARE( splitPiece.storageId(), (int)Piece::ChangeStorage );

    // split at one after start
    piece.set( Start, End );
    splitPiece = piece.splitAtLocal( 1 );
    QCOMPARE( piece.start(), Start );
    QCOMPARE( piece.end(), Start );
    QCOMPARE( piece.storageId(), (int)Piece::ChangeStorage );
    QCOMPARE( splitPiece.start(), Start+1 );
    QCOMPARE( splitPiece.end(),   End );
    QCOMPARE( splitPiece.storageId(), (int)Piece::ChangeStorage );

    // split at mid
    const int Mid = Width/2;
    piece.set( Start, End );
    splitPiece = piece.splitAtLocal( Mid );
    QCOMPARE( piece.start(), Start );
    QCOMPARE( piece.end(), Start+Mid-1 );
    QCOMPARE( piece.storageId(), (int)Piece::ChangeStorage );
    QCOMPARE( splitPiece.start(), Start+Mid );
    QCOMPARE( splitPiece.end(),   End );
    QCOMPARE( splitPiece.storageId(), (int)Piece::ChangeStorage );

    // split at one before width
    piece.set( Start, End );
    splitPiece = piece.splitAtLocal( Width-1 );
    QCOMPARE( piece.start(), Start );
    QCOMPARE( piece.end(), End-1 );
    QCOMPARE( piece.storageId(), (int)Piece::ChangeStorage );
    QCOMPARE( splitPiece.start(), End );
    QCOMPARE( splitPiece.end(),   End );
    QCOMPARE( splitPiece.storageId(), (int)Piece::ChangeStorage );

    // split at start so the split is the full
    piece.set( Start, End );
    splitPiece = piece.splitAtLocal( Width );
    QCOMPARE( piece.start(), Start );
    QCOMPARE( piece.end(), End );
    QCOMPARE( piece.storageId(), (int)Piece::ChangeStorage );
    QVERIFY( !splitPiece.isValid() );
}

void PieceTest::testRemove()
{
    const int Mid = (Start+End)/2;
    const KSection storageSection( Start, End );
    Piece piece( storageSection, Piece::ChangeStorage );

    // remove none at start
    KSection RemoveSection( Start, Start-1  );
    Piece splitPiece = piece.remove( RemoveSection );
    QVERIFY( !piece.isValid() );
    QCOMPARE( splitPiece.start(), Start );
    QCOMPARE( splitPiece.end(),   End );
    QCOMPARE( splitPiece.storageId(), (int)Piece::ChangeStorage );

    // remove one at start
    piece.set( Start, End );
    RemoveSection.set( Start, Start );
    splitPiece = piece.remove( RemoveSection );
    QVERIFY( !piece.isValid() );
    QCOMPARE( splitPiece.start(), Start+1 );
    QCOMPARE( splitPiece.end(),   End );
    QCOMPARE( splitPiece.storageId(), (int)Piece::ChangeStorage );

    // remove many at start
    piece.set( Start, End );
    RemoveSection.set( Start, Mid );
    splitPiece = piece.remove( RemoveSection );
    QVERIFY( !piece.isValid() );
    QCOMPARE( splitPiece.start(), Mid+1 );
    QCOMPARE( splitPiece.end(),   End );
    QCOMPARE( splitPiece.storageId(), (int)Piece::ChangeStorage );

    // remove all except last
    piece.set( Start, End );
    RemoveSection.set( Start, End-1 );
    splitPiece = piece.remove( RemoveSection );
    QVERIFY( !piece.isValid() );
    QCOMPARE( splitPiece.start(), End );
    QCOMPARE( splitPiece.end(),   End );
    QCOMPARE( splitPiece.storageId(), (int)Piece::ChangeStorage );

    // remove at mid
    piece.set( Start, End );
    RemoveSection.set( Mid-1, Mid+1 );
    splitPiece = piece.remove( RemoveSection );
    QCOMPARE( piece.start(), Start );
    QCOMPARE( piece.end(), Mid-2 );
    QCOMPARE( piece.storageId(), (int)Piece::ChangeStorage );
    QCOMPARE( splitPiece.start(), Mid+2 );
    QCOMPARE( splitPiece.end(),   End );
    QCOMPARE( splitPiece.storageId(), (int)Piece::ChangeStorage );

    // remove none at width
    piece.set( Start, End );
    RemoveSection.set( End+1, End );
    splitPiece = piece.remove( RemoveSection );
    QCOMPARE( piece.start(), Start );
    QCOMPARE( piece.end(),   End );
    QCOMPARE( piece.storageId(), (int)Piece::ChangeStorage );
    QVERIFY( !splitPiece.isValid() );

    // remove one at width
    piece.set( Start, End );
    RemoveSection.set( End, End );
    splitPiece = piece.remove( RemoveSection );
    QCOMPARE( piece.start(), Start );
    QCOMPARE( piece.end(),   End-1 );
    QCOMPARE( piece.storageId(), (int)Piece::ChangeStorage );
    QVERIFY( !splitPiece.isValid() );

    // remove many at width
    piece.set( Start, End );
    RemoveSection.set( Mid, End );
    splitPiece = piece.remove( RemoveSection );
    QCOMPARE( piece.start(), Start );
    QCOMPARE( piece.end(),   Mid-1 );
    QCOMPARE( piece.storageId(), (int)Piece::ChangeStorage );
    QVERIFY( !splitPiece.isValid() );

    // remove all except first
    piece.set( Start, End );
    RemoveSection.set( Start+1, End );
    splitPiece = piece.remove( RemoveSection );
    QCOMPARE( piece.start(), Start );
    QCOMPARE( piece.end(),   Start );
    QCOMPARE( piece.storageId(), (int)Piece::ChangeStorage );
    QVERIFY( !splitPiece.isValid() );

    // remove all
    piece.set( Start, End );
    RemoveSection.set( Start, End );
    splitPiece = piece.remove( RemoveSection );
    QVERIFY( !piece.isValid() );
    QVERIFY( !splitPiece.isValid() );
}

void PieceTest::testRemoveLocal()
{
    const int Mid = Width/2;
    const KSection storageSection( Start, End );
    Piece piece( storageSection, Piece::ChangeStorage );

    // remove none at start
    KSection RemoveSection( 0, 0-1  );
    Piece splitPiece = piece.removeLocal( RemoveSection );
    QVERIFY( !piece.isValid() );
    QCOMPARE( splitPiece.start(), Start );
    QCOMPARE( splitPiece.end(),   End );
    QCOMPARE( splitPiece.storageId(), (int)Piece::ChangeStorage );

    // remove one at start
    piece.set( Start, End );
    RemoveSection.set( 0, 0 );
    splitPiece = piece.removeLocal( RemoveSection );
    QVERIFY( !piece.isValid() );
    QCOMPARE( splitPiece.start(), Start+1 );
    QCOMPARE( splitPiece.end(),   End );
    QCOMPARE( splitPiece.storageId(), (int)Piece::ChangeStorage );

    // remove many at start
    piece.set( Start, End );
    RemoveSection.set( 0, Mid );
    splitPiece = piece.removeLocal( RemoveSection );
    QVERIFY( !piece.isValid() );
    QCOMPARE( splitPiece.start(), Start+Mid+1 );
    QCOMPARE( splitPiece.end(),   End );
    QCOMPARE( splitPiece.storageId(), (int)Piece::ChangeStorage );

    // remove all except last
    piece.set( Start, End );
    RemoveSection.set( 0, Width-2 );
    splitPiece = piece.removeLocal( RemoveSection );
    QVERIFY( !piece.isValid() );
    QCOMPARE( splitPiece.start(), End );
    QCOMPARE( splitPiece.end(),   End );
    QCOMPARE( splitPiece.storageId(), (int)Piece::ChangeStorage );

    // remove at mid
    piece.set( Start, End );
    RemoveSection.set( Mid-1, Mid+1 );
    splitPiece = piece.removeLocal( RemoveSection );
    QCOMPARE( piece.start(), Start );
    QCOMPARE( piece.end(), Start+Mid-2 );
    QCOMPARE( piece.storageId(), (int)Piece::ChangeStorage );
    QCOMPARE( splitPiece.start(), Start+Mid+2 );
    QCOMPARE( splitPiece.end(),   End );
    QCOMPARE( splitPiece.storageId(), (int)Piece::ChangeStorage );

    // remove none at width
    piece.set( Start, End );
    RemoveSection.set( Width, Width-1 );
    splitPiece = piece.removeLocal( RemoveSection );
    QCOMPARE( piece.start(), Start );
    QCOMPARE( piece.end(),   End );
    QCOMPARE( piece.storageId(), (int)Piece::ChangeStorage );
    QVERIFY( !splitPiece.isValid() );

    // remove one at width
    piece.set( Start, End );
    RemoveSection.set( Width-1, Width-1 );
    splitPiece = piece.removeLocal( RemoveSection );
    QCOMPARE( piece.start(), Start );
    QCOMPARE( piece.end(),   End-1 );
    QCOMPARE( piece.storageId(), (int)Piece::ChangeStorage );
    QVERIFY( !splitPiece.isValid() );

    // remove many at width
    piece.set( Start, End );
    RemoveSection.set( Mid, Width-1 );
    splitPiece = piece.removeLocal( RemoveSection );
    QCOMPARE( piece.start(), Start );
    QCOMPARE( piece.end(),   Start+Mid-1 );
    QCOMPARE( piece.storageId(), (int)Piece::ChangeStorage );
    QVERIFY( !splitPiece.isValid() );

    // remove all except first
    piece.set( Start, End );
    RemoveSection.set( 1, Width-1 );
    splitPiece = piece.removeLocal( RemoveSection );
    QCOMPARE( piece.start(), Start );
    QCOMPARE( piece.end(),   Start );
    QCOMPARE( piece.storageId(), (int)Piece::ChangeStorage );
    QVERIFY( !splitPiece.isValid() );

    // remove all
    piece.set( Start, End );
    RemoveSection.set( 0, Width-1 );
    splitPiece = piece.removeLocal( RemoveSection );
    QVERIFY( !piece.isValid() );
    QVERIFY( !splitPiece.isValid() );
}

QTEST_MAIN( PieceTest )
