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

#include "abstractpiecetablechangeiftest.h"

// test object
#include <piecetable/abstractpiecetablechange.h>
// lib
#include "testpiecetablechange.h"
#include <piecetable/piecetable.h>
// Qt
#include <QtTest/QtTest>


namespace KPieceTable
{

static const Size BaseSize = 100;
static const Address ChangeStorageOffset = 23;

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
    pieceTable.insert( 0, BaseSize, ChangeStorageOffset );

    // do change which creates this
    changePieceTable ( &pieceTable );
    mPieceTableChange->revert( &pieceTable );

    QCOMPARE( pieceTable.size(), BaseSize );
    for( int i=0; i<BaseSize; ++i )
    {
        int storageId;
        Address storageOffset;
        bool result = pieceTable.getStorageData( &storageId, &storageOffset, i );

        QVERIFY( result );
        QCOMPARE( storageOffset, i+ChangeStorageOffset );
        QCOMPARE( storageId, (int)Piece::ChangeStorage );
    }

    // now replay changes
    mPieceTableChange->apply( &pieceTable );
    mPieceTableChange->revert( &pieceTable );

    QCOMPARE( pieceTable.size(), BaseSize );
    for( Address i=0; i<BaseSize; ++i )
    {
        int storageId;
        Address storageOffset;
        bool result = pieceTable.getStorageData( &storageId, &storageOffset, i );

        QVERIFY( result );
        QCOMPARE( storageOffset, i+ChangeStorageOffset );
        QCOMPARE( storageId, (int)Piece::ChangeStorage );
    }
}

}
