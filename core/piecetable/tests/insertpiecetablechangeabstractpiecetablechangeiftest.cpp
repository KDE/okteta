/***************************************************************************
                          insertpiecetablechangeabstractpiecetablechangeiftest.cpp  -  description
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


#include "insertpiecetablechangeabstractpiecetablechangeiftest.h"

// test object
#include <piecetable/insertpiecetablechange.h>
// lib
#include <piecetable/piecetable.h>
// Qt
#include <QtTest/QtTest>


static const int InsertOffset = 30;
static const int InsertLength = 80;
static const int StorageOffset = 67;

namespace KPieceTable
{

AbstractPieceTableChange *InsertPieceTableChangeAbstractPieceTableChangeIfTest::createPieceTableChange()
{
    InsertPieceTableChange *pieceTableChange =
        new InsertPieceTableChange( InsertOffset, InsertLength, StorageOffset );

    return pieceTableChange;
}
void InsertPieceTableChangeAbstractPieceTableChangeIfTest::changePieceTable( PieceTable *pieceTable )
{
    pieceTable->insert( InsertOffset, InsertLength, StorageOffset );
}

void InsertPieceTableChangeAbstractPieceTableChangeIfTest::deletePieceTableChange( AbstractPieceTableChange *pieceTableChange )
{
    delete pieceTableChange;
}

}

QTEST_MAIN( KPieceTable::InsertPieceTableChangeAbstractPieceTableChangeIfTest )

