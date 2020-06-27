/*
    This file is part of the Okteta Gui library, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearraytablelayouttest.hpp"

// test object
#include <bytearraytablelayout.hpp>
// Qt
#include <QTest>

namespace Okteta {

// local variables
static constexpr Size NoOfBytesPerLine = 8;
static constexpr Address StartOffset = 22;
static constexpr Address FirstLineOffset = 10;
static constexpr Address RelativeStartOffset = StartOffset - FirstLineOffset;
static constexpr Address ByteArrayOffset = 9;
static constexpr Size Length = 250;
static constexpr Address FinalOffset = (StartOffset + Length - 1);
static constexpr Address RelativeFinalOffset = FinalOffset - FirstLineOffset;

static constexpr Line StartLine = RelativeStartOffset / NoOfBytesPerLine;
static constexpr LinePosition StartLinePosition = RelativeStartOffset % NoOfBytesPerLine;
static constexpr Coord StartCoord(StartLinePosition, StartLine);

static constexpr Line FinalLine =  RelativeFinalOffset / NoOfBytesPerLine;
static constexpr LinePosition FinalLinePosition = RelativeFinalOffset % NoOfBytesPerLine;
static constexpr Coord FinalCoord(FinalLinePosition, FinalLine);

static constexpr LineSize NoOfLinesPerPage = 5;


void ByteArrayTableLayoutTest::testConstructor()
{
    ByteArrayTableLayout layout(NoOfBytesPerLine, FirstLineOffset, StartOffset, ByteArrayOffset, Length);

    QCOMPARE(layout.noOfBytesPerLine(), NoOfBytesPerLine);
    QCOMPARE(layout.startOffset(), StartOffset);
    QCOMPARE(layout.byteArrayOffset(), ByteArrayOffset);
    QCOMPARE(layout.length(), Length);
    QCOMPARE(layout.startLine(), StartLine);
    QCOMPARE(layout.firstStartLinePosition(), StartLinePosition);
    QCOMPARE(layout.startCoord(), StartCoord);
    QCOMPARE(layout.finalLine(), FinalLine);
    QCOMPARE(layout.lastFinalLinePosition(), FinalLinePosition);
    QCOMPARE(layout.finalCoord(), FinalCoord);
    QCOMPARE(layout.noOfLines(), FinalLine + 1);
    QCOMPARE(layout.noOfLinesPerPage(), 1);
}

void ByteArrayTableLayoutTest::testSetStartOffset()
{
    ByteArrayTableLayout layout(NoOfBytesPerLine, FirstLineOffset, 0, ByteArrayOffset, Length);

    layout.setStartOffset(StartOffset);
    QCOMPARE(layout.noOfBytesPerLine(), NoOfBytesPerLine);
    QCOMPARE(layout.startOffset(), StartOffset);
    QCOMPARE(layout.byteArrayOffset(), ByteArrayOffset);
    QCOMPARE(layout.length(), Length);
    QCOMPARE(layout.startLine(), StartLine);
    QCOMPARE(layout.firstStartLinePosition(), StartLinePosition);
    QCOMPARE(layout.startCoord(), StartCoord);
    QCOMPARE(layout.finalLine(), FinalLine);
    QCOMPARE(layout.lastFinalLinePosition(), FinalLinePosition);
    QCOMPARE(layout.finalCoord(), FinalCoord);
    QCOMPARE(layout.noOfLines(), FinalLine + 1);
    QCOMPARE(layout.noOfLinesPerPage(), 1);
}

void ByteArrayTableLayoutTest::testSetFirstLineOffset()
{
    ByteArrayTableLayout layout(NoOfBytesPerLine, 0, StartOffset, ByteArrayOffset, Length);

    layout.setFirstLineOffset(FirstLineOffset);
    QCOMPARE(layout.noOfBytesPerLine(), NoOfBytesPerLine);
    QCOMPARE(layout.startOffset(), StartOffset);
    QCOMPARE(layout.byteArrayOffset(), ByteArrayOffset);
    QCOMPARE(layout.length(), Length);
    QCOMPARE(layout.startLine(), StartLine);
    QCOMPARE(layout.firstStartLinePosition(), StartLinePosition);
    QCOMPARE(layout.startCoord(), StartCoord);
    QCOMPARE(layout.finalLine(), FinalLine);
    QCOMPARE(layout.lastFinalLinePosition(), FinalLinePosition);
    QCOMPARE(layout.finalCoord(), FinalCoord);
    QCOMPARE(layout.noOfLines(), FinalLine + 1);
    QCOMPARE(layout.noOfLinesPerPage(), 1);
}

void ByteArrayTableLayoutTest::testSetNoOfBytesPerLine()
{
    ByteArrayTableLayout layout(1, FirstLineOffset, StartOffset, ByteArrayOffset, Length);

    layout.setNoOfBytesPerLine(NoOfBytesPerLine);
    QCOMPARE(layout.noOfBytesPerLine(), NoOfBytesPerLine);
    QCOMPARE(layout.startOffset(), StartOffset);
    QCOMPARE(layout.byteArrayOffset(), ByteArrayOffset);
    QCOMPARE(layout.length(), Length);
    QCOMPARE(layout.startLine(), StartLine);
    QCOMPARE(layout.firstStartLinePosition(), StartLinePosition);
    QCOMPARE(layout.startCoord(), StartCoord);
    QCOMPARE(layout.finalLine(), FinalLine);
    QCOMPARE(layout.lastFinalLinePosition(), FinalLinePosition);
    QCOMPARE(layout.finalCoord(), FinalCoord);
    QCOMPARE(layout.noOfLines(), FinalLine + 1);
    QCOMPARE(layout.noOfLinesPerPage(), 1);
}

void ByteArrayTableLayoutTest::testSetLength()
{
    ByteArrayTableLayout layout(NoOfBytesPerLine, FirstLineOffset, StartOffset, ByteArrayOffset, 1);

    layout.setLength(Length);
    QCOMPARE(layout.noOfBytesPerLine(), NoOfBytesPerLine);
    QCOMPARE(layout.startOffset(), StartOffset);
    QCOMPARE(layout.byteArrayOffset(), ByteArrayOffset);
    QCOMPARE(layout.length(), Length);
    QCOMPARE(layout.startLine(), StartLine);
    QCOMPARE(layout.firstStartLinePosition(), StartLinePosition);
    QCOMPARE(layout.startCoord(), StartCoord);
    QCOMPARE(layout.finalLine(), FinalLine);
    QCOMPARE(layout.lastFinalLinePosition(), FinalLinePosition);
    QCOMPARE(layout.finalCoord(), FinalCoord);
    QCOMPARE(layout.noOfLines(), FinalLine + 1);
    QCOMPARE(layout.noOfLinesPerPage(), 1);
}

void ByteArrayTableLayoutTest::testSetByteArrayOffset()
{
    ByteArrayTableLayout layout(NoOfBytesPerLine, FirstLineOffset, StartOffset, 0, Length);

    layout.setByteArrayOffset(ByteArrayOffset);
    QCOMPARE(layout.noOfBytesPerLine(), NoOfBytesPerLine);
    QCOMPARE(layout.startOffset(), StartOffset);
    QCOMPARE(layout.byteArrayOffset(), ByteArrayOffset);
    QCOMPARE(layout.length(), Length);
    QCOMPARE(layout.startLine(), StartLine);
    QCOMPARE(layout.firstStartLinePosition(), StartLinePosition);
    QCOMPARE(layout.startCoord(), StartCoord);
    QCOMPARE(layout.finalLine(), FinalLine);
    QCOMPARE(layout.lastFinalLinePosition(), FinalLinePosition);
    QCOMPARE(layout.finalCoord(), FinalCoord);
    QCOMPARE(layout.noOfLines(), FinalLine + 1);
    QCOMPARE(layout.noOfLinesPerPage(), 1);
}

void ByteArrayTableLayoutTest::testSetNoOfLinesPerPage()
{
    ByteArrayTableLayout layout(NoOfBytesPerLine, FirstLineOffset, StartOffset, ByteArrayOffset, Length);

    layout.setNoOfLinesPerPage(NoOfLinesPerPage);
    QCOMPARE(layout.noOfBytesPerLine(), NoOfBytesPerLine);
    QCOMPARE(layout.startOffset(), StartOffset);
    QCOMPARE(layout.byteArrayOffset(), ByteArrayOffset);
    QCOMPARE(layout.length(), Length);
    QCOMPARE(layout.startLine(), StartLine);
    QCOMPARE(layout.firstStartLinePosition(), StartLinePosition);
    QCOMPARE(layout.startCoord(), StartCoord);
    QCOMPARE(layout.finalLine(), FinalLine);
    QCOMPARE(layout.lastFinalLinePosition(), FinalLinePosition);
    QCOMPARE(layout.finalCoord(), FinalCoord);
    QCOMPARE(layout.noOfLines(), FinalLine + 1);
    QCOMPARE(layout.noOfLinesPerPage(), NoOfLinesPerPage);
}

}

QTEST_MAIN(Okteta::ByteArrayTableLayoutTest)
