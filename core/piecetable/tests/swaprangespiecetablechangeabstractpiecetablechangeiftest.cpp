/***************************************************************************
                          swaprangespiecetablechangeabstractpiecetablechangeiftest.cpp  -  description
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


#include "swaprangespiecetablechangeabstractpiecetablechangeiftest.h"

// test object
#include <piecetable/swaprangespiecetablechange.h>
// lib
#include <piecetable/piecetable.h>
// Qt
#include <QtTest/QtTest>


static const int FirstRangeStart = 30;
static const int SecondRangeStart = 50;
static const int SecondRangeEnd = 66;

namespace KPieceTable
{

AbstractPieceTableChange *SwapRangesPieceTableChangeAbstractPieceTableChangeIfTest::createPieceTableChange()
{
    SwapRangesPieceTableChange *pieceTableChange =
        new SwapRangesPieceTableChange( FirstRangeStart, KHE::KSection(SecondRangeStart,SecondRangeEnd) );

    return pieceTableChange;
}
void SwapRangesPieceTableChangeAbstractPieceTableChangeIfTest::changePieceTable( PieceTable *pieceTable )
{
    pieceTable->swap( FirstRangeStart, KHE::KSection(SecondRangeStart,SecondRangeEnd) );
}

void SwapRangesPieceTableChangeAbstractPieceTableChangeIfTest::deletePieceTableChange( AbstractPieceTableChange *pieceTableChange )
{
    delete pieceTableChange;
}

}

QTEST_MAIN( KPieceTable::SwapRangesPieceTableChangeAbstractPieceTableChangeIfTest )
