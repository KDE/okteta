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
    const Piece removedPiece( KHE::Section::fromWidth(Start+ChangeStorageOffset,Width), Piece::ChangeStorage );
    RemovePieceTableChange *pieceTableChange =
        new RemovePieceTableChange( KHE::Section(Start,End), PieceList(removedPiece) );

    return pieceTableChange;
}
void RemovePieceTableChangeAbstractPieceTableChangeIfTest::changePieceTable ( PieceTable *pieceTable )
{
    pieceTable->remove( KHE::Section(Start,End) );
}

void RemovePieceTableChangeAbstractPieceTableChangeIfTest::deletePieceTableChange(
       AbstractPieceTableChange *pieceTableChange )
{
    delete pieceTableChange;
}

}

QTEST_MAIN( KPieceTable::RemovePieceTableChangeAbstractPieceTableChangeIfTest )
