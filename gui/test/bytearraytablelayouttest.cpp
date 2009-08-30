/*
    This file is part of the Okteta Gui library, part of the KDE project.

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

#include "bytearraytablelayouttest.h"

// test object
#include <bytearraytablelayout.h>
// Qt
#include <QtTest/QtTest>

namespace Okteta
{

// local variables
static const int NoOfBytesPerLine = 8;
static const int StartOffset = 22;
static const int FirstLineOffset = 10;
static const int RelativeStartOffset = StartOffset-FirstLineOffset;
static const int ByteArrayOffset = 9;
static const int Length = 250;
static const int FinalOffset = (StartOffset+Length-1);
static const int RelativeFinalOffset = FinalOffset-FirstLineOffset;

static const int StartLine = RelativeStartOffset / NoOfBytesPerLine;
static const int StartLinePosition = RelativeStartOffset % NoOfBytesPerLine;
static Coord StartCoord( StartLinePosition, StartLine );

static const int FinalLine =  RelativeFinalOffset/ NoOfBytesPerLine;
static const int FinalLinePosition = RelativeFinalOffset % NoOfBytesPerLine;
static Coord FinalCoord( FinalLinePosition, FinalLine );

static const int NoOfLinesPerPage = 5;

static const int Pos1 = 15;
static const int Pos2 = 25;
static const int Line1 = 10;
static const int LineCount = 10;
static const int Line2 = Line1 + LineCount - 1;
static Coord Start( Pos1, Line1 );
static Coord End(   Pos2, Line2 );


void ByteArrayTableLayoutTest::testConstructor()
{
    ByteArrayTableLayout layout( NoOfBytesPerLine, FirstLineOffset, StartOffset, ByteArrayOffset, Length );

    QCOMPARE( layout.noOfBytesPerLine(), NoOfBytesPerLine );
    QCOMPARE( layout.startOffset(), StartOffset );
    QCOMPARE( layout.byteArrayOffset(), ByteArrayOffset );
    QCOMPARE( layout.length(), Length );
    QCOMPARE( layout.startLine(), StartLine );
    QCOMPARE( layout.startLinePosition(), StartLinePosition );
    QCOMPARE( layout.startCoord(), StartCoord );
    QCOMPARE( layout.finalLine(), FinalLine );
    QCOMPARE( layout.finalLinePosition(), FinalLinePosition );
    QCOMPARE( layout.finalCoord(), FinalCoord );
    QCOMPARE( layout.noOfLines(), FinalLine+1 );
    QCOMPARE( layout.noOfLinesPerPage(), 1 );
}

void ByteArrayTableLayoutTest::testSetStartOffset()
{
    ByteArrayTableLayout layout( NoOfBytesPerLine, FirstLineOffset, 0, ByteArrayOffset, Length );

    layout.setStartOffset( StartOffset );
    QCOMPARE( layout.noOfBytesPerLine(), NoOfBytesPerLine );
    QCOMPARE( layout.startOffset(), StartOffset );
    QCOMPARE( layout.byteArrayOffset(), ByteArrayOffset );
    QCOMPARE( layout.length(), Length );
    QCOMPARE( layout.startLine(), StartLine );
    QCOMPARE( layout.startLinePosition(), StartLinePosition );
    QCOMPARE( layout.startCoord(), StartCoord );
    QCOMPARE( layout.finalLine(), FinalLine );
    QCOMPARE( layout.finalLinePosition(), FinalLinePosition );
    QCOMPARE( layout.finalCoord(), FinalCoord );
    QCOMPARE( layout.noOfLines(), FinalLine+1 );
    QCOMPARE( layout.noOfLinesPerPage(), 1 );
}

void ByteArrayTableLayoutTest::testSetFirstLineOffset()
{
    ByteArrayTableLayout layout( NoOfBytesPerLine, 0, StartOffset, ByteArrayOffset, Length );

    layout.setFirstLineOffset( FirstLineOffset );
    QCOMPARE( layout.noOfBytesPerLine(), NoOfBytesPerLine );
    QCOMPARE( layout.startOffset(), StartOffset );
    QCOMPARE( layout.byteArrayOffset(), ByteArrayOffset );
    QCOMPARE( layout.length(), Length );
    QCOMPARE( layout.startLine(), StartLine );
    QCOMPARE( layout.startLinePosition(), StartLinePosition );
    QCOMPARE( layout.startCoord(), StartCoord );
    QCOMPARE( layout.finalLine(), FinalLine );
    QCOMPARE( layout.finalLinePosition(), FinalLinePosition );
    QCOMPARE( layout.finalCoord(), FinalCoord );
    QCOMPARE( layout.noOfLines(), FinalLine+1 );
    QCOMPARE( layout.noOfLinesPerPage(), 1 );
}

void ByteArrayTableLayoutTest::testSetNoOfBytesPerLine()
{
    ByteArrayTableLayout layout( 1, FirstLineOffset, StartOffset, ByteArrayOffset, Length );

    layout.setNoOfBytesPerLine( NoOfBytesPerLine );
    QCOMPARE( layout.noOfBytesPerLine(), NoOfBytesPerLine );
    QCOMPARE( layout.startOffset(), StartOffset );
    QCOMPARE( layout.byteArrayOffset(), ByteArrayOffset );
    QCOMPARE( layout.length(), Length );
    QCOMPARE( layout.startLine(), StartLine );
    QCOMPARE( layout.startLinePosition(), StartLinePosition );
    QCOMPARE( layout.startCoord(), StartCoord );
    QCOMPARE( layout.finalLine(), FinalLine );
    QCOMPARE( layout.finalLinePosition(), FinalLinePosition );
    QCOMPARE( layout.finalCoord(), FinalCoord );
    QCOMPARE( layout.noOfLines(), FinalLine+1 );
    QCOMPARE( layout.noOfLinesPerPage(), 1 );
}

void ByteArrayTableLayoutTest::testSetLength()
{
    ByteArrayTableLayout layout( NoOfBytesPerLine, FirstLineOffset, StartOffset, ByteArrayOffset, 1 );

    layout.setLength( Length );
    QCOMPARE( layout.noOfBytesPerLine(), NoOfBytesPerLine );
    QCOMPARE( layout.startOffset(), StartOffset );
    QCOMPARE( layout.byteArrayOffset(), ByteArrayOffset );
    QCOMPARE( layout.length(), Length );
    QCOMPARE( layout.startLine(), StartLine );
    QCOMPARE( layout.startLinePosition(), StartLinePosition );
    QCOMPARE( layout.startCoord(), StartCoord );
    QCOMPARE( layout.finalLine(), FinalLine );
    QCOMPARE( layout.finalLinePosition(), FinalLinePosition );
    QCOMPARE( layout.finalCoord(), FinalCoord );
    QCOMPARE( layout.noOfLines(), FinalLine+1 );
    QCOMPARE( layout.noOfLinesPerPage(), 1 );
}

void ByteArrayTableLayoutTest::testSetByteArrayOffset()
{
    ByteArrayTableLayout layout( NoOfBytesPerLine, FirstLineOffset, StartOffset, 0, Length );

    layout.setByteArrayOffset( ByteArrayOffset );
    QCOMPARE( layout.noOfBytesPerLine(), NoOfBytesPerLine );
    QCOMPARE( layout.startOffset(), StartOffset );
    QCOMPARE( layout.byteArrayOffset(), ByteArrayOffset );
    QCOMPARE( layout.length(), Length );
    QCOMPARE( layout.startLine(), StartLine );
    QCOMPARE( layout.startLinePosition(), StartLinePosition );
    QCOMPARE( layout.startCoord(), StartCoord );
    QCOMPARE( layout.finalLine(), FinalLine );
    QCOMPARE( layout.finalLinePosition(), FinalLinePosition );
    QCOMPARE( layout.finalCoord(), FinalCoord );
    QCOMPARE( layout.noOfLines(), FinalLine+1 );
    QCOMPARE( layout.noOfLinesPerPage(), 1 );
}

void ByteArrayTableLayoutTest::testSetNoOfLinesPerPage()
{
    ByteArrayTableLayout layout( NoOfBytesPerLine, FirstLineOffset, StartOffset, ByteArrayOffset, Length );

    layout.setNoOfLinesPerPage( NoOfLinesPerPage );
    QCOMPARE( layout.noOfBytesPerLine(), NoOfBytesPerLine );
    QCOMPARE( layout.startOffset(), StartOffset );
    QCOMPARE( layout.byteArrayOffset(), ByteArrayOffset );
    QCOMPARE( layout.length(), Length );
    QCOMPARE( layout.startLine(), StartLine );
    QCOMPARE( layout.startLinePosition(), StartLinePosition );
    QCOMPARE( layout.startCoord(), StartCoord );
    QCOMPARE( layout.finalLine(), FinalLine );
    QCOMPARE( layout.finalLinePosition(), FinalLinePosition );
    QCOMPARE( layout.finalCoord(), FinalCoord );
    QCOMPARE( layout.noOfLines(), FinalLine+1 );
    QCOMPARE( layout.noOfLinesPerPage(), NoOfLinesPerPage );
}

}

QTEST_MAIN( Okteta::ByteArrayTableLayoutTest )
