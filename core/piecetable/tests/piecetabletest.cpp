/***************************************************************************
                          piecetabletest.cpp  -  description
                             -------------------
    begin                : Fri Jan 18 2008
    copyright            : 2008 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License version 2 as published by the Free Software Foundation.       *
 *                                                                         *
 ***************************************************************************/


#include "piecetabletest.h"

// test object
#include <piecetable/piecetable.h>
// Qt
#include <QtTest/QtTest>

using namespace KHE;

// local variables
static const int Start = 15;
static const int End = 27;
static const int HalfSize = 50;
static const int Size = 2 * HalfSize;

static const int Width = End - Start + 1;

static const int ChangeStart = 8;
static const int ChangeEnd = ChangeStart + Width - 1;

namespace KPieceTable
{

void PieceTableTest::testSimpleConstructor()
{
    PieceTable pieceTable;
    QCOMPARE( pieceTable.size(), 0 );
}


void PieceTableTest::testInit()
{
    PieceTable pieceTable;

    pieceTable.init( Size );
    int storageId;
    int storageOffset;

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

        result = pieceTable.getStorageData( &storageId, &storageOffset, Size-1 );
        QVERIFY( result );
        QCOMPARE( storageOffset, Size-1 );
        QCOMPARE( storageId, (int)Piece::OriginalStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, Size );
        QVERIFY( !result );
}

static void fillWithSize( PieceTable *pieceTable, int count )
{
    pieceTable->init( 0 );
    for( int i=0; i<count; ++i ) pieceTable->insert( 0, Size, Size*i );

}

void PieceTableTest::testInsert()
{
    PieceTable pieceTable;

    int storageId;
    int storageOffset;
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
    pieceTable.init( Size );
    pieceTable.insert( 0, Width, ChangeStart );

        QCOMPARE( pieceTable.size(), Size+Width );
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

        result = pieceTable.getStorageData( &storageId, &storageOffset, Size+Width-1 );
        QVERIFY( result );
        QCOMPARE( storageOffset, Size-1 );
        QCOMPARE( storageId, (int)Piece::OriginalStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, Size+Width );
        QVERIFY( !result );

    // inserting one in the middle
    pieceTable.init( Size );
    pieceTable.insert( Start, Width, ChangeStart );

        QCOMPARE( pieceTable.size(), Size+Width );
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

        result = pieceTable.getStorageData( &storageId, &storageOffset, Size+Width-1 );
        QVERIFY( result );
        QCOMPARE( storageOffset, Size-1 );
        QCOMPARE( storageId, (int)Piece::OriginalStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, Size+Width );
        QVERIFY( !result );

    // inserting one at the end
    pieceTable.init( Size );
    pieceTable.insert( Size, Width, ChangeStart );

        QCOMPARE( pieceTable.size(), Size+Width );
        result = pieceTable.getStorageData( &storageId, &storageOffset, Size-1 );
        QVERIFY( result );
        QCOMPARE( storageOffset, Size-1 );
        QCOMPARE( storageId, (int)Piece::OriginalStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, Size );
        QVERIFY( result );
        QCOMPARE( storageOffset, ChangeStart );
        QCOMPARE( storageId, (int)Piece::ChangeStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, Size+Width-1 );
        QVERIFY( result );
        QCOMPARE( storageOffset, ChangeEnd );
        QCOMPARE( storageId, (int)Piece::ChangeStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, Size+Width );
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
    pieceTable.init( Size ); // 0
    const int Mid = Size/2;
    int changeStarts[13];
    int changeStart = 0;
    changeStarts[1] = changeStart;
    pieceTable.insert( Size, Size, changeStart ); // 1
    changeStart += Size;
    changeStarts[2] = changeStart;
    pieceTable.insert( Size+Size, Size, changeStart ); // 2
    changeStart += Size;
    changeStarts[3] = changeStart;
    pieceTable.insert( Size+Mid, Size, changeStart ); // 3
    changeStart += Size;
    changeStarts[4] = changeStart;
    pieceTable.insert( Size, Size, changeStart ); // 4

    changeStart += Size;
    changeStarts[5] = changeStart;
    pieceTable.insert( Mid, Size, changeStart ); // 5
    changeStart += Size;
    changeStarts[6] = changeStart;
    pieceTable.insert( Mid+Size, Size, changeStart ); // 6
    changeStart += Size;
    changeStarts[7] = changeStart;
    pieceTable.insert( Mid+Mid, Size, changeStart ); // 7
    changeStart += Size;
    changeStarts[8] = changeStart;
    pieceTable.insert( Mid, Size, changeStart ); // 8

    changeStart += Size;
    changeStarts[9] = changeStart;
    pieceTable.insert( 0, Size, changeStart ); // 9
    changeStart += Size;
    changeStarts[10] = changeStart;
    pieceTable.insert( Size, Size, changeStart ); // 10
    changeStart += Size;
    changeStarts[11] = changeStart;
    pieceTable.insert( Mid, Size, changeStart ); // 11
    changeStart += Size;
    changeStarts[12] = changeStart;
    pieceTable.insert( 0, Size, changeStart ); // 12

    // test
    QCOMPARE( pieceTable.size(), Size+12*Size );
    // all borders
    // 12: begin
    int byteArrayOffset = 0;
    result = pieceTable.getStorageData( &storageId, &storageOffset, byteArrayOffset );
    QVERIFY( result );
    QCOMPARE( storageOffset, changeStarts[12] );
    QCOMPARE( storageId, (int)Piece::ChangeStorage );

    // 12: end
    byteArrayOffset += Size;
    result = pieceTable.getStorageData( &storageId, &storageOffset, byteArrayOffset-1 );
    QVERIFY( result );
    QCOMPARE( storageOffset, changeStarts[12]+Size-1 );
    QCOMPARE( storageId, (int)Piece::ChangeStorage );
    result = pieceTable.getStorageData( &storageId, &storageOffset, byteArrayOffset );
    QVERIFY( result );
    QCOMPARE( storageOffset, changeStarts[9] );
    QCOMPARE( storageId, (int)Piece::ChangeStorage );

    // 11: begin
    byteArrayOffset += HalfSize;
    result = pieceTable.getStorageData( &storageId, &storageOffset, byteArrayOffset-1 );
    QVERIFY( result );
    QCOMPARE( storageOffset, changeStarts[9]+HalfSize-1 );
    QCOMPARE( storageId, (int)Piece::ChangeStorage );
    result = pieceTable.getStorageData( &storageId, &storageOffset, byteArrayOffset );
    QVERIFY( result );
    QCOMPARE( storageOffset, changeStarts[11] );
    QCOMPARE( storageId, (int)Piece::ChangeStorage );

    // 11: end
    byteArrayOffset += Size;
    result = pieceTable.getStorageData( &storageId, &storageOffset, byteArrayOffset-1 );
    QVERIFY( result );
    QCOMPARE( storageOffset, changeStarts[11]+Size-1 );
    QCOMPARE( storageId, (int)Piece::ChangeStorage );
    result = pieceTable.getStorageData( &storageId, &storageOffset, byteArrayOffset );
    QVERIFY( result );
    QCOMPARE( storageOffset, changeStarts[9]+HalfSize );
    QCOMPARE( storageId, (int)Piece::ChangeStorage );

    // 10: begin
    byteArrayOffset += HalfSize;
    result = pieceTable.getStorageData( &storageId, &storageOffset, byteArrayOffset-1 );
    QVERIFY( result );
    QCOMPARE( storageOffset, changeStarts[9]+Size-1 );
    QCOMPARE( storageId, (int)Piece::ChangeStorage );
    result = pieceTable.getStorageData( &storageId, &storageOffset, byteArrayOffset );
    QVERIFY( result );
    QCOMPARE( storageOffset, changeStarts[10] );
    QCOMPARE( storageId, (int)Piece::ChangeStorage );

    // 10: end
    byteArrayOffset += Size;
    result = pieceTable.getStorageData( &storageId, &storageOffset, byteArrayOffset-1 );
    QVERIFY( result );
    QCOMPARE( storageOffset, changeStarts[10]+Size-1 );
    QCOMPARE( storageId, (int)Piece::ChangeStorage );
    result = pieceTable.getStorageData( &storageId, &storageOffset, byteArrayOffset );
    QVERIFY( result );
    QCOMPARE( storageOffset, 0 );
    QCOMPARE( storageId, (int)Piece::OriginalStorage );

    // 8: begin
    byteArrayOffset += HalfSize;
    result = pieceTable.getStorageData( &storageId, &storageOffset, byteArrayOffset-1 );
    QVERIFY( result );
    QCOMPARE( storageOffset, HalfSize-1 );
    QCOMPARE( storageId, (int)Piece::OriginalStorage );
    result = pieceTable.getStorageData( &storageId, &storageOffset, byteArrayOffset );
    QVERIFY( result );
    QCOMPARE( storageOffset, changeStarts[8] );
    QCOMPARE( storageId, (int)Piece::ChangeStorage );

    // 8: end
    byteArrayOffset += Size;
    result = pieceTable.getStorageData( &storageId, &storageOffset, byteArrayOffset-1 );
    QVERIFY( result );
    QCOMPARE( storageOffset, changeStarts[8]+Size-1 );
    QCOMPARE( storageId, (int)Piece::ChangeStorage );
    result = pieceTable.getStorageData( &storageId, &storageOffset, byteArrayOffset );
    QVERIFY( result );
    QCOMPARE( storageOffset, changeStarts[5] );
    QCOMPARE( storageId, (int)Piece::ChangeStorage );

    // 7: begin
    byteArrayOffset += HalfSize;
    result = pieceTable.getStorageData( &storageId, &storageOffset, byteArrayOffset-1 );
    QVERIFY( result );
    QCOMPARE( storageOffset, changeStarts[5]+HalfSize-1 );
    QCOMPARE( storageId, (int)Piece::ChangeStorage );
    result = pieceTable.getStorageData( &storageId, &storageOffset, byteArrayOffset );
    QVERIFY( result );
    QCOMPARE( storageOffset, changeStarts[7] );
    QCOMPARE( storageId, (int)Piece::ChangeStorage );

    // 7: end
    byteArrayOffset += Size;
    result = pieceTable.getStorageData( &storageId, &storageOffset, byteArrayOffset-1 );
    QVERIFY( result );
    QCOMPARE( storageOffset, changeStarts[7]+Size-1 );
    QCOMPARE( storageId, (int)Piece::ChangeStorage );
    result = pieceTable.getStorageData( &storageId, &storageOffset, byteArrayOffset );
    QVERIFY( result );
    QCOMPARE( storageOffset, changeStarts[5]+HalfSize );
    QCOMPARE( storageId, (int)Piece::ChangeStorage );

    // 6: begin
    byteArrayOffset += HalfSize;
    result = pieceTable.getStorageData( &storageId, &storageOffset, byteArrayOffset-1 );
    QVERIFY( result );
    QCOMPARE( storageOffset, changeStarts[5]+Size-1 );
    QCOMPARE( storageId, (int)Piece::ChangeStorage );
    result = pieceTable.getStorageData( &storageId, &storageOffset, byteArrayOffset );
    QVERIFY( result );
    QCOMPARE( storageOffset, changeStarts[6] );
    QCOMPARE( storageId, (int)Piece::ChangeStorage );

    // 6: end
    byteArrayOffset += Size;
    result = pieceTable.getStorageData( &storageId, &storageOffset, byteArrayOffset-1 );
    QVERIFY( result );
    QCOMPARE( storageOffset, changeStarts[6]+Size-1 );
    QCOMPARE( storageId, (int)Piece::ChangeStorage );
    result = pieceTable.getStorageData( &storageId, &storageOffset, byteArrayOffset );
    QVERIFY( result );
    QCOMPARE( storageOffset, HalfSize );
    QCOMPARE( storageId, (int)Piece::OriginalStorage );

    // 4: begin
    byteArrayOffset += HalfSize;
    result = pieceTable.getStorageData( &storageId, &storageOffset, byteArrayOffset-1 );
    QVERIFY( result );
    QCOMPARE( storageOffset, Size-1 );
    QCOMPARE( storageId, (int)Piece::OriginalStorage );
    result = pieceTable.getStorageData( &storageId, &storageOffset, byteArrayOffset );
    QVERIFY( result );
    QCOMPARE( storageOffset, changeStarts[4] );
    QCOMPARE( storageId, (int)Piece::ChangeStorage );

    // 4: end
    byteArrayOffset += Size;
    result = pieceTable.getStorageData( &storageId, &storageOffset, byteArrayOffset-1 );
    QVERIFY( result );
    QCOMPARE( storageOffset, changeStarts[4]+Size-1 );
    QCOMPARE( storageId, (int)Piece::ChangeStorage );
    result = pieceTable.getStorageData( &storageId, &storageOffset, byteArrayOffset );
    QVERIFY( result );
    QCOMPARE( storageOffset, changeStarts[1] );
    QCOMPARE( storageId, (int)Piece::ChangeStorage );

    // 3: begin
    byteArrayOffset += HalfSize;
    result = pieceTable.getStorageData( &storageId, &storageOffset, byteArrayOffset-1 );
    QVERIFY( result );
    QCOMPARE( storageOffset, changeStarts[1]+HalfSize-1 );
    QCOMPARE( storageId, (int)Piece::ChangeStorage );
    result = pieceTable.getStorageData( &storageId, &storageOffset, byteArrayOffset );
    QVERIFY( result );
    QCOMPARE( storageOffset, changeStarts[3] );
    QCOMPARE( storageId, (int)Piece::ChangeStorage );

    // 3: end
    byteArrayOffset += Size;
    result = pieceTable.getStorageData( &storageId, &storageOffset, byteArrayOffset-1 );
    QVERIFY( result );
    QCOMPARE( storageOffset, changeStarts[3]+Size-1 );
    QCOMPARE( storageId, (int)Piece::ChangeStorage );
    result = pieceTable.getStorageData( &storageId, &storageOffset, byteArrayOffset );
    QVERIFY( result );
    QCOMPARE( storageOffset, changeStarts[1]+HalfSize );
    QCOMPARE( storageId, (int)Piece::ChangeStorage );

    // 2: begin
    byteArrayOffset += HalfSize;
    result = pieceTable.getStorageData( &storageId, &storageOffset, byteArrayOffset-1 );
    QVERIFY( result );
    QCOMPARE( storageOffset, changeStarts[1]+Size-1 );
    QCOMPARE( storageId, (int)Piece::ChangeStorage );
    result = pieceTable.getStorageData( &storageId, &storageOffset, byteArrayOffset );
    QVERIFY( result );
    QCOMPARE( storageOffset, changeStarts[2] );
    QCOMPARE( storageId, (int)Piece::ChangeStorage );

    // 2: end
    byteArrayOffset += Size;
    result = pieceTable.getStorageData( &storageId, &storageOffset, byteArrayOffset-1 );
    QVERIFY( result );
    QCOMPARE( storageOffset, changeStarts[2]+Size-1 );
    QCOMPARE( storageId, (int)Piece::ChangeStorage );

    result = pieceTable.getStorageData( &storageId, &storageOffset, byteArrayOffset );
    QVERIFY( !result );
}

void PieceTableTest::testRemove()
{
    PieceTable pieceTable;

    int changeStarts[6];
    int storageId;
    int storageOffset;
    bool result;

    // removing at begin
    pieceTable.init( Size );
    pieceTable.remove( KHE::KSection(0, Start-1) );

        QCOMPARE( pieceTable.size(), Size-Start );
        result = pieceTable.getStorageData( &storageId, &storageOffset, 0 );
        QVERIFY( result );
        QCOMPARE( storageOffset, Start );
        QCOMPARE( storageId, (int)Piece::OriginalStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, Size-Start-1 );
        QVERIFY( result );
        QCOMPARE( storageOffset, Size-1 );
        QCOMPARE( storageId, (int)Piece::OriginalStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, Size-Start );
        QVERIFY( !result );

    // removing at middle
    pieceTable.init( Size );
    pieceTable.remove( KHE::KSection(Start, End) );

        QCOMPARE( pieceTable.size(), Size-Width );
        result = pieceTable.getStorageData( &storageId, &storageOffset, Start-1 );
        QVERIFY( result );
        QCOMPARE( storageOffset, Start-1 );
        QCOMPARE( storageId, (int)Piece::OriginalStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, Start );
        QVERIFY( result );
        QCOMPARE( storageOffset, End+1 );
        QCOMPARE( storageId, (int)Piece::OriginalStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, Size-Width-1 );
        QVERIFY( result );
        QCOMPARE( storageOffset, Size-1 );
        QCOMPARE( storageId, (int)Piece::OriginalStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, Size-Width );
        QVERIFY( !result );

    // removing at end
    pieceTable.init( Size );
    pieceTable.remove( KHE::KSection(End+1, Size-1) );

        QCOMPARE( pieceTable.size(), End+1 );
        result = pieceTable.getStorageData( &storageId, &storageOffset, End );
        QVERIFY( result );
        QCOMPARE( storageOffset, End );
        QCOMPARE( storageId, (int)Piece::OriginalStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, End+1 );
        QVERIFY( !result );

    // removing all
    pieceTable.init( Size );
    pieceTable.remove( KHE::KSection::fromWidth(Size) );

        QCOMPARE( pieceTable.size(), 0 );
        result = pieceTable.getStorageData( &storageId, &storageOffset, 0 );
        QVERIFY( !result );


    // removing a lot:
    const int pieceCount = 5;
    const int mid = (pieceCount+1)/2;
    const int midPieceOffset = Size * (mid-1);
    const int fullSize = pieceCount * Size;
    // for this five equally sized pieces are inserted, reverse to offset in ChangeStore
    for( int i=0; i<pieceCount; ++i ) changeStarts[pieceCount-i] = Size*i;

    // removing inside a piece in the middle
    fillWithSize( &pieceTable, pieceCount );
    pieceTable.remove( KHE::KSection::fromWidth(midPieceOffset+Start, Width) );

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
        QCOMPARE( storageOffset, changeStarts[pieceCount]+Size-1 );
        QCOMPARE( storageId, (int)Piece::ChangeStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, fullSize-Width );
        QVERIFY( !result );

    // removing start of a piece in the middle
    fillWithSize( &pieceTable, pieceCount );
    pieceTable.remove( KHE::KSection::fromWidth(midPieceOffset, Start) );

        QCOMPARE( pieceTable.size(), fullSize-Start );

        result = pieceTable.getStorageData( &storageId, &storageOffset, midPieceOffset-1 );
        QVERIFY( result );
        QCOMPARE( storageOffset, changeStarts[mid-1]+Size-1 );
        QCOMPARE( storageId, (int)Piece::ChangeStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, midPieceOffset );
        QVERIFY( result );
        QCOMPARE( storageOffset, changeStarts[mid]+Start );
        QCOMPARE( storageId, (int)Piece::ChangeStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, fullSize-Start-1 );
        QVERIFY( result );
        QCOMPARE( storageOffset, changeStarts[pieceCount]+Size-1 );
        QCOMPARE( storageId, (int)Piece::ChangeStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, fullSize-Start );
        QVERIFY( !result );

    // removing end of a piece in the middle
    fillWithSize( &pieceTable, pieceCount );
    pieceTable.remove( KHE::KSection::fromWidth(midPieceOffset+End+1, Size-(End+1)) );

        QCOMPARE( pieceTable.size(), fullSize-(Size-End-1) );

        result = pieceTable.getStorageData( &storageId, &storageOffset, midPieceOffset+End );
        QVERIFY( result );
        QCOMPARE( storageOffset, changeStarts[mid]+End );
        QCOMPARE( storageId, (int)Piece::ChangeStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, midPieceOffset+End+1 );
        QVERIFY( result );
        QCOMPARE( storageOffset, changeStarts[mid+1] );
        QCOMPARE( storageId, (int)Piece::ChangeStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, fullSize-(Size-End-1)-1 );
        QVERIFY( result );
        QCOMPARE( storageOffset, changeStarts[pieceCount]+Size-1 );
        QCOMPARE( storageId, (int)Piece::ChangeStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, fullSize-(Size-End-1) );
        QVERIFY( !result );

    // removing whole piece in the middle
    fillWithSize( &pieceTable, pieceCount );
    pieceTable.remove( KHE::KSection::fromWidth(midPieceOffset, Size) );

        QCOMPARE( pieceTable.size(), fullSize-Size );

        result = pieceTable.getStorageData( &storageId, &storageOffset, midPieceOffset-1 );
        QVERIFY( result );
        QCOMPARE( storageOffset, changeStarts[mid-1]+Size-1 );
        QCOMPARE( storageId, (int)Piece::ChangeStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, midPieceOffset );
        QVERIFY( result );
        QCOMPARE( storageOffset, changeStarts[mid+1] );
        QCOMPARE( storageId, (int)Piece::ChangeStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, fullSize-Size-1 );
        QVERIFY( result );
        QCOMPARE( storageOffset, changeStarts[pieceCount]+Size-1 );
        QCOMPARE( storageId, (int)Piece::ChangeStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, fullSize-Size );
        QVERIFY( !result );

    // removing whole piece and start of next in the middke
    fillWithSize( &pieceTable, pieceCount );
    pieceTable.remove( KHE::KSection::fromWidth(midPieceOffset, Size+Start) );

        QCOMPARE( pieceTable.size(), fullSize-Size-Start );

        result = pieceTable.getStorageData( &storageId, &storageOffset, midPieceOffset-1 );
        QVERIFY( result );
        QCOMPARE( storageOffset, changeStarts[mid-1]+Size-1 );
        QCOMPARE( storageId, (int)Piece::ChangeStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, midPieceOffset );
        QVERIFY( result );
        QCOMPARE( storageOffset, changeStarts[mid+1]+Start );
        QCOMPARE( storageId, (int)Piece::ChangeStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, fullSize-Size-Start-1 );
        QVERIFY( result );
        QCOMPARE( storageOffset, changeStarts[pieceCount]+Size-1 );
        QCOMPARE( storageId, (int)Piece::ChangeStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, fullSize-Size-Start );
        QVERIFY( !result );

    // removing whole piece and end of previous in the middle
    fillWithSize( &pieceTable, pieceCount );
    pieceTable.remove( KHE::KSection::fromWidth(midPieceOffset-(Size-End-1), Size+Size-(End+1)) );

        QCOMPARE( pieceTable.size(), fullSize-Size-(Size-End-1) );

        result = pieceTable.getStorageData( &storageId, &storageOffset, midPieceOffset-(Size-End-1)-1 );
        QVERIFY( result );
        QCOMPARE( storageOffset, changeStarts[mid-1]+End );
        QCOMPARE( storageId, (int)Piece::ChangeStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, midPieceOffset-(Size-End-1) );
        QVERIFY( result );
        QCOMPARE( storageOffset, changeStarts[mid+1] );
        QCOMPARE( storageId, (int)Piece::ChangeStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, fullSize-Size-(Size-End-1)-1 );
        QVERIFY( result );
        QCOMPARE( storageOffset, changeStarts[pieceCount]+Size-1 );
        QCOMPARE( storageId, (int)Piece::ChangeStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, fullSize-Size-(Size-End-1) );
        QVERIFY( !result );

    // removing end of previous, whole and start of next in the middle
    fillWithSize( &pieceTable, pieceCount );
    pieceTable.remove( KHE::KSection::fromWidth(midPieceOffset-(Size-End-1), Start+Size+Size-(End+1)) );

        QCOMPARE( pieceTable.size(), fullSize-Size-(Size-End-1)-Start );

        result = pieceTable.getStorageData( &storageId, &storageOffset, midPieceOffset-(Size-End-1)-1 );
        QVERIFY( result );
        QCOMPARE( storageOffset, changeStarts[mid-1]+Size-(Size-End-1)-1 );
        QCOMPARE( storageId, (int)Piece::ChangeStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, midPieceOffset-(Size-End-1) );
        QVERIFY( result );
        QCOMPARE( storageOffset, changeStarts[mid+1]+Start );
        QCOMPARE( storageId, (int)Piece::ChangeStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, fullSize-Size-(Size-End-1)-Start-1 );
        QVERIFY( result );
        QCOMPARE( storageOffset, changeStarts[pieceCount]+Size-1 );
        QCOMPARE( storageId, (int)Piece::ChangeStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, fullSize-Size-(Size-End-1)-Start );
        QVERIFY( !result );

    // removing start of piece at start
    fillWithSize( &pieceTable, pieceCount );
    pieceTable.remove( KHE::KSection::fromWidth(Start) );

        QCOMPARE( pieceTable.size(), fullSize-Start );

        result = pieceTable.getStorageData( &storageId, &storageOffset, 0 );
        QVERIFY( result );
        QCOMPARE( storageOffset, changeStarts[1]+Start );
        QCOMPARE( storageId, (int)Piece::ChangeStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, fullSize-Start-1 );
        QVERIFY( result );
        QCOMPARE( storageOffset, changeStarts[pieceCount]+Size-1 );
        QCOMPARE( storageId, (int)Piece::ChangeStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, fullSize-Start );
        QVERIFY( !result );

    // removing whole piece at start
    fillWithSize( &pieceTable, pieceCount );
    pieceTable.remove( KHE::KSection::fromWidth(Size) );

        QCOMPARE( pieceTable.size(), fullSize-Size );

        result = pieceTable.getStorageData( &storageId, &storageOffset, 0 );
        QVERIFY( result );
        QCOMPARE( storageOffset, changeStarts[2] );
        QCOMPARE( storageId, (int)Piece::ChangeStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, fullSize-Size-1 );
        QVERIFY( result );
        QCOMPARE( storageOffset, changeStarts[pieceCount]+Size-1 );
        QCOMPARE( storageId, (int)Piece::ChangeStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, fullSize-Size );
        QVERIFY( !result );

    // removing whole piece and start of next at start
    fillWithSize( &pieceTable, pieceCount );
    pieceTable.remove( KHE::KSection::fromWidth(Size+Start) );

        QCOMPARE( pieceTable.size(), fullSize-Size-Start );

        result = pieceTable.getStorageData( &storageId, &storageOffset, 0 );
        QVERIFY( result );
        QCOMPARE( storageOffset, changeStarts[2]+Start );
        QCOMPARE( storageId, (int)Piece::ChangeStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, fullSize-Size-Start-1 );
        QVERIFY( result );
        QCOMPARE( storageOffset, changeStarts[pieceCount]+Size-1 );
        QCOMPARE( storageId, (int)Piece::ChangeStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, fullSize-Size-Start );
        QVERIFY( !result );

    // removing end of piece at end
    fillWithSize( &pieceTable, pieceCount );
    pieceTable.remove( KHE::KSection::fromWidth(fullSize-Size+End+1, Size-(End+1)) );

        QCOMPARE( pieceTable.size(), fullSize-(Size-End-1) );

        result = pieceTable.getStorageData( &storageId, &storageOffset, fullSize-Size+End );
        QVERIFY( result );
        QCOMPARE( storageOffset, changeStarts[pieceCount]+End );
        QCOMPARE( storageId, (int)Piece::ChangeStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, fullSize-(Size-End-1) );
        QVERIFY( !result );

    // removing whole piece at end
    fillWithSize( &pieceTable, pieceCount );
    pieceTable.remove( KHE::KSection::fromWidth(fullSize-Size, Size) );

        QCOMPARE( pieceTable.size(), fullSize-Size );

        result = pieceTable.getStorageData( &storageId, &storageOffset, fullSize-Size-1 );
        QVERIFY( result );
        QCOMPARE( storageOffset, changeStarts[pieceCount-1]+Size-1 );
        QCOMPARE( storageId, (int)Piece::ChangeStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, fullSize-Size );
        QVERIFY( !result );

    // removing whole piece and end of previous at end
    fillWithSize( &pieceTable, pieceCount );
    pieceTable.remove( KHE::KSection::fromWidth(fullSize-Size-(Size-End-1), Size+Size-(End+1)) );

        QCOMPARE( pieceTable.size(), fullSize-Size-(Size-End-1) );

        result = pieceTable.getStorageData( &storageId, &storageOffset, fullSize-Size-(Size-End-1)-1 );
        QVERIFY( result );
        QCOMPARE( storageOffset, changeStarts[pieceCount-1]+End );
        QCOMPARE( storageId, (int)Piece::ChangeStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, fullSize-Size-(Size-End-1) );
        QVERIFY( !result );

    // removing all 
    fillWithSize( &pieceTable, pieceCount );
    pieceTable.remove( KHE::KSection::fromWidth(fullSize) );

        QCOMPARE( pieceTable.size(), 0 );
        result = pieceTable.getStorageData( &storageId, &storageOffset, 0 );
        QVERIFY( !result );
}

void PieceTableTest::testSwap()
{
    PieceTable pieceTable;

    int storageId;
    int storageOffset;
    bool result;

    // moving end at begin
    pieceTable.init( Size );
    pieceTable.swap( 0, KHE::KSection::fromWidth(End+1, Size-(End+1)) );

        QCOMPARE( pieceTable.size(), Size );

        result = pieceTable.getStorageData( &storageId, &storageOffset, 0 );
        QVERIFY( result );
        QCOMPARE( storageOffset, End+1 );
        QCOMPARE( storageId, (int)Piece::OriginalStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, Size-End-2 );
        QVERIFY( result );
        QCOMPARE( storageOffset, Size-1 );
        QCOMPARE( storageId, (int)Piece::OriginalStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, Size-End-1 );
        QVERIFY( result );
        QCOMPARE( storageOffset, 0 );
        QCOMPARE( storageId, (int)Piece::OriginalStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, Size-1 );
        QVERIFY( result );
        QCOMPARE( storageOffset, End );
        QCOMPARE( storageId, (int)Piece::OriginalStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, Size );
        QVERIFY( !result );

    // moving end at mid
    pieceTable.init( Size );
    pieceTable.swap( Start, KHE::KSection::fromWidth(End+1, Size-(End+1)) );

        QCOMPARE( pieceTable.size(), Size );

        result = pieceTable.getStorageData( &storageId, &storageOffset, Start-1 );
        QVERIFY( result );
        QCOMPARE( storageOffset, Start-1 );
        QCOMPARE( storageId, (int)Piece::OriginalStorage );

        QCOMPARE( pieceTable.size(), Size );
        result = pieceTable.getStorageData( &storageId, &storageOffset, Start );
        QVERIFY( result );
        QCOMPARE( storageOffset, End+1 );
        QCOMPARE( storageId, (int)Piece::OriginalStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, Start+Size-End-2 );
        QVERIFY( result );
        QCOMPARE( storageOffset, Size-1 );
        QCOMPARE( storageId, (int)Piece::OriginalStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, Start+Size-End-1 );
        QVERIFY( result );
        QCOMPARE( storageOffset, Start );
        QCOMPARE( storageId, (int)Piece::OriginalStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, Size-1 );
        QVERIFY( result );
        QCOMPARE( storageOffset, End );
        QCOMPARE( storageId, (int)Piece::OriginalStorage );

        result = pieceTable.getStorageData( &storageId, &storageOffset, Size );
        QVERIFY( !result );

    // moving mid at begin
    pieceTable.init( Size );
    pieceTable.swap( 0, KHE::KSection::fromWidth(Start, Width) );

        QCOMPARE( pieceTable.size(), Size );

        QCOMPARE( pieceTable.size(), Size );
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
    pieceTable.init( Size );
    int mid = (End+Start)/2;
    pieceTable.swap( Start, KHE::KSection::fromWidth(mid, End-mid+1) );

        QCOMPARE( pieceTable.size(), Size );

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
    const int fullSize = pieceCount * Size;
    const int midPiece = (pieceCount+1)/2;
    const int midPieceOffset = Size * (midPiece-1);
    int changeStarts[6];
    for( int i=0; i<pieceCount; ++i ) changeStarts[pieceCount-i] = Size*i;

    // moving start of piece at start
    fillWithSize( &pieceTable, pieceCount );
    pieceTable.swap( Start, KHE::KSection::fromWidth(mid, End-mid+1) );

        QCOMPARE( pieceTable.size(), fullSize );
    //TODO: add other tests
}

}

QTEST_MAIN( KPieceTable::PieceTableTest )
