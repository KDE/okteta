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

#include "replacepiecetablechangeabstractpiecetablechangeiftest.h"

// test object
#include <piecetable/replacepiecetablechange.h>
// lib
#include <piecetable/piecetable.h>
// Qt
#include <QtTest/QtTest>


namespace KPieceTable
{

static const Address Start = 32;
static const Address End = 78;
static const Size Width = End-Start+1;
static const Size InsertLength = 80;
static const Address InsertStorageOffset = 67;

static const Address ChangeStorageOffset = 23;


AbstractPieceTableChange* ReplacePieceTableChangeAbstractPieceTableChangeIfTest::createPieceTableChange()
{
    const Piece replacedPiece( AddressRange::fromWidth(Start+ChangeStorageOffset,Width), Piece::ChangeStorage );

    ReplacePieceTableChange* pieceTableChange =
        new ReplacePieceTableChange( AddressRange(Start,End), InsertLength, InsertStorageOffset,
                                              PieceList(replacedPiece) );

    return pieceTableChange;
}
void ReplacePieceTableChangeAbstractPieceTableChangeIfTest::changePieceTable( PieceTable *pieceTable )
{
    pieceTable->replace( AddressRange(Start,End), InsertLength, InsertStorageOffset );
}

void ReplacePieceTableChangeAbstractPieceTableChangeIfTest::deletePieceTableChange(
       AbstractPieceTableChange *pieceTableChange )
{
    delete pieceTableChange;
}

}

QTEST_MAIN( KPieceTable::ReplacePieceTableChangeAbstractPieceTableChangeIfTest )
