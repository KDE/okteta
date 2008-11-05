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

#include "grouppiecetablechangeabstractpiecetablechangeiftest.h"

// test object
#include <piecetable/grouppiecetablechange.h>
#include <piecetable/replacepiecetablechange.h>
#include <piecetable/insertpiecetablechange.h>
#include <piecetable/removepiecetablechange.h>
// lib
#include <piecetable/piecetable.h>
// Qt
#include <QtTest/QtTest>


static const int ReplacedOldStorageOffset = 80;
static const int ReplacedStart = 32;
static const int ReplacedEnd = 78;
static const int ReplacedWidth = ReplacedEnd-ReplacedStart+1;
static const int ReplaceLength = 30;
static const int ReplaceInsertStorageOffset = 67;

static const int InsertOffset = 50;
static const int InsertLength = 80;
static const int InsertStorageOffset = 67;

static const int RemovedStart = 40;
static const int RemovedEnd = 78;
static const int RemovedWidth = RemovedEnd-RemovedStart+1;
static const int RemovedOldStorageOffset = 23;


namespace KPieceTable
{

AbstractPieceTableChange *GroupPieceTableChangeAbstractPieceTableChangeIfTest::createPieceTableChange()
{
    const Piece replacedPiece( KHE::Section::fromWidth(ReplacedOldStorageOffset,ReplacedWidth), Piece::ChangeStorage );
    const Piece removedPiece( KHE::Section::fromWidth(RemovedOldStorageOffset,RemovedWidth), Piece::ChangeStorage );

    ReplacePieceTableChange *replaceChange =
        new ReplacePieceTableChange( KHE::Section(ReplacedStart,ReplacedEnd),
                                     ReplaceLength, ReplaceInsertStorageOffset,
                                     PieceList(replacedPiece) );
    InsertPieceTableChange *insertChange =
        new InsertPieceTableChange( InsertOffset, InsertLength, InsertStorageOffset );
    RemovePieceTableChange *removeChange =
        new RemovePieceTableChange( KHE::Section(RemovedStart,RemovedEnd), PieceList(removedPiece) );

    GroupPieceTableChange *pieceTableChange =
        new GroupPieceTableChange( 0, QString() );

    pieceTableChange->appendChange( replaceChange );
    pieceTableChange->appendChange( insertChange );
    pieceTableChange->appendChange( removeChange );

    return pieceTableChange;
}
void GroupPieceTableChangeAbstractPieceTableChangeIfTest::changePieceTable( PieceTable *pieceTable )
{
    pieceTable->replace( KHE::Section(ReplacedStart,ReplacedEnd), ReplaceLength, ReplaceInsertStorageOffset );
    pieceTable->insert( InsertOffset, InsertLength, InsertStorageOffset );
    pieceTable->remove( KHE::Section(RemovedStart,RemovedEnd) );
}

void GroupPieceTableChangeAbstractPieceTableChangeIfTest::deletePieceTableChange(
       AbstractPieceTableChange *pieceTableChange )
{
    delete pieceTableChange;
}

}

QTEST_MAIN( KPieceTable::GroupPieceTableChangeAbstractPieceTableChangeIfTest )
