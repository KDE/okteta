/***************************************************************************
                          abstractpiecetablechangeiftest.cpp  -  description
                            -------------------
    begin                : Tue Feb 12 2008
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


#include "abstractpiecetablechangeiftest.h"

// test object
#include <piecetable/abstractpiecetablechange.h>
// lib
#include "testpiecetablechange.h"
#include <piecetable/piecetable.h>
// Qt
#include <QtTest/QtTest>

using namespace KHE;

namespace KPieceTable
{

static const int Size = 100;
static const int ChangeStorageOffset = 23;

// ---------------------------------------------------------------- Tests -----


void AbstractPieceTableChangeIfTest::init()
{
    mPieceTableChange = createPieceTableChange();
}

void AbstractPieceTableChangeIfTest::cleanup()
{
    deletePieceTableChange( mPieceTableChange );
}

// ---------------------------------------------------------------- Tests -----

void AbstractPieceTableChangeIfTest::testMerge()
{
    TestPieceTableChange testChange;

    int typeId = mPieceTableChange->type();
    QString description = mPieceTableChange->description();
    bool result = mPieceTableChange->merge( &testChange );

    QVERIFY( !result );
    QCOMPARE( mPieceTableChange->type(), typeId );
    QCOMPARE( mPieceTableChange->description(), description );
}

void AbstractPieceTableChangeIfTest::testRevertApply()
{
    PieceTable pieceTable;
    pieceTable.init( 0 );
    pieceTable.insert( 0, Size, ChangeStorageOffset );

    // do change which creates this
    changePieceTable ( &pieceTable );
    mPieceTableChange->revert( &pieceTable );

    QCOMPARE( pieceTable.size(), Size );
    for( int i=0; i<Size; ++i )
    {
        int storageId;
        int storageOffset;
        bool result = pieceTable.getStorageData( &storageId, &storageOffset, i );

        QVERIFY( result );
        QCOMPARE( storageOffset, i+ChangeStorageOffset );
        QCOMPARE( storageId, (int)Piece::ChangeStorage );
    }

    // now replay changes
    mPieceTableChange->apply( &pieceTable );
    mPieceTableChange->revert( &pieceTable );

    QCOMPARE( pieceTable.size(), Size );
    for( int i=0; i<Size; ++i )
    {
        int storageId;
        int storageOffset;
        bool result = pieceTable.getStorageData( &storageId, &storageOffset, i );

        QVERIFY( result );
        QCOMPARE( storageOffset, i+ChangeStorageOffset );
        QCOMPARE( storageId, (int)Piece::ChangeStorage );
    }
}

}
