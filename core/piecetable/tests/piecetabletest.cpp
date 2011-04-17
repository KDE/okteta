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

#include "piecetabletest.h"

// test object
#include <piecetable/piecetable.h>
// Qt
#include <QtTest/QtTest>


namespace KPieceTable
{

// local variables
static const Address Start = 15;
static const Address End = 27;
static const Size HalfBaseSize = 50;
static const Size BaseSize = 2 * HalfBaseSize;

static const Size Width = End - Start + 1;

static const Address ChangeStart = 8;
static const Address ChangeEnd = ChangeStart + Width - 1;

void PieceTableTest::testSimpleConstructor()
{
    PieceTable pieceTable;
    QCOMPARE( pieceTable.size(), 0 );
}


void PieceTableTest::testInit()
{
    PieceTable pieceTable;

    pieceTable.init( BaseSize );
    int storageId;
    Address storageOffset;

        bool result = pieceTable.getStorageData( &storageId, &storageOffset, 0 );
        QVERIFY( result );
        QCOMPARE( storageOffset, 0 );
        QCOMPARE( storageId, (int)Piece::OriginalStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, Start );
        QVERIFY( result );
        QCOMPARE( storageOffset, Start );
        QCOMPARE( storageId, (int)Piece::OriginalStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, End );
        QVERIFY( result );
        QCOMPARE( storageOffset, End );
        QCOMPARE( storageId, (int)Piece::OriginalStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, BaseSize-1 );
        QVERIFY( result );
        QCOMPARE( storageOffset, BaseSize-1 );
        QCOMPARE( storageId, (int)Piece::OriginalStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, BaseSize );
        QVERIFY( !result );
}

static void fillWithSize( PieceTable *pieceTable, int count )
{
    pieceTable->init( 0 );
    for( int i=0; i<count; ++i ) pieceTable->insert( 0, BaseSize, BaseSize*i );

}

void PieceTableTest::testInsert()
{
    PieceTable pieceTable;

    int storageId;
    Address storageOffset;
    bool result;


    //inserting to empty
    pieceTable.init( 0 );
    pieceTable.insert( 0, Width, ChangeStart );

        QCOMPARE( pieceTable.size(), Width );
        result = pieceTable.getStorageData( &storageId, &storageOffset, 0 );
        QVERIFY( result );
        QCOMPARE( storageOffset, ChangeStart );
        QCOMPARE( storageId, (int)Piece::ChangeStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, Width-1 );
        QVERIFY( result );
        QCOMPARE( storageOffset, ChangeEnd );
        QCOMPARE( storageId, (int)Piece::ChangeStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, Width );
        QVERIFY( !result );

    // inserting one at the begin
    pieceTable.init( BaseSize );
    pieceTable.insert( 0, Width, ChangeStart );

        QCOMPARE( pieceTable.size(), BaseSize+Width );
        result = pieceTable.getStorageData( &storageId, &storageOffset, 0 );
        QVERIFY( result );
        QCOMPARE( storageOffset, ChangeStart );
        QCOMPARE( storageId, (int)Piece::ChangeStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, Width-1 );
        QVERIFY( result );
        QCOMPARE( storageOffset, ChangeEnd );
        QCOMPARE( storageId, (int)Piece::ChangeStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, Width );
        QVERIFY( result );
        QCOMPARE( storageOffset, 0 );
        QCOMPARE( storageId, (int)Piece::OriginalStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, BaseSize+Width-1 );
        QVERIFY( result );
        QCOMPARE( storageOffset, BaseSize-1 );
        QCOMPARE( storageId, (int)Piece::OriginalStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, BaseSize+Width );
        QVERIFY( !result );

    // inserting one in the middle
    pieceTable.init( BaseSize );
    pieceTable.insert( Start, Width, ChangeStart );

        QCOMPARE( pieceTable.size(), BaseSize+Width );
        result = pieceTable.getStorageData( &storageId, &storageOffset, Start-1 );
        QVERIFY( result );
        QCOMPARE( storageOffset, Start-1 );
        QCOMPARE( storageId, (int)Piece::OriginalStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, Start );
        QVERIFY( result );
        QCOMPARE( storageOffset, ChangeStart );
        QCOMPARE( storageId, (int)Piece::ChangeStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, End );
        QVERIFY( result );
        QCOMPARE( storageOffset, ChangeEnd );
        QCOMPARE( storageId, (int)Piece::ChangeStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, End+1 );
        QVERIFY( result );
        QCOMPARE( storageOffset, Start );
        QCOMPARE( storageId, (int)Piece::OriginalStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, BaseSize+Width-1 );
        QVERIFY( result );
        QCOMPARE( storageOffset, BaseSize-1 );
        QCOMPARE( storageId, (int)Piece::OriginalStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, BaseSize+Width );
        QVERIFY( !result );

    // inserting one at the end
    pieceTable.init( BaseSize );
    pieceTable.insert( BaseSize, Width, ChangeStart );

        QCOMPARE( pieceTable.size(), BaseSize+Width );
        result = pieceTable.getStorageData( &storageId, &storageOffset, BaseSize-1 );
        QVERIFY( result );
        QCOMPARE( storageOffset, BaseSize-1 );
        QCOMPARE( storageId, (int)Piece::OriginalStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, BaseSize );
        QVERIFY( result );
        QCOMPARE( storageOffset, ChangeStart );
        QCOMPARE( storageId, (int)Piece::ChangeStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, BaseSize+Width-1 );
        QVERIFY( result );
        QCOMPARE( storageOffset, ChangeEnd );
        QCOMPARE( storageId, (int)Piece::ChangeStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, BaseSize+Width );
        QVERIFY( !result );

    // inserting a lot:
    // inserting before, after and in another inserted section, two recursions
    // 121110  8 7 6  4 3 2
    // \/\/\/ \/\/\/ \/\/\/
    //  |-9-|  |-5-|  |-1-|
    //  \  /   \  /   \  /
    //   \/     \/     \/
    //    |-----0-------|
    // TODO: do this also in forward sequences
    pieceTable.init( BaseSize ); // 0
    const Address Mid = BaseSize/2;
    Address changeStarts[13];
    Address changeStart = 0;
    changeStarts[1] = changeStart;
    pieceTable.insert( BaseSize, BaseSize, changeStart ); // 1
    changeStart += BaseSize;
    changeStarts[2] = changeStart;
    pieceTable.insert( BaseSize+BaseSize, BaseSize, changeStart ); // 2
    changeStart += BaseSize;
    changeStarts[3] = changeStart;
    pieceTable.insert( BaseSize+Mid, BaseSize, changeStart ); // 3
    changeStart += BaseSize;
    changeStarts[4] = changeStart;
    pieceTable.insert( BaseSize, BaseSize, changeStart ); // 4

    changeStart += BaseSize;
    changeStarts[5] = changeStart;
    pieceTable.insert( Mid, BaseSize, changeStart ); // 5
    changeStart += BaseSize;
    changeStarts[6] = changeStart;
    pieceTable.insert( Mid+BaseSize, BaseSize, changeStart ); // 6
    changeStart += BaseSize;
    changeStarts[7] = changeStart;
    pieceTable.insert( Mid+Mid, BaseSize, changeStart ); // 7
    changeStart += BaseSize;
    changeStarts[8] = changeStart;
    pieceTable.insert( Mid, BaseSize, changeStart ); // 8

    changeStart += BaseSize;
    changeStarts[9] = changeStart;
    pieceTable.insert( 0, BaseSize, changeStart ); // 9
    changeStart += BaseSize;
    changeStarts[10] = changeStart;
    pieceTable.insert( BaseSize, BaseSize, changeStart ); // 10
    changeStart += BaseSize;
    changeStarts[11] = changeStart;
    pieceTable.insert( Mid, BaseSize, changeStart ); // 11
    changeStart += BaseSize;
    changeStarts[12] = changeStart;
    pieceTable.insert( 0, BaseSize, changeStart ); // 12

    // test
    QCOMPARE( pieceTable.size(), BaseSize+12*BaseSize );
    // all borders
    // 12: begin
    Address byteArrayOffset = 0;
    result = pieceTable.getStorageData( &storageId, &storageOffset, byteArrayOffset );
    QVERIFY( result );
    QCOMPARE( storageOffset, changeStarts[12] );
    QCOMPARE( storageId, (int)Piece::ChangeStorage );

    // 12: end
    byteArrayOffset += BaseSize;
    result = pieceTable.getStorageData( &storageId, &storageOffset, byteArrayOffset-1 );
    QVERIFY( result );
    QCOMPARE( storageOffset, changeStarts[12]+BaseSize-1 );
    QCOMPARE( storageId, (int)Piece::ChangeStorage );
    result = pieceTable.getStorageData( &storageId, &storageOffset, byteArrayOffset );
    QVERIFY( result );
    QCOMPARE( storageOffset, changeStarts[9] );
    QCOMPARE( storageId, (int)Piece::ChangeStorage );

    // 11: begin
    byteArrayOffset += HalfBaseSize;
    result = pieceTable.getStorageData( &storageId, &storageOffset, byteArrayOffset-1 );
    QVERIFY( result );
    QCOMPARE( storageOffset, changeStarts[9]+HalfBaseSize-1 );
    QCOMPARE( storageId, (int)Piece::ChangeStorage );
    result = pieceTable.getStorageData( &storageId, &storageOffset, byteArrayOffset );
    QVERIFY( result );
    QCOMPARE( storageOffset, changeStarts[11] );
    QCOMPARE( storageId, (int)Piece::ChangeStorage );

    // 11: end
    byteArrayOffset += BaseSize;
    result = pieceTable.getStorageData( &storageId, &storageOffset, byteArrayOffset-1 );
    QVERIFY( result );
    QCOMPARE( storageOffset, changeStarts[11]+BaseSize-1 );
    QCOMPARE( storageId, (int)Piece::ChangeStorage );
    result = pieceTable.getStorageData( &storageId, &storageOffset, byteArrayOffset );
    QVERIFY( result );
    QCOMPARE( storageOffset, changeStarts[9]+HalfBaseSize );
    QCOMPARE( storageId, (int)Piece::ChangeStorage );

    // 10: begin
    byteArrayOffset += HalfBaseSize;
    result = pieceTable.getStorageData( &storageId, &storageOffset, byteArrayOffset-1 );
    QVERIFY( result );
    QCOMPARE( storageOffset, changeStarts[9]+BaseSize-1 );
    QCOMPARE( storageId, (int)Piece::ChangeStorage );
    result = pieceTable.getStorageData( &storageId, &storageOffset, byteArrayOffset );
    QVERIFY( result );
    QCOMPARE( storageOffset, changeStarts[10] );
    QCOMPARE( storageId, (int)Piece::ChangeStorage );

    // 10: end
    byteArrayOffset += BaseSize;
    result = pieceTable.getStorageData( &storageId, &storageOffset, byteArrayOffset-1 );
    QVERIFY( result );
    QCOMPARE( storageOffset, changeStarts[10]+BaseSize-1 );
    QCOMPARE( storageId, (int)Piece::ChangeStorage );
    result = pieceTable.getStorageData( &storageId, &storageOffset, byteArrayOffset );
    QVERIFY( result );
    QCOMPARE( storageOffset, 0 );
    QCOMPARE( storageId, (int)Piece::OriginalStorage );

    // 8: begin
    byteArrayOffset += HalfBaseSize;
    result = pieceTable.getStorageData( &storageId, &storageOffset, byteArrayOffset-1 );
    QVERIFY( result );
    QCOMPARE( storageOffset, HalfBaseSize-1 );
    QCOMPARE( storageId, (int)Piece::OriginalStorage );
    result = pieceTable.getStorageData( &storageId, &storageOffset, byteArrayOffset );
    QVERIFY( result );
    QCOMPARE( storageOffset, changeStarts[8] );
    QCOMPARE( storageId, (int)Piece::ChangeStorage );

    // 8: end
    byteArrayOffset += BaseSize;
    result = pieceTable.getStorageData( &storageId, &storageOffset, byteArrayOffset-1 );
    QVERIFY( result );
    QCOMPARE( storageOffset, changeStarts[8]+BaseSize-1 );
    QCOMPARE( storageId, (int)Piece::ChangeStorage );
    result = pieceTable.getStorageData( &storageId, &storageOffset, byteArrayOffset );
    QVERIFY( result );
    QCOMPARE( storageOffset, changeStarts[5] );
    QCOMPARE( storageId, (int)Piece::ChangeStorage );

    // 7: begin
    byteArrayOffset += HalfBaseSize;
    result = pieceTable.getStorageData( &storageId, &storageOffset, byteArrayOffset-1 );
    QVERIFY( result );
    QCOMPARE( storageOffset, changeStarts[5]+HalfBaseSize-1 );
    QCOMPARE( storageId, (int)Piece::ChangeStorage );
    result = pieceTable.getStorageData( &storageId, &storageOffset, byteArrayOffset );
    QVERIFY( result );
    QCOMPARE( storageOffset, changeStarts[7] );
    QCOMPARE( storageId, (int)Piece::ChangeStorage );

    // 7: end
    byteArrayOffset += BaseSize;
    result = pieceTable.getStorageData( &storageId, &storageOffset, byteArrayOffset-1 );
    QVERIFY( result );
    QCOMPARE( storageOffset, changeStarts[7]+BaseSize-1 );
    QCOMPARE( storageId, (int)Piece::ChangeStorage );
    result = pieceTable.getStorageData( &storageId, &storageOffset, byteArrayOffset );
    QVERIFY( result );
    QCOMPARE( storageOffset, changeStarts[5]+HalfBaseSize );
    QCOMPARE( storageId, (int)Piece::ChangeStorage );

    // 6: begin
    byteArrayOffset += HalfBaseSize;
    result = pieceTable.getStorageData( &storageId, &storageOffset, byteArrayOffset-1 );
    QVERIFY( result );
    QCOMPARE( storageOffset, changeStarts[5]+BaseSize-1 );
    QCOMPARE( storageId, (int)Piece::ChangeStorage );
    result = pieceTable.getStorageData( &storageId, &storageOffset, byteArrayOffset );
    QVERIFY( result );
    QCOMPARE( storageOffset, changeStarts[6] );
    QCOMPARE( storageId, (int)Piece::ChangeStorage );

    // 6: end
    byteArrayOffset += BaseSize;
    result = pieceTable.getStorageData( &storageId, &storageOffset, byteArrayOffset-1 );
    QVERIFY( result );
    QCOMPARE( storageOffset, changeStarts[6]+BaseSize-1 );
    QCOMPARE( storageId, (int)Piece::ChangeStorage );
    result = pieceTable.getStorageData( &storageId, &storageOffset, byteArrayOffset );
    QVERIFY( result );
    QCOMPARE( storageOffset, HalfBaseSize );
    QCOMPARE( storageId, (int)Piece::OriginalStorage );

    // 4: begin
    byteArrayOffset += HalfBaseSize;
    result = pieceTable.getStorageData( &storageId, &storageOffset, byteArrayOffset-1 );
    QVERIFY( result );
    QCOMPARE( storageOffset, BaseSize-1 );
    QCOMPARE( storageId, (int)Piece::OriginalStorage );
    result = pieceTable.getStorageData( &storageId, &storageOffset, byteArrayOffset );
    QVERIFY( result );
    QCOMPARE( storageOffset, changeStarts[4] );
    QCOMPARE( storageId, (int)Piece::ChangeStorage );

    // 4: end
    byteArrayOffset += BaseSize;
    result = pieceTable.getStorageData( &storageId, &storageOffset, byteArrayOffset-1 );
    QVERIFY( result );
    QCOMPARE( storageOffset, changeStarts[4]+BaseSize-1 );
    QCOMPARE( storageId, (int)Piece::ChangeStorage );
    result = pieceTable.getStorageData( &storageId, &storageOffset, byteArrayOffset );
    QVERIFY( result );
    QCOMPARE( storageOffset, changeStarts[1] );
    QCOMPARE( storageId, (int)Piece::ChangeStorage );

    // 3: begin
    byteArrayOffset += HalfBaseSize;
    result = pieceTable.getStorageData( &storageId, &storageOffset, byteArrayOffset-1 );
    QVERIFY( result );
    QCOMPARE( storageOffset, changeStarts[1]+HalfBaseSize-1 );
    QCOMPARE( storageId, (int)Piece::ChangeStorage );
    result = pieceTable.getStorageData( &storageId, &storageOffset, byteArrayOffset );
    QVERIFY( result );
    QCOMPARE( storageOffset, changeStarts[3] );
    QCOMPARE( storageId, (int)Piece::ChangeStorage );

    // 3: end
    byteArrayOffset += BaseSize;
    result = pieceTable.getStorageData( &storageId, &storageOffset, byteArrayOffset-1 );
    QVERIFY( result );
    QCOMPARE( storageOffset, changeStarts[3]+BaseSize-1 );
    QCOMPARE( storageId, (int)Piece::ChangeStorage );
    result = pieceTable.getStorageData( &storageId, &storageOffset, byteArrayOffset );
    QVERIFY( result );
    QCOMPARE( storageOffset, changeStarts[1]+HalfBaseSize );
    QCOMPARE( storageId, (int)Piece::ChangeStorage );

    // 2: begin
    byteArrayOffset += HalfBaseSize;
    result = pieceTable.getStorageData( &storageId, &storageOffset, byteArrayOffset-1 );
    QVERIFY( result );
    QCOMPARE( storageOffset, changeStarts[1]+BaseSize-1 );
    QCOMPARE( storageId, (int)Piece::ChangeStorage );
    result = pieceTable.getStorageData( &storageId, &storageOffset, byteArrayOffset );
    QVERIFY( result );
    QCOMPARE( storageOffset, changeStarts[2] );
    QCOMPARE( storageId, (int)Piece::ChangeStorage );

    // 2: end
    byteArrayOffset += BaseSize;
    result = pieceTable.getStorageData( &storageId, &storageOffset, byteArrayOffset-1 );
    QVERIFY( result );
    QCOMPARE( storageOffset, changeStarts[2]+BaseSize-1 );
    QCOMPARE( storageId, (int)Piece::ChangeStorage );

    result = pieceTable.getStorageData( &storageId, &storageOffset, byteArrayOffset );
    QVERIFY( !result );
}

void PieceTableTest::testRemove()
{
    PieceTable pieceTable;

    Address changeStarts[6];
    int storageId;
    Address storageOffset;
    bool result;

    // removing at begin
    pieceTable.init( BaseSize );
    pieceTable.remove( AddressRange(0, Start-1) );

        QCOMPARE( pieceTable.size(), BaseSize-Start );
        result = pieceTable.getStorageData( &storageId, &storageOffset, 0 );
        QVERIFY( result );
        QCOMPARE( storageOffset, Start );
        QCOMPARE( storageId, (int)Piece::OriginalStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, BaseSize-Start-1 );
        QVERIFY( result );
        QCOMPARE( storageOffset, BaseSize-1 );
        QCOMPARE( storageId, (int)Piece::OriginalStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, BaseSize-Start );
        QVERIFY( !result );

    // removing at middle
    pieceTable.init( BaseSize );
    pieceTable.remove( AddressRange(Start, End) );

        QCOMPARE( pieceTable.size(), BaseSize-Width );
        result = pieceTable.getStorageData( &storageId, &storageOffset, Start-1 );
        QVERIFY( result );
        QCOMPARE( storageOffset, Start-1 );
        QCOMPARE( storageId, (int)Piece::OriginalStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, Start );
        QVERIFY( result );
        QCOMPARE( storageOffset, End+1 );
        QCOMPARE( storageId, (int)Piece::OriginalStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, BaseSize-Width-1 );
        QVERIFY( result );
        QCOMPARE( storageOffset, BaseSize-1 );
        QCOMPARE( storageId, (int)Piece::OriginalStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, BaseSize-Width );
        QVERIFY( !result );

    // removing at end
    pieceTable.init( BaseSize );
    pieceTable.remove( AddressRange(End+1, BaseSize-1) );

        QCOMPARE( pieceTable.size(), End+1 );
        result = pieceTable.getStorageData( &storageId, &storageOffset, End );
        QVERIFY( result );
        QCOMPARE( storageOffset, End );
        QCOMPARE( storageId, (int)Piece::OriginalStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, End+1 );
        QVERIFY( !result );

    // removing all
    pieceTable.init( BaseSize );
    pieceTable.remove( AddressRange::fromWidth(BaseSize) );

        QCOMPARE( pieceTable.size(), 0 );
        result = pieceTable.getStorageData( &storageId, &storageOffset, 0 );
        QVERIFY( !result );


    // removing a lot:
    const int pieceCount = 5;
    const int mid = (pieceCount+1)/2;
    const Address midPieceOffset = BaseSize * (mid-1);
    const Size fullSize = pieceCount * BaseSize;
    // for this five equally sized pieces are inserted, reverse to offset in ChangeStore
    for( int i=0; i<pieceCount; ++i ) changeStarts[pieceCount-i] = BaseSize*i;

    // removing inside a piece in the middle
    fillWithSize( &pieceTable, pieceCount );
    pieceTable.remove( AddressRange::fromWidth(midPieceOffset+Start, Width) );

        QCOMPARE( pieceTable.size(), fullSize-Width );

        result = pieceTable.getStorageData( &storageId, &storageOffset, midPieceOffset+Start-1 );
        QVERIFY( result );
        QCOMPARE( storageOffset, changeStarts[mid]+Start-1 );
        QCOMPARE( storageId, (int)Piece::ChangeStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, midPieceOffset+Start );
        QVERIFY( result );
        QCOMPARE( storageOffset, changeStarts[mid]+End+1 );
        QCOMPARE( storageId, (int)Piece::ChangeStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, fullSize-Width-1 );
        QVERIFY( result );
        QCOMPARE( storageOffset, changeStarts[pieceCount]+BaseSize-1 );
        QCOMPARE( storageId, (int)Piece::ChangeStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, fullSize-Width );
        QVERIFY( !result );

    // removing start of a piece in the middle
    fillWithSize( &pieceTable, pieceCount );
    pieceTable.remove( AddressRange::fromWidth(midPieceOffset, Start) );

        QCOMPARE( pieceTable.size(), fullSize-Start );

        result = pieceTable.getStorageData( &storageId, &storageOffset, midPieceOffset-1 );
        QVERIFY( result );
        QCOMPARE( storageOffset, changeStarts[mid-1]+BaseSize-1 );
        QCOMPARE( storageId, (int)Piece::ChangeStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, midPieceOffset );
        QVERIFY( result );
        QCOMPARE( storageOffset, changeStarts[mid]+Start );
        QCOMPARE( storageId, (int)Piece::ChangeStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, fullSize-Start-1 );
        QVERIFY( result );
        QCOMPARE( storageOffset, changeStarts[pieceCount]+BaseSize-1 );
        QCOMPARE( storageId, (int)Piece::ChangeStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, fullSize-Start );
        QVERIFY( !result );

    // removing end of a piece in the middle
    fillWithSize( &pieceTable, pieceCount );
    pieceTable.remove( AddressRange::fromWidth(midPieceOffset+End+1, BaseSize-(End+1)) );

        QCOMPARE( pieceTable.size(), fullSize-(BaseSize-End-1) );

        result = pieceTable.getStorageData( &storageId, &storageOffset, midPieceOffset+End );
        QVERIFY( result );
        QCOMPARE( storageOffset, changeStarts[mid]+End );
        QCOMPARE( storageId, (int)Piece::ChangeStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, midPieceOffset+End+1 );
        QVERIFY( result );
        QCOMPARE( storageOffset, changeStarts[mid+1] );
        QCOMPARE( storageId, (int)Piece::ChangeStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, fullSize-(BaseSize-End-1)-1 );
        QVERIFY( result );
        QCOMPARE( storageOffset, changeStarts[pieceCount]+BaseSize-1 );
        QCOMPARE( storageId, (int)Piece::ChangeStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, fullSize-(BaseSize-End-1) );
        QVERIFY( !result );

    // removing whole piece in the middle
    fillWithSize( &pieceTable, pieceCount );
    pieceTable.remove( AddressRange::fromWidth(midPieceOffset, BaseSize) );

        QCOMPARE( pieceTable.size(), fullSize-BaseSize );

        result = pieceTable.getStorageData( &storageId, &storageOffset, midPieceOffset-1 );
        QVERIFY( result );
        QCOMPARE( storageOffset, changeStarts[mid-1]+BaseSize-1 );
        QCOMPARE( storageId, (int)Piece::ChangeStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, midPieceOffset );
        QVERIFY( result );
        QCOMPARE( storageOffset, changeStarts[mid+1] );
        QCOMPARE( storageId, (int)Piece::ChangeStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, fullSize-BaseSize-1 );
        QVERIFY( result );
        QCOMPARE( storageOffset, changeStarts[pieceCount]+BaseSize-1 );
        QCOMPARE( storageId, (int)Piece::ChangeStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, fullSize-BaseSize );
        QVERIFY( !result );

    // removing whole piece and start of next in the middke
    fillWithSize( &pieceTable, pieceCount );
    pieceTable.remove( AddressRange::fromWidth(midPieceOffset, BaseSize+Start) );

        QCOMPARE( pieceTable.size(), fullSize-BaseSize-Start );

        result = pieceTable.getStorageData( &storageId, &storageOffset, midPieceOffset-1 );
        QVERIFY( result );
        QCOMPARE( storageOffset, changeStarts[mid-1]+BaseSize-1 );
        QCOMPARE( storageId, (int)Piece::ChangeStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, midPieceOffset );
        QVERIFY( result );
        QCOMPARE( storageOffset, changeStarts[mid+1]+Start );
        QCOMPARE( storageId, (int)Piece::ChangeStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, fullSize-BaseSize-Start-1 );
        QVERIFY( result );
        QCOMPARE( storageOffset, changeStarts[pieceCount]+BaseSize-1 );
        QCOMPARE( storageId, (int)Piece::ChangeStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, fullSize-BaseSize-Start );
        QVERIFY( !result );

    // removing whole piece and end of previous in the middle
    fillWithSize( &pieceTable, pieceCount );
    pieceTable.remove( AddressRange::fromWidth(midPieceOffset-(BaseSize-End-1), BaseSize+BaseSize-(End+1)) );

        QCOMPARE( pieceTable.size(), fullSize-BaseSize-(BaseSize-End-1) );

        result = pieceTable.getStorageData( &storageId, &storageOffset, midPieceOffset-(BaseSize-End-1)-1 );
        QVERIFY( result );
        QCOMPARE( storageOffset, changeStarts[mid-1]+End );
        QCOMPARE( storageId, (int)Piece::ChangeStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, midPieceOffset-(BaseSize-End-1) );
        QVERIFY( result );
        QCOMPARE( storageOffset, changeStarts[mid+1] );
        QCOMPARE( storageId, (int)Piece::ChangeStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, fullSize-BaseSize-(BaseSize-End-1)-1 );
        QVERIFY( result );
        QCOMPARE( storageOffset, changeStarts[pieceCount]+BaseSize-1 );
        QCOMPARE( storageId, (int)Piece::ChangeStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, fullSize-BaseSize-(BaseSize-End-1) );
        QVERIFY( !result );

    // removing end of previous, whole and start of next in the middle
    fillWithSize( &pieceTable, pieceCount );
    pieceTable.remove( AddressRange::fromWidth(midPieceOffset-(BaseSize-End-1), Start+BaseSize+BaseSize-(End+1)) );

        QCOMPARE( pieceTable.size(), fullSize-BaseSize-(BaseSize-End-1)-Start );

        result = pieceTable.getStorageData( &storageId, &storageOffset, midPieceOffset-(BaseSize-End-1)-1 );
        QVERIFY( result );
        QCOMPARE( storageOffset, changeStarts[mid-1]+BaseSize-(BaseSize-End-1)-1 );
        QCOMPARE( storageId, (int)Piece::ChangeStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, midPieceOffset-(BaseSize-End-1) );
        QVERIFY( result );
        QCOMPARE( storageOffset, changeStarts[mid+1]+Start );
        QCOMPARE( storageId, (int)Piece::ChangeStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, fullSize-BaseSize-(BaseSize-End-1)-Start-1 );
        QVERIFY( result );
        QCOMPARE( storageOffset, changeStarts[pieceCount]+BaseSize-1 );
        QCOMPARE( storageId, (int)Piece::ChangeStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, fullSize-BaseSize-(BaseSize-End-1)-Start );
        QVERIFY( !result );

    // removing start of piece at start
    fillWithSize( &pieceTable, pieceCount );
    pieceTable.remove( AddressRange::fromWidth(Start) );

        QCOMPARE( pieceTable.size(), fullSize-Start );

        result = pieceTable.getStorageData( &storageId, &storageOffset, 0 );
        QVERIFY( result );
        QCOMPARE( storageOffset, changeStarts[1]+Start );
        QCOMPARE( storageId, (int)Piece::ChangeStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, fullSize-Start-1 );
        QVERIFY( result );
        QCOMPARE( storageOffset, changeStarts[pieceCount]+BaseSize-1 );
        QCOMPARE( storageId, (int)Piece::ChangeStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, fullSize-Start );
        QVERIFY( !result );

    // removing whole piece at start
    fillWithSize( &pieceTable, pieceCount );
    pieceTable.remove( AddressRange::fromWidth(BaseSize) );

        QCOMPARE( pieceTable.size(), fullSize-BaseSize );

        result = pieceTable.getStorageData( &storageId, &storageOffset, 0 );
        QVERIFY( result );
        QCOMPARE( storageOffset, changeStarts[2] );
        QCOMPARE( storageId, (int)Piece::ChangeStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, fullSize-BaseSize-1 );
        QVERIFY( result );
        QCOMPARE( storageOffset, changeStarts[pieceCount]+BaseSize-1 );
        QCOMPARE( storageId, (int)Piece::ChangeStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, fullSize-BaseSize );
        QVERIFY( !result );

    // removing whole piece and start of next at start
    fillWithSize( &pieceTable, pieceCount );
    pieceTable.remove( AddressRange::fromWidth(BaseSize+Start) );

        QCOMPARE( pieceTable.size(), fullSize-BaseSize-Start );

        result = pieceTable.getStorageData( &storageId, &storageOffset, 0 );
        QVERIFY( result );
        QCOMPARE( storageOffset, changeStarts[2]+Start );
        QCOMPARE( storageId, (int)Piece::ChangeStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, fullSize-BaseSize-Start-1 );
        QVERIFY( result );
        QCOMPARE( storageOffset, changeStarts[pieceCount]+BaseSize-1 );
        QCOMPARE( storageId, (int)Piece::ChangeStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, fullSize-BaseSize-Start );
        QVERIFY( !result );

    // removing end of piece at end
    fillWithSize( &pieceTable, pieceCount );
    pieceTable.remove( AddressRange::fromWidth(fullSize-BaseSize+End+1, BaseSize-(End+1)) );

        QCOMPARE( pieceTable.size(), fullSize-(BaseSize-End-1) );

        result = pieceTable.getStorageData( &storageId, &storageOffset, fullSize-BaseSize+End );
        QVERIFY( result );
        QCOMPARE( storageOffset, changeStarts[pieceCount]+End );
        QCOMPARE( storageId, (int)Piece::ChangeStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, fullSize-(BaseSize-End-1) );
        QVERIFY( !result );

    // removing whole piece at end
    fillWithSize( &pieceTable, pieceCount );
    pieceTable.remove( AddressRange::fromWidth(fullSize-BaseSize, BaseSize) );

        QCOMPARE( pieceTable.size(), fullSize-BaseSize );

        result = pieceTable.getStorageData( &storageId, &storageOffset, fullSize-BaseSize-1 );
        QVERIFY( result );
        QCOMPARE( storageOffset, changeStarts[pieceCount-1]+BaseSize-1 );
        QCOMPARE( storageId, (int)Piece::ChangeStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, fullSize-BaseSize );
        QVERIFY( !result );

    // removing whole piece and end of previous at end
    fillWithSize( &pieceTable, pieceCount );
    pieceTable.remove( AddressRange::fromWidth(fullSize-BaseSize-(BaseSize-End-1), BaseSize+BaseSize-(End+1)) );

        QCOMPARE( pieceTable.size(), fullSize-BaseSize-(BaseSize-End-1) );

        result = pieceTable.getStorageData( &storageId, &storageOffset, fullSize-BaseSize-(BaseSize-End-1)-1 );
        QVERIFY( result );
        QCOMPARE( storageOffset, changeStarts[pieceCount-1]+End );
        QCOMPARE( storageId, (int)Piece::ChangeStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, fullSize-BaseSize-(BaseSize-End-1) );
        QVERIFY( !result );

    // removing all 
    fillWithSize( &pieceTable, pieceCount );
    pieceTable.remove( AddressRange::fromWidth(fullSize) );

        QCOMPARE( pieceTable.size(), 0 );
        result = pieceTable.getStorageData( &storageId, &storageOffset, 0 );
        QVERIFY( !result );
}

void PieceTableTest::testSwap()
{
    PieceTable pieceTable;

    int storageId;
    Address storageOffset;
    bool result;

    // moving end at begin
    pieceTable.init( BaseSize );
    pieceTable.swap( 0, AddressRange::fromWidth(End+1, BaseSize-(End+1)) );

        QCOMPARE( pieceTable.size(), BaseSize );

        result = pieceTable.getStorageData( &storageId, &storageOffset, 0 );
        QVERIFY( result );
        QCOMPARE( storageOffset, End+1 );
        QCOMPARE( storageId, (int)Piece::OriginalStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, BaseSize-End-2 );
        QVERIFY( result );
        QCOMPARE( storageOffset, BaseSize-1 );
        QCOMPARE( storageId, (int)Piece::OriginalStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, BaseSize-End-1 );
        QVERIFY( result );
        QCOMPARE( storageOffset, 0 );
        QCOMPARE( storageId, (int)Piece::OriginalStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, BaseSize-1 );
        QVERIFY( result );
        QCOMPARE( storageOffset, End );
        QCOMPARE( storageId, (int)Piece::OriginalStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, BaseSize );
        QVERIFY( !result );

    // moving end at mid
    pieceTable.init( BaseSize );
    pieceTable.swap( Start, AddressRange::fromWidth(End+1, BaseSize-(End+1)) );

        QCOMPARE( pieceTable.size(), BaseSize );

        result = pieceTable.getStorageData( &storageId, &storageOffset, Start-1 );
        QVERIFY( result );
        QCOMPARE( storageOffset, Start-1 );
        QCOMPARE( storageId, (int)Piece::OriginalStorage );

        QCOMPARE( pieceTable.size(), BaseSize );
        result = pieceTable.getStorageData( &storageId, &storageOffset, Start );
        QVERIFY( result );
        QCOMPARE( storageOffset, End+1 );
        QCOMPARE( storageId, (int)Piece::OriginalStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, Start+BaseSize-End-2 );
        QVERIFY( result );
        QCOMPARE( storageOffset, BaseSize-1 );
        QCOMPARE( storageId, (int)Piece::OriginalStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, Start+BaseSize-End-1 );
        QVERIFY( result );
        QCOMPARE( storageOffset, Start );
        QCOMPARE( storageId, (int)Piece::OriginalStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, BaseSize-1 );
        QVERIFY( result );
        QCOMPARE( storageOffset, End );
        QCOMPARE( storageId, (int)Piece::OriginalStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, BaseSize );
        QVERIFY( !result );

    // moving mid at begin
    pieceTable.init( BaseSize );
    pieceTable.swap( 0, AddressRange::fromWidth(Start, Width) );

        QCOMPARE( pieceTable.size(), BaseSize );

        QCOMPARE( pieceTable.size(), BaseSize );
        result = pieceTable.getStorageData( &storageId, &storageOffset, 0 );
        QVERIFY( result );
        QCOMPARE( storageOffset, Start );
        QCOMPARE( storageId, (int)Piece::OriginalStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, Width-1 );
        QVERIFY( result );
        QCOMPARE( storageOffset, End );
        QCOMPARE( storageId, (int)Piece::OriginalStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, Width );
        QVERIFY( result );
        QCOMPARE( storageOffset, 0 );
        QCOMPARE( storageId, (int)Piece::OriginalStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, End );
        QVERIFY( result );
        QCOMPARE( storageOffset, Start-1 );
        QCOMPARE( storageId, (int)Piece::OriginalStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, End+1 );
        QVERIFY( result );
        QCOMPARE( storageOffset, End+1 );
        QCOMPARE( storageId, (int)Piece::OriginalStorage );

    // moving mid at mid
    pieceTable.init( BaseSize );
    Address mid = (End+Start)/2;
    pieceTable.swap( Start, AddressRange::fromWidth(mid, End-mid+1) );

        QCOMPARE( pieceTable.size(), BaseSize );

        result = pieceTable.getStorageData( &storageId, &storageOffset, Start-1 );
        QVERIFY( result );
        QCOMPARE( storageOffset, Start-1 );
        QCOMPARE( storageId, (int)Piece::OriginalStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, Start );
        QVERIFY( result );
        QCOMPARE( storageOffset, mid );
        QCOMPARE( storageId, (int)Piece::OriginalStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, Start+End-mid );
        QVERIFY( result );
        QCOMPARE( storageOffset, End );
        QCOMPARE( storageId, (int)Piece::OriginalStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, Start+End-mid+1 );
        QVERIFY( result );
        QCOMPARE( storageOffset, Start );
        QCOMPARE( storageId, (int)Piece::OriginalStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, End );
        QVERIFY( result );
        QCOMPARE( storageOffset, mid-1 );
        QCOMPARE( storageId, (int)Piece::OriginalStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, End+1 );
        QVERIFY( result );
        QCOMPARE( storageOffset, End+1 );
        QCOMPARE( storageId, (int)Piece::OriginalStorage );

    // moving a lot:
    const int pieceCount = 5;
    const Size fullSize = pieceCount * BaseSize;
    const int midPiece = (pieceCount+1)/2;
    const Address midPieceOffset = BaseSize * (midPiece-1);
    Address changeStarts[6];
    for( int i=0; i<pieceCount; ++i ) changeStarts[pieceCount-i] = BaseSize*i;

    // moving start of piece at start
    fillWithSize( &pieceTable, pieceCount );
    pieceTable.swap( Start, AddressRange::fromWidth(mid, End-mid+1) );

        QCOMPARE( pieceTable.size(), fullSize );
    //TODO: add other tests
}

}

QTEST_MAIN( KPieceTable::PieceTableTest )
