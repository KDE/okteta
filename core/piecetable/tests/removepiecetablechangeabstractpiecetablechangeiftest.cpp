/***************************************************************************
                          removepiecetablechangeabstractpiecetablechangeiftest.cpp  -  description
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


#include "removepiecetablechangeabstractpiecetablechangeiftest.h"

// test object
#include <piecetable/removepiecetablechange.h>
// lib
#include <piecetable/piecetable.h>
// Qt
#include <QtTest/QtTest>


static const int Start = 32;
static const int End = 78;
static const int Width = End-Start+1;

static const int ChangeStorageOffset = 23;

namespace KPieceTable
{

AbstractPieceTableChange *RemovePieceTableChangeAbstractPieceTableChangeIfTest::createPieceTableChange()
{
    const Piece removedPiece( KHE::KSection::fromWidth(Start+ChangeStorageOffset,Width), Piece::ChangeStorage );
    RemovePieceTableChange *pieceTableChange =
        new RemovePieceTableChange( KHE::KSection(Start,End), PieceList(removedPiece) );

    return pieceTableChange;
}
void RemovePieceTableChangeAbstractPieceTableChangeIfTest::changePieceTable ( PieceTable *pieceTable )
{
    pieceTable->remove( KHE::KSection(Start,End) );
}

void RemovePieceTableChangeAbstractPieceTableChangeIfTest::deletePieceTableChange(
       AbstractPieceTableChange *pieceTableChange )
{
    delete pieceTableChange;
}

}

QTEST_MAIN( KPieceTable::RemovePieceTableChangeAbstractPieceTableChangeIfTest )
