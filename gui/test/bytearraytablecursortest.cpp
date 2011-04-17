/*
    This file is part of the Okteta Gui library, made within the KDE community.

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

#include "bytearraytablecursortest.h"

// test object
#include <bytearraytablecursor.h>
// lib
#include <bytearraytablelayout.h>
// Qt
#include <QtTest/QtTest>


namespace Okteta
{

// local variables
static const Size NoOfBytesPerLine = 8;
static const Address StartOffset = 22;
static const Address FirstLineOffset = 10;
static const Address RelativeStartOffset = StartOffset-FirstLineOffset;
static const Address ByteArrayOffset = 9;
static const Size Length = 250;
static const Address FinalOffset = (StartOffset+Length-1);
static const Address RelativeFinalOffset = FinalOffset-FirstLineOffset;

static const Address FirstIndex = ByteArrayOffset;
static const Address LastIndex = Length-1+ByteArrayOffset;

static const Line StartLine = RelativeStartOffset / NoOfBytesPerLine;
static const LinePosition StartLinePosition = RelativeStartOffset % NoOfBytesPerLine;
static Coord StartCoord( StartLinePosition, StartLine );

static const Line FinalLine =  RelativeFinalOffset/ NoOfBytesPerLine;
static const LinePosition FinalLinePosition = RelativeFinalOffset % NoOfBytesPerLine;
static Coord FinalCoord( FinalLinePosition, FinalLine );

static const LineSize NoOfLinesPerPage = 5;

static const LinePosition Pos1 = 15;
static const LinePosition Pos2 = 25;
static const Line Line1 = 10;
static const LineSize LineCount = 10;
static const Line Line2 = Line1 + LineCount - 1;
static Coord Start( Pos1, Line1 );
static Coord End(   Pos2, Line2 );


void ByteArrayTableCursorTest::testConstructor()
{
    const ByteArrayTableLayout layout( NoOfBytesPerLine, FirstLineOffset, StartOffset, ByteArrayOffset, Length );

    ByteArrayTableCursor cursor( &layout );

    QCOMPARE( cursor.index(), FirstIndex );
    QCOMPARE( cursor.validIndex(), FirstIndex );
    QCOMPARE( cursor.realIndex(), FirstIndex );
    QCOMPARE( cursor.coord(), StartCoord );
    QCOMPARE( cursor.pos(), StartCoord.pos() );
    QCOMPARE( cursor.line(), StartCoord.line() );
    QCOMPARE( cursor.isBehind(), false );
    QCOMPARE( cursor.appendPosEnabled(), false );
    QCOMPARE( cursor.atStart(), true );
    QCOMPARE( cursor.atEnd(), false );
    QCOMPARE( cursor.atAppendPos(), false );
    QCOMPARE( cursor.atLineStart(), true );
    QCOMPARE( cursor.atLineEnd(), false );
}

void ByteArrayTableCursorTest::testSetAppendPosEnabled()
{
    const ByteArrayTableLayout layout( NoOfBytesPerLine, FirstLineOffset, StartOffset, ByteArrayOffset, Length );
    const Coord finalCoord = layout.finalCoord();

    ByteArrayTableCursor cursor( &layout );

    // with append pos
    cursor.setAppendPosEnabled( true );
    cursor.gotoEnd();

    Coord nextToFinalCoord( finalCoord );
    nextToFinalCoord.goCRight( layout.noOfBytesPerLine()-1 );
    QCOMPARE( cursor.appendPosEnabled(), true );
    QCOMPARE( cursor.atAppendPos(), true );
    QCOMPARE( cursor.coord(), nextToFinalCoord );

    // without append pos
    cursor.setAppendPosEnabled( false );
    cursor.gotoEnd();
    QCOMPARE( cursor.appendPosEnabled(), false );
    QCOMPARE( cursor.atAppendPos(), false );
    QCOMPARE( cursor.coord(), finalCoord );
}

void ByteArrayTableCursorTest::testGotoIndex()
{
    const ByteArrayTableLayout layout( NoOfBytesPerLine, FirstLineOffset, StartOffset, ByteArrayOffset, Length );

    ByteArrayTableCursor cursor( &layout );

    // 2 before first valid, as (in)validIndex equals -1, see next test
    Address index = FirstIndex-2;
    Coord coord = layout.coordOfIndex( index );

    cursor.gotoIndex( index );

    QCOMPARE( cursor.index(), index );
    QCOMPARE( cursor.validIndex(), -1 );
    QCOMPARE( cursor.realIndex(), index );
    QCOMPARE( cursor.isBehind(), false );
    QCOMPARE( cursor.atStart(), false );
    QCOMPARE( cursor.atEnd(), false );
    QCOMPARE( cursor.atAppendPos(), false );
    QCOMPARE( cursor.atLineStart(), false );
    QCOMPARE( cursor.atLineEnd(), false );
    QCOMPARE( cursor.coord(), coord );

    // 1 before first valid
    index = FirstIndex-1; // hm
    coord = layout.coordOfIndex( index );

    cursor.gotoIndex( index );

    QCOMPARE( cursor.index(), index );
    QCOMPARE( cursor.validIndex(), -1 );
    QCOMPARE( cursor.realIndex(), index );
    QCOMPARE( cursor.isBehind(), false );
    QCOMPARE( cursor.atStart(), false );
    QCOMPARE( cursor.atEnd(), false );
    QCOMPARE( cursor.atAppendPos(), false );
    QCOMPARE( cursor.atLineStart(), false );
    QCOMPARE( cursor.atLineEnd(), false );
    QCOMPARE( cursor.coord(), coord );

    // at first valid
    index = FirstIndex;
    coord = layout.coordOfIndex( index );

    cursor.gotoIndex( index );

    QCOMPARE( cursor.index(), index );
    QCOMPARE( cursor.validIndex(), index );
    QCOMPARE( cursor.realIndex(), index );
    QCOMPARE( cursor.isBehind(), false );
    QCOMPARE( cursor.atStart(), true );
    QCOMPARE( cursor.atEnd(), false );
    QCOMPARE( cursor.atAppendPos(), false );
    QCOMPARE( cursor.atLineStart(), true );
    QCOMPARE( cursor.atLineEnd(), false );
    QCOMPARE( cursor.coord(), coord );

    // at last valid
    index = LastIndex;
    coord = layout.coordOfIndex( index );

    cursor.gotoIndex( index );

    QCOMPARE( cursor.index(), index );
    QCOMPARE( cursor.validIndex(), index );
    QCOMPARE( cursor.realIndex(), index );
    QCOMPARE( cursor.isBehind(), false );
    QCOMPARE( cursor.atStart(), false );
    QCOMPARE( cursor.atEnd(), false );
    QCOMPARE( cursor.atAppendPos(), false );
    QCOMPARE( cursor.atLineStart(), false );
    QCOMPARE( cursor.atLineEnd(), true );
    QCOMPARE( cursor.coord(), coord );

    // 1 after last valid
    index = LastIndex+1;
    coord = layout.coordOfIndex( index );

    cursor.gotoIndex( index );

    QCOMPARE( cursor.index(), index );
    QCOMPARE( cursor.validIndex(), -1 );
    QCOMPARE( cursor.realIndex(), index );
    QCOMPARE( cursor.isBehind(), false );
    QCOMPARE( cursor.atStart(), false );
    QCOMPARE( cursor.atEnd(), true );
    QCOMPARE( cursor.atAppendPos(), true );
    QCOMPARE( cursor.atLineStart(), false );
    QCOMPARE( cursor.atLineEnd(), false );
    QCOMPARE( cursor.coord(), coord );

    // 2 after last valid
    index = LastIndex + 2;
    coord = layout.coordOfIndex( index );

    cursor.gotoIndex( index );

    QCOMPARE( cursor.index(), index );
    QCOMPARE( cursor.validIndex(), -1 );
    QCOMPARE( cursor.realIndex(), index );
    QCOMPARE( cursor.isBehind(), false );
    QCOMPARE( cursor.atStart(), false );
    QCOMPARE( cursor.atEnd(), false );
    QCOMPARE( cursor.atAppendPos(), false );
    QCOMPARE( cursor.atLineStart(), false );
    QCOMPARE( cursor.atLineEnd(), false );
    QCOMPARE( cursor.coord(), coord );
}

void ByteArrayTableCursorTest::testGotoCIndex()
{
    const ByteArrayTableLayout layout( NoOfBytesPerLine, FirstLineOffset, StartOffset, ByteArrayOffset, Length );

    ByteArrayTableCursor cursor( &layout );

    // 2 before first valid, as (in)validIndex equals -1, see next test
    Address index = FirstIndex-2;
    const Coord startCoord = layout.startCoord();

    cursor.gotoCIndex( index );

    QCOMPARE( cursor.index(), FirstIndex );
    QCOMPARE( cursor.validIndex(), FirstIndex );
    QCOMPARE( cursor.realIndex(), FirstIndex );
    QCOMPARE( cursor.isBehind(), false );
    QCOMPARE( cursor.atStart(), true );
    QCOMPARE( cursor.atEnd(), false );
    QCOMPARE( cursor.atAppendPos(), false );
    QCOMPARE( cursor.atLineStart(), true );
    QCOMPARE( cursor.atLineEnd(), false );
    QCOMPARE( cursor.coord(), startCoord );

    // 1 before first valid
    index = FirstIndex-1; // hm

    cursor.gotoCIndex( index );

    QCOMPARE( cursor.index(), FirstIndex );
    QCOMPARE( cursor.validIndex(), FirstIndex );
    QCOMPARE( cursor.realIndex(), FirstIndex );
    QCOMPARE( cursor.isBehind(), false );
    QCOMPARE( cursor.atStart(), true );
    QCOMPARE( cursor.atEnd(), false );
    QCOMPARE( cursor.atAppendPos(), false );
    QCOMPARE( cursor.atLineStart(), true );
    QCOMPARE( cursor.atLineEnd(), false );
    QCOMPARE( cursor.coord(), startCoord );

    // at first valid
    index = FirstIndex;
    Coord coord = layout.coordOfIndex( index );

    cursor.gotoCIndex( index );

    QCOMPARE( cursor.index(), index );
    QCOMPARE( cursor.validIndex(), index );
    QCOMPARE( cursor.realIndex(), index );
    QCOMPARE( cursor.isBehind(), false );
    QCOMPARE( cursor.atStart(), true );
    QCOMPARE( cursor.atEnd(), false );
    QCOMPARE( cursor.atAppendPos(), false );
    QCOMPARE( cursor.atLineStart(), true );
    QCOMPARE( cursor.atLineEnd(), false );
    QCOMPARE( cursor.coord(), coord );

    // at last valid
    index = LastIndex;
    coord = layout.coordOfIndex( index );

    cursor.gotoCIndex( index );

    QCOMPARE( cursor.index(), index );
    QCOMPARE( cursor.validIndex(), index );
    QCOMPARE( cursor.realIndex(), index );
    QCOMPARE( cursor.isBehind(), false );
    QCOMPARE( cursor.atStart(), false );
    QCOMPARE( cursor.atEnd(), false );
    QCOMPARE( cursor.atAppendPos(), false );
    QCOMPARE( cursor.atLineStart(), false );
    QCOMPARE( cursor.atLineEnd(), true );
    QCOMPARE( cursor.coord(), coord );

    // 1 after last valid
    index = LastIndex + 1;
    const Coord finalCoord = layout.finalCoord();

    cursor.gotoCIndex( index );

    QCOMPARE( cursor.index(), LastIndex );
    QCOMPARE( cursor.validIndex(), LastIndex );
    QCOMPARE( cursor.realIndex(), index );
    QCOMPARE( cursor.isBehind(), true );
    QCOMPARE( cursor.atStart(), false );
    QCOMPARE( cursor.atEnd(), true );
    QCOMPARE( cursor.atAppendPos(), false );
    QCOMPARE( cursor.atLineStart(), false );
    QCOMPARE( cursor.atLineEnd(), true );
    QCOMPARE( cursor.coord(), finalCoord );

    // 2 after last valid
    index = LastIndex + 2;

    cursor.gotoCIndex( index );

    QCOMPARE( cursor.index(), LastIndex );
    QCOMPARE( cursor.validIndex(), LastIndex );
    QCOMPARE( cursor.realIndex(), LastIndex+1 );
    QCOMPARE( cursor.isBehind(), true );
    QCOMPARE( cursor.atStart(), false );
    QCOMPARE( cursor.atEnd(), true );
    QCOMPARE( cursor.atAppendPos(), false );
    QCOMPARE( cursor.atLineStart(), false );
    QCOMPARE( cursor.atLineEnd(), true );
    QCOMPARE( cursor.coord(), finalCoord );
}

void ByteArrayTableCursorTest::testGotoRealIndex()
{
    const ByteArrayTableLayout layout( NoOfBytesPerLine, FirstLineOffset, StartOffset, ByteArrayOffset, Length );

    ByteArrayTableCursor cursor( &layout );

    // 2 before first valid, as (in)validIndex equals -1, see next test
    Address index = FirstIndex;
    Coord coord = layout.coordOfIndex( index );
    cursor.gotoIndex( index );

    cursor.gotoRealIndex();

    QCOMPARE( cursor.index(), index );
    QCOMPARE( cursor.validIndex(), index );
    QCOMPARE( cursor.realIndex(), index );
    QCOMPARE( cursor.isBehind(), false );
    QCOMPARE( cursor.coord(), coord );

    // after gotoLineEnd in first line (so lineEnd != end)
    cursor.gotoStart();
    cursor.gotoLineEnd();
    index = cursor.index() + 1;
    coord = layout.coordOfIndex( index );

    cursor.gotoRealIndex();

    QCOMPARE( cursor.index(), index );
    QCOMPARE( cursor.validIndex(), index );
    QCOMPARE( cursor.realIndex(), index );
    QCOMPARE( cursor.isBehind(), false );
    QCOMPARE( cursor.coord(), coord );

    // after gotoEnd with append pos disabled
    cursor.setAppendPosEnabled( false );
    cursor.gotoEnd();
    index = cursor.index();
    coord = layout.coordOfIndex( index );

    cursor.gotoRealIndex();

    QCOMPARE( cursor.index(), index );
    QCOMPARE( cursor.validIndex(), index );
    QCOMPARE( cursor.realIndex(), index+1 );
    QCOMPARE( cursor.isBehind(), true );
    QCOMPARE( cursor.coord(), coord );

    // after gotoEnd with append pos disabled
    cursor.setAppendPosEnabled( true );
    cursor.gotoEnd();
    index = cursor.index();
    coord = layout.coordOfIndex( index );

    cursor.gotoRealIndex();

    QCOMPARE( cursor.index(), index );
    QCOMPARE( cursor.validIndex(), -1 );
    QCOMPARE( cursor.realIndex(), index );
    QCOMPARE( cursor.isBehind(), false );
    QCOMPARE( cursor.coord(), coord );
}

void ByteArrayTableCursorTest::testGotoStart()
{
    const ByteArrayTableLayout layout( NoOfBytesPerLine, FirstLineOffset, StartOffset, ByteArrayOffset, Length );
    const Coord startCoord = layout.startCoord();

    ByteArrayTableCursor cursor( &layout );

    cursor.gotoStart();

    QCOMPARE( cursor.index(), FirstIndex );
    QCOMPARE( cursor.validIndex(), FirstIndex );
    QCOMPARE( cursor.realIndex(), FirstIndex );
    QCOMPARE( cursor.isBehind(), false );
    QCOMPARE( cursor.coord(), startCoord );

    // TODO: test for length == 0
}

void ByteArrayTableCursorTest::testGotoEnd()
{
    const ByteArrayTableLayout layout( NoOfBytesPerLine, FirstLineOffset, StartOffset, ByteArrayOffset, Length );
    const Coord finalCoord = layout.finalCoord();

    ByteArrayTableCursor cursor( &layout );

    // without append pos
    cursor.setAppendPosEnabled( false );

    cursor.gotoEnd();

    QCOMPARE( cursor.index(), LastIndex );
    QCOMPARE( cursor.validIndex(), LastIndex );
    QCOMPARE( cursor.realIndex(), LastIndex+1 );
    QCOMPARE( cursor.isBehind(), true );
    QCOMPARE( cursor.coord(), finalCoord );

    // without append pos
    cursor.setAppendPosEnabled( true );
    Coord nextToFinalCoord( finalCoord );
    nextToFinalCoord.goCRight( layout.noOfBytesPerLine()-1 );

    cursor.gotoEnd();

    QCOMPARE( cursor.index(), LastIndex+1 );
    QCOMPARE( cursor.validIndex(), -1 );
    QCOMPARE( cursor.realIndex(), LastIndex+1 );
    QCOMPARE( cursor.isBehind(), false );
    QCOMPARE( cursor.coord(), nextToFinalCoord );

    // TODO: test for length == 0
}

void ByteArrayTableCursorTest::testGotoNextByte()
{
    const ByteArrayTableLayout layout( NoOfBytesPerLine, FirstLineOffset, StartOffset, ByteArrayOffset, Length );

    ByteArrayTableCursor cursor( &layout );

    // at start
    Address index = FirstIndex + 1;
    Coord coord = layout.startCoord();
    coord.goCRight( layout.noOfBytesPerLine()-1 );
    cursor.gotoStart();

    cursor.gotoNextByte();

    QCOMPARE( cursor.index(), index );
    QCOMPARE( cursor.validIndex(), index );
    QCOMPARE( cursor.realIndex(), index );
    QCOMPARE( cursor.isBehind(), false );
    QCOMPARE( cursor.atStart(), false );
    QCOMPARE( cursor.atEnd(), false );
    QCOMPARE( cursor.atAppendPos(), false );
    QCOMPARE( cursor.atLineStart(), false );
    QCOMPARE( cursor.atLineEnd(), false );
    QCOMPARE( cursor.coord(), coord );

    // one before end
    index = LastIndex;
    coord = layout.coordOfIndex( index );
    cursor.gotoIndex( index-1 );

    cursor.gotoNextByte();

    QCOMPARE( cursor.index(), index );
    QCOMPARE( cursor.validIndex(), index );
    QCOMPARE( cursor.realIndex(), index );
    QCOMPARE( cursor.isBehind(), false );
    QCOMPARE( cursor.atStart(), false );
    QCOMPARE( cursor.atEnd(), false );
    QCOMPARE( cursor.atAppendPos(), false );
    QCOMPARE( cursor.atLineStart(), false );
    QCOMPARE( cursor.atLineEnd(), true );
    QCOMPARE( cursor.coord(), coord );

    // at end, append disabled -> noop
    cursor.setAppendPosEnabled( false );
    cursor.gotoEnd();
    index = cursor.index();
    coord = cursor.coord();
    bool isBehind = cursor.isBehind();

    cursor.gotoNextByte();

    QCOMPARE( cursor.index(), index );
    QCOMPARE( cursor.validIndex(), index );
    QCOMPARE( cursor.realIndex(), index+1 );
    QCOMPARE( cursor.isBehind(), isBehind );
    QCOMPARE( cursor.atStart(), false );
    QCOMPARE( cursor.atEnd(), true );
    QCOMPARE( cursor.atAppendPos(), false );
    QCOMPARE( cursor.atLineStart(), false );
    QCOMPARE( cursor.atLineEnd(), true );
    QCOMPARE( cursor.coord(), coord );

    // at end, append enabled -> noop
    cursor.setAppendPosEnabled( true );
    cursor.gotoEnd();
    index = cursor.index();
    coord = cursor.coord();
    isBehind = cursor.isBehind();

    cursor.gotoNextByte();

    QCOMPARE( cursor.index(), index );
    QCOMPARE( cursor.validIndex(), -1 );
    QCOMPARE( cursor.realIndex(), index );
    QCOMPARE( cursor.isBehind(), isBehind );
    QCOMPARE( cursor.atStart(), false );
    QCOMPARE( cursor.atEnd(), true );
    QCOMPARE( cursor.atAppendPos(), true );
    QCOMPARE( cursor.atLineStart(), false );
    QCOMPARE( cursor.atLineEnd(), false );
    QCOMPARE( cursor.coord(), coord );
}

void ByteArrayTableCursorTest::testGotoPreviousByte()
{
    const ByteArrayTableLayout layout( NoOfBytesPerLine, FirstLineOffset, StartOffset, ByteArrayOffset, Length );

    ByteArrayTableCursor cursor( &layout );

    // at start
    cursor.gotoStart();
    Address index = cursor.index();
    Coord coord = cursor.coord();

    cursor.gotoPreviousByte();

    QCOMPARE( cursor.index(), index );
    QCOMPARE( cursor.validIndex(), index );
    QCOMPARE( cursor.realIndex(), index );
    QCOMPARE( cursor.isBehind(), false );
    QCOMPARE( cursor.atStart(), true );
    QCOMPARE( cursor.atEnd(), false );
    QCOMPARE( cursor.atAppendPos(), false );
    QCOMPARE( cursor.atLineStart(), true );
    QCOMPARE( cursor.atLineEnd(), false );
    QCOMPARE( cursor.coord(), coord );

    // one after start
    index = FirstIndex;
    coord = layout.coordOfIndex( index );
    cursor.gotoIndex( index + 1 );

    cursor.gotoPreviousByte();

    QCOMPARE( cursor.index(), index );
    QCOMPARE( cursor.validIndex(), index );
    QCOMPARE( cursor.realIndex(), index );
    QCOMPARE( cursor.isBehind(), false );
    QCOMPARE( cursor.atStart(), true );
    QCOMPARE( cursor.atEnd(), false );
    QCOMPARE( cursor.atAppendPos(), false );
    QCOMPARE( cursor.atLineStart(), true );
    QCOMPARE( cursor.atLineEnd(), false );
    QCOMPARE( cursor.coord(), coord );

    // at end, append disabled -> noop
    cursor.setAppendPosEnabled( false );
    cursor.gotoEnd();
    index = cursor.realIndex() - 1;
    coord = layout.coordOfIndex( index );

    cursor.gotoPreviousByte();

    QCOMPARE( cursor.index(), index );
    QCOMPARE( cursor.validIndex(), index );
    QCOMPARE( cursor.realIndex(), index );
    QCOMPARE( cursor.isBehind(), false );
    QCOMPARE( cursor.atStart(), false );
    QCOMPARE( cursor.atEnd(), false );
    QCOMPARE( cursor.atAppendPos(), false );
    QCOMPARE( cursor.atLineStart(), false );
    QCOMPARE( cursor.atLineEnd(), true );
    QCOMPARE( cursor.coord(), coord );

    // at end, append enabled -> noop
    cursor.setAppendPosEnabled( true );
    cursor.gotoEnd();
    index = cursor.realIndex() - 1;
    coord = layout.coordOfIndex( index );

    cursor.gotoPreviousByte();

    QCOMPARE( cursor.index(), index );
    QCOMPARE( cursor.validIndex(), index );
    QCOMPARE( cursor.realIndex(), index );
    QCOMPARE( cursor.isBehind(), false );
    QCOMPARE( cursor.atStart(), false );
    QCOMPARE( cursor.atEnd(), false );
    QCOMPARE( cursor.atAppendPos(), false );
    QCOMPARE( cursor.atLineStart(), false );
    QCOMPARE( cursor.atLineEnd(), true );
    QCOMPARE( cursor.coord(), coord );
}

void ByteArrayTableCursorTest::testGotoNextByteN()
{
    const ByteArrayTableLayout layout( NoOfBytesPerLine, FirstLineOffset, StartOffset, ByteArrayOffset, Length );

    ByteArrayTableCursor cursor( &layout );

    // at start
    // one right
    Address indexSteps = 1;
    Address expectedIndex = FirstIndex + indexSteps;
    Coord expectedCoord = layout.coordOfIndex( expectedIndex );
    cursor.gotoStart();

    cursor.gotoNextByte( indexSteps );

    QCOMPARE( cursor.index(), expectedIndex );
    QCOMPARE( cursor.validIndex(), expectedIndex );
    QCOMPARE( cursor.realIndex(), expectedIndex );
    QCOMPARE( cursor.isBehind(), false );
    QCOMPARE( cursor.atStart(), false );
    QCOMPARE( cursor.atEnd(), false );
    QCOMPARE( cursor.atAppendPos(), false );
    QCOMPARE( cursor.atLineStart(), false );
    QCOMPARE( cursor.atLineEnd(), false );
    QCOMPARE( cursor.coord(), expectedCoord );

    // one line
    indexSteps = layout.noOfBytesPerLine();
    expectedIndex = FirstIndex + indexSteps;
    expectedCoord = layout.coordOfIndex( expectedIndex );
    cursor.gotoStart();

    cursor.gotoNextByte( indexSteps );

    QCOMPARE( cursor.index(), expectedIndex );
    QCOMPARE( cursor.validIndex(), expectedIndex );
    QCOMPARE( cursor.realIndex(), expectedIndex );
    QCOMPARE( cursor.isBehind(), false );
    QCOMPARE( cursor.atStart(), false );
    QCOMPARE( cursor.atEnd(), false );
    QCOMPARE( cursor.atAppendPos(), false );
    QCOMPARE( cursor.atLineStart(), false );
    QCOMPARE( cursor.atLineEnd(), false );
    QCOMPARE( cursor.coord(), expectedCoord );

    // to end, append disabled
    cursor.setAppendPosEnabled( false );
    indexSteps = layout.length();
    expectedIndex = FirstIndex + indexSteps -1;
    expectedCoord = layout.coordOfIndex( expectedIndex );
    cursor.gotoStart();

    cursor.gotoNextByte( indexSteps );

    QCOMPARE( cursor.index(), expectedIndex );
    QCOMPARE( cursor.validIndex(), expectedIndex );
    QCOMPARE( cursor.realIndex(), expectedIndex+1 );
    QCOMPARE( cursor.isBehind(), true );
    QCOMPARE( cursor.atStart(), false );
    QCOMPARE( cursor.atEnd(), true );
    QCOMPARE( cursor.atAppendPos(), false );
    QCOMPARE( cursor.atLineStart(), false );
    QCOMPARE( cursor.atLineEnd(), true );
    QCOMPARE( cursor.coord(), expectedCoord );

    // to end, append disabled
    cursor.setAppendPosEnabled( true );
    indexSteps = layout.length();
    expectedIndex = FirstIndex + indexSteps;
    expectedCoord = layout.coordOfIndex( expectedIndex );
    cursor.gotoStart();

    cursor.gotoNextByte( indexSteps );

    QCOMPARE( cursor.index(), expectedIndex );
    QCOMPARE( cursor.validIndex(), -1 );
    QCOMPARE( cursor.realIndex(), expectedIndex );
    QCOMPARE( cursor.isBehind(), false );
    QCOMPARE( cursor.atStart(), false );
    QCOMPARE( cursor.atEnd(), true );
    QCOMPARE( cursor.atAppendPos(), true );
    QCOMPARE( cursor.atLineStart(), false );
    QCOMPARE( cursor.atLineEnd(), false );
    QCOMPARE( cursor.coord(), expectedCoord );

    // behind end, append disabled
    cursor.setAppendPosEnabled( false );
    indexSteps = layout.length() + 1;
    expectedIndex = LastIndex;
    expectedCoord = layout.coordOfIndex( expectedIndex );
    cursor.gotoStart();

    cursor.gotoNextByte( indexSteps );

    QCOMPARE( cursor.index(), expectedIndex );
    QCOMPARE( cursor.validIndex(), expectedIndex );
    QCOMPARE( cursor.realIndex(), expectedIndex+1 );
    QCOMPARE( cursor.isBehind(), true );
    QCOMPARE( cursor.atStart(), false );
    QCOMPARE( cursor.atEnd(), true );
    QCOMPARE( cursor.atAppendPos(), false );
    QCOMPARE( cursor.atLineStart(), false );
    QCOMPARE( cursor.atLineEnd(), true );
    QCOMPARE( cursor.coord(), expectedCoord );

    // behind end, append disabled
    cursor.setAppendPosEnabled( true );
    indexSteps = layout.length() + 1;
    expectedIndex = LastIndex + 1;
    expectedCoord = layout.coordOfIndex( expectedIndex );
    cursor.gotoStart();

    cursor.gotoNextByte( indexSteps );

    QCOMPARE( cursor.index(), expectedIndex );
    QCOMPARE( cursor.validIndex(), -1 );
    QCOMPARE( cursor.realIndex(), expectedIndex );
    QCOMPARE( cursor.isBehind(), false );
    QCOMPARE( cursor.atStart(), false );
    QCOMPARE( cursor.atEnd(), true );
    QCOMPARE( cursor.atAppendPos(), true );
    QCOMPARE( cursor.atLineStart(), false );
    QCOMPARE( cursor.atLineEnd(), false );
    QCOMPARE( cursor.coord(), expectedCoord );


    // one before end
    // to end
    indexSteps = 1;
    expectedIndex = LastIndex;
    expectedCoord = layout.coordOfIndex( expectedIndex );
    cursor.gotoIndex( LastIndex-1 );

    cursor.gotoNextByte( indexSteps );

    QCOMPARE( cursor.index(), expectedIndex );
    QCOMPARE( cursor.validIndex(), expectedIndex );
    QCOMPARE( cursor.realIndex(), expectedIndex );
    QCOMPARE( cursor.isBehind(), false );
    QCOMPARE( cursor.atStart(), false );
    QCOMPARE( cursor.atEnd(), false );
    QCOMPARE( cursor.atAppendPos(), false );
    QCOMPARE( cursor.atLineStart(), false );
    QCOMPARE( cursor.atLineEnd(), true );
    QCOMPARE( cursor.coord(), expectedCoord );

    // behind end, append disabled
    cursor.setAppendPosEnabled( false );
    indexSteps = 2;
    expectedIndex = LastIndex;
    expectedCoord = layout.coordOfIndex( expectedIndex );
    cursor.gotoIndex( LastIndex-1 );

    cursor.gotoNextByte( indexSteps );

    QCOMPARE( cursor.index(), expectedIndex );
    QCOMPARE( cursor.validIndex(), expectedIndex );
    QCOMPARE( cursor.realIndex(), expectedIndex+1 );
    QCOMPARE( cursor.isBehind(), true );
    QCOMPARE( cursor.atStart(), false );
    QCOMPARE( cursor.atEnd(), true );
    QCOMPARE( cursor.atAppendPos(), false );
    QCOMPARE( cursor.atLineStart(), false );
    QCOMPARE( cursor.atLineEnd(), true );
    QCOMPARE( cursor.coord(), expectedCoord );

    // behind end, append disabled
    cursor.setAppendPosEnabled( true );
    indexSteps = 2;
    expectedIndex = LastIndex + 1;
    expectedCoord = layout.coordOfIndex( expectedIndex );
    cursor.gotoIndex( LastIndex-1 );

    cursor.gotoNextByte( indexSteps );

    QCOMPARE( cursor.index(), expectedIndex );
    QCOMPARE( cursor.validIndex(), -1 );
    QCOMPARE( cursor.realIndex(), expectedIndex );
    QCOMPARE( cursor.isBehind(), false );
    QCOMPARE( cursor.atStart(), false );
    QCOMPARE( cursor.atEnd(), true );
    QCOMPARE( cursor.atAppendPos(), true );
    QCOMPARE( cursor.atLineStart(), false );
    QCOMPARE( cursor.atLineEnd(), false );
    QCOMPARE( cursor.coord(), expectedCoord );

    // at end
    // TODO: check for end == lineEnd
    // append disabled -> noop
    cursor.setAppendPosEnabled( false );
    indexSteps = 1;
    expectedIndex = LastIndex;
    expectedCoord = layout.coordOfIndex( expectedIndex );
    cursor.gotoEnd();

    cursor.gotoNextByte( indexSteps );

    QCOMPARE( cursor.index(), expectedIndex );
    QCOMPARE( cursor.validIndex(), expectedIndex );
    QCOMPARE( cursor.realIndex(), expectedIndex+1 );
    QCOMPARE( cursor.isBehind(), true );
    QCOMPARE( cursor.atStart(), false );
    QCOMPARE( cursor.atEnd(), true );
    QCOMPARE( cursor.atAppendPos(), false );
    QCOMPARE( cursor.atLineStart(), false );
    QCOMPARE( cursor.atLineEnd(), true );
    QCOMPARE( cursor.coord(), expectedCoord );

    // append enabled -> noop
    cursor.setAppendPosEnabled( true );
    indexSteps = 1;
    expectedIndex = LastIndex + 1;
    expectedCoord = layout.coordOfIndex( expectedIndex );
    cursor.gotoEnd();

    cursor.gotoNextByte( indexSteps );

    QCOMPARE( cursor.index(), expectedIndex );
    QCOMPARE( cursor.validIndex(), -1 );
    QCOMPARE( cursor.realIndex(), expectedIndex );
    QCOMPARE( cursor.isBehind(), false );
    QCOMPARE( cursor.atStart(), false );
    QCOMPARE( cursor.atEnd(), true );
    QCOMPARE( cursor.atAppendPos(), true );
    QCOMPARE( cursor.atLineStart(), false );
    QCOMPARE( cursor.atLineEnd(), false );
    QCOMPARE( cursor.coord(), expectedCoord );
}

void ByteArrayTableCursorTest::testGotoPreviousByteN()
{
    const ByteArrayTableLayout layout( NoOfBytesPerLine, FirstLineOffset, StartOffset, ByteArrayOffset, Length );

    ByteArrayTableCursor cursor( &layout );

    // at start
    Address indexSteps = 1;
    Address expectedIndex = FirstIndex;
    Coord expectedCoord = layout.coordOfIndex( expectedIndex );
    cursor.gotoStart();

    cursor.gotoPreviousByte( indexSteps );

    QCOMPARE( cursor.index(), expectedIndex );
    QCOMPARE( cursor.validIndex(), expectedIndex );
    QCOMPARE( cursor.realIndex(), expectedIndex );
    QCOMPARE( cursor.isBehind(), false );
    QCOMPARE( cursor.atStart(), true );
    QCOMPARE( cursor.atEnd(), false );
    QCOMPARE( cursor.atAppendPos(), false );
    QCOMPARE( cursor.atLineStart(), true );
    QCOMPARE( cursor.atLineEnd(), false );
    QCOMPARE( cursor.coord(), expectedCoord );

    // one behind start
    // to start
    indexSteps = 1;
    expectedIndex = FirstIndex;
    expectedCoord = layout.coordOfIndex( expectedIndex );
    cursor.gotoIndex( FirstIndex+1 );

    cursor.gotoPreviousByte( indexSteps );

    QCOMPARE( cursor.index(), expectedIndex );
    QCOMPARE( cursor.validIndex(), expectedIndex );
    QCOMPARE( cursor.realIndex(), expectedIndex );
    QCOMPARE( cursor.isBehind(), false );
    QCOMPARE( cursor.atStart(), true );
    QCOMPARE( cursor.atEnd(), false );
    QCOMPARE( cursor.atAppendPos(), false );
    QCOMPARE( cursor.atLineStart(), true );
    QCOMPARE( cursor.atLineEnd(), false );
    QCOMPARE( cursor.coord(), expectedCoord );

    // to one before start
    indexSteps = 2;
    expectedIndex = FirstIndex;
    expectedCoord = layout.coordOfIndex( expectedIndex );
    cursor.gotoIndex( FirstIndex+1 );

    cursor.gotoPreviousByte( indexSteps );

    QCOMPARE( cursor.index(), expectedIndex );
    QCOMPARE( cursor.validIndex(), expectedIndex );
    QCOMPARE( cursor.realIndex(), expectedIndex );
    QCOMPARE( cursor.isBehind(), false );
    QCOMPARE( cursor.atStart(), true );
    QCOMPARE( cursor.atEnd(), false );
    QCOMPARE( cursor.atAppendPos(), false );
    QCOMPARE( cursor.atLineStart(), true );
    QCOMPARE( cursor.atLineEnd(), false );
    QCOMPARE( cursor.coord(), expectedCoord );

    // at end
    // append disabled -> noop
    cursor.setAppendPosEnabled( false );
    // to start
    indexSteps = layout.length();
    expectedIndex = FirstIndex;
    expectedCoord = layout.coordOfIndex( expectedIndex );
    cursor.gotoEnd();

    cursor.gotoPreviousByte( indexSteps );

    QCOMPARE( cursor.index(), expectedIndex );
    QCOMPARE( cursor.validIndex(), expectedIndex );
    QCOMPARE( cursor.realIndex(), expectedIndex );
    QCOMPARE( cursor.isBehind(), false );
    QCOMPARE( cursor.atStart(), true );
    QCOMPARE( cursor.atEnd(), false );
    QCOMPARE( cursor.atAppendPos(), false );
    QCOMPARE( cursor.atLineStart(), true );
    QCOMPARE( cursor.atLineEnd(), false );
    QCOMPARE( cursor.coord(), expectedCoord );

    // to one before start
    indexSteps = layout.length() + 1;
    expectedIndex = FirstIndex;
    expectedCoord = layout.coordOfIndex( expectedIndex );
    cursor.gotoEnd();

    cursor.gotoPreviousByte( indexSteps );

    QCOMPARE( cursor.index(), expectedIndex );
    QCOMPARE( cursor.validIndex(), expectedIndex );
    QCOMPARE( cursor.realIndex(), expectedIndex );
    QCOMPARE( cursor.isBehind(), false );
    QCOMPARE( cursor.atStart(), true );
    QCOMPARE( cursor.atEnd(), false );
    QCOMPARE( cursor.atAppendPos(), false );
    QCOMPARE( cursor.atLineStart(), true );
    QCOMPARE( cursor.atLineEnd(), false );
    QCOMPARE( cursor.coord(), expectedCoord );

    // append disabled -> noop
    cursor.setAppendPosEnabled( true );
    // to start
    indexSteps = layout.length();
    expectedIndex = FirstIndex;
    expectedCoord = layout.coordOfIndex( expectedIndex );
    cursor.gotoEnd();

    cursor.gotoPreviousByte( indexSteps );

    QCOMPARE( cursor.index(), expectedIndex );
    QCOMPARE( cursor.validIndex(), expectedIndex );
    QCOMPARE( cursor.realIndex(), expectedIndex );
    QCOMPARE( cursor.isBehind(), false );
    QCOMPARE( cursor.atStart(), true );
    QCOMPARE( cursor.atEnd(), false );
    QCOMPARE( cursor.atAppendPos(), false );
    QCOMPARE( cursor.atLineStart(), true );
    QCOMPARE( cursor.atLineEnd(), false );
    QCOMPARE( cursor.coord(), expectedCoord );

    // to one before start
    indexSteps = layout.length() + 1;
    expectedIndex = FirstIndex;
    expectedCoord = layout.coordOfIndex( expectedIndex );
    cursor.gotoEnd();

    cursor.gotoPreviousByte( indexSteps );

    QCOMPARE( cursor.index(), expectedIndex );
    QCOMPARE( cursor.validIndex(), expectedIndex );
    QCOMPARE( cursor.realIndex(), expectedIndex );
    QCOMPARE( cursor.isBehind(), false );
    QCOMPARE( cursor.atStart(), true );
    QCOMPARE( cursor.atEnd(), false );
    QCOMPARE( cursor.atAppendPos(), false );
    QCOMPARE( cursor.atLineStart(), true );
    QCOMPARE( cursor.atLineEnd(), false );
    QCOMPARE( cursor.coord(), expectedCoord );
}

}

QTEST_MAIN( Okteta::ByteArrayTableCursorTest )
