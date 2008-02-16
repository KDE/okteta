/***************************************************************************
                          testpiecetablechangeabstractpiecetablechangeiftest.cpp  -  description
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


#include "testpiecetablechangeabstractpiecetablechangeiftest.h"

// test object
#include "testpiecetablechange.h"
// lib
#include <piecetable/piecetable.h>
// Qt
#include <QtTest/QtTest>


static const int TestOffset = 30;
static const int StorageOffset = 20;
static const int ReplacedStorageOffset = 53;

namespace KPieceTable
{

KPieceTable::AbstractPieceTableChange *TestPieceTableChangeAbstractPieceTableChangeIfTest::createPieceTableChange()
{
    TestPieceTableChange *pieceTableChange =
        new TestPieceTableChange( 0, QString(),
                                           TestOffset, StorageOffset, Piece::ChangeStorage,
                                           ReplacedStorageOffset );

    return pieceTableChange;
}
void TestPieceTableChangeAbstractPieceTableChangeIfTest::changePieceTable( PieceTable *pieceTable )
{
    pieceTable->replaceOne( TestOffset, StorageOffset );
}

void TestPieceTableChangeAbstractPieceTableChangeIfTest::deletePieceTableChange(
       AbstractPieceTableChange *pieceTableChange )
{
    delete pieceTableChange;
}

}

QTEST_MAIN( KPieceTable::TestPieceTableChangeAbstractPieceTableChangeIfTest )
