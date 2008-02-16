/***************************************************************************
                          replacepiecetablechangeabstractpiecetablechangeiftest.cpp  -  description
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


#include "replacepiecetablechangeabstractpiecetablechangeiftest.h"

// test object
#include <piecetable/replacepiecetablechange.h>
// lib
#include <piecetable/piecetable.h>
// Qt
#include <QtTest/QtTest>


static const int Start = 32;
static const int End = 78;
static const int Width = End-Start+1;
static const int InsertLength = 80;
static const int InsertStorageOffset = 67;

static const int ChangeStorageOffset = 23;

namespace KPieceTable
{

AbstractPieceTableChange *ReplacePieceTableChangeAbstractPieceTableChangeIfTest::createPieceTableChange()
{
    const Piece replacedPiece( KHE::KSection::fromWidth(Start+ChangeStorageOffset,Width), Piece::ChangeStorage );

    ReplacePieceTableChange *pieceTableChange =
        new ReplacePieceTableChange( KHE::KSection(Start,End), InsertLength, InsertStorageOffset,
                                              PieceList(replacedPiece) );

    return pieceTableChange;
}
void ReplacePieceTableChangeAbstractPieceTableChangeIfTest::changePieceTable( PieceTable *pieceTable )
{
    pieceTable->replace( KHE::KSection(Start,End), InsertLength, InsertStorageOffset );
}

void ReplacePieceTableChangeAbstractPieceTableChangeIfTest::deletePieceTableChange(
       AbstractPieceTableChange *pieceTableChange )
{
    delete pieceTableChange;
}

}

QTEST_MAIN( KPieceTable::ReplacePieceTableChangeAbstractPieceTableChangeIfTest )
