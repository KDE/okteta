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
