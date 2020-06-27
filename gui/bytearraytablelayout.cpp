/*
    This file is part of the Okteta Gui library, made within the KDE community.

    SPDX-FileCopyrightText: 2003, 2008-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearraytablelayout.hpp"

namespace Okteta {

static constexpr LineSize DefaultNoOfLinesPerPage = 1;

ByteArrayTableLayout::ByteArrayTableLayout(Size noOfBytesPerLine, Address firstLineOffset, Address startOffset,
                                           Address byteArrayOffset, Size byteArrayLength)
    : mNoOfBytesPerLine(noOfBytesPerLine)
    , mFirstLineOffset(firstLineOffset)
    , mStartOffset(startOffset)
    , mRelativeStartOffset(startOffset - firstLineOffset)
    , mByteArrayOffset(byteArrayOffset)
    , mLastByteArrayOffset(byteArrayOffset + byteArrayLength - 1)
    , mNoOfLinesPerPage(DefaultNoOfLinesPerPage)
{
    calcStart();
    calcEnd();
}

ByteArrayTableLayout::~ByteArrayTableLayout() = default;

bool ByteArrayTableLayout::setStartOffset(Address startOffset)
{
    // rejecting <0
    if (startOffset < 0) {
        startOffset = 0;
    }

    if (mStartOffset == startOffset) {
        return false;
    }

    mStartOffset = startOffset;
    mRelativeStartOffset = mStartOffset - mFirstLineOffset;

    calcStart();
    calcEnd();
    return true;
}

bool ByteArrayTableLayout::setFirstLineOffset(Address firstLineOffset)
{
    // rejecting <0
    if (firstLineOffset < 0) {
        firstLineOffset = 0;
    }

    if (mFirstLineOffset == firstLineOffset) {
        return false;
    }

    mFirstLineOffset = firstLineOffset;
    mRelativeStartOffset = mStartOffset - mFirstLineOffset;

    calcStart();
    calcEnd();
    return true;
}

bool ByteArrayTableLayout::setNoOfBytesPerLine(LineSize noOfBytesPerLine)
{
    // rejecting <1
    if (noOfBytesPerLine < 1) {
        noOfBytesPerLine = 1;
    }

    // no changes?
    if (mNoOfBytesPerLine == noOfBytesPerLine) {
        return false;
    }

    mNoOfBytesPerLine = noOfBytesPerLine;

    calcStart();
    calcEnd();
    return true;
}

bool ByteArrayTableLayout::setByteArrayOffset(Address byteArrayOffset)
{
    // rejecting < 0
    if (byteArrayOffset < 0) {
        byteArrayOffset = 0;
    }

    // no changes?
    if (mByteArrayOffset == byteArrayOffset) {
        return false;
    }

    const Size l = length();

    mByteArrayOffset = byteArrayOffset;
    mLastByteArrayOffset = mByteArrayOffset + l - 1;

    calcEnd();
    return true;
}

bool ByteArrayTableLayout::setLength(Size length)
{
    // rejecting < 0
    if (length < 0) {
        length = 0;
    }

    const Address newLastByteArrayOffset = mByteArrayOffset + length - 1;

    // no changes?
    if (mLastByteArrayOffset == newLastByteArrayOffset) {
        return false;
    }

    mLastByteArrayOffset = newLastByteArrayOffset;

    calcEnd();
    return true;
}

void ByteArrayTableLayout::setNoOfLinesPerPage(LineSize noOfLinesPerPage)
{
    mNoOfLinesPerPage = noOfLinesPerPage;
}

void ByteArrayTableLayout::calcStart()
{
    mCoordRange.setStart(Coord::fromIndex(mRelativeStartOffset, mNoOfBytesPerLine));
}

void ByteArrayTableLayout::calcEnd()
{
    const Size l = length();
    mCoordRange.setEnd((l > 0) ?
                       Coord::fromIndex(l - 1 + mRelativeStartOffset, mNoOfBytesPerLine) :
                       Coord(-1, mCoordRange.start().line()));
}

Address ByteArrayTableLayout::indexAtCFirstLinePosition(Line line) const
{
    return (line <= mCoordRange.start().line()) ? mByteArrayOffset :
           (line > mCoordRange.end().line()) ?    mLastByteArrayOffset :
                                                  line* mNoOfBytesPerLine - mRelativeStartOffset + mByteArrayOffset;
}

Address ByteArrayTableLayout::indexAtCLastLinePosition(Line line) const
{
    return (line < mCoordRange.start().line()) ? mByteArrayOffset :
           (line >= mCoordRange.end().line()) ?  mLastByteArrayOffset :
                                                 (line + 1) * mNoOfBytesPerLine - mRelativeStartOffset - 1 + mByteArrayOffset;
}

Address ByteArrayTableLayout::indexAtCCoord(const Coord& coord) const
{
    const Address index = indexAtCoord(coord);

    return (index <= mByteArrayOffset) ?     mByteArrayOffset :
           (index >= mLastByteArrayOffset) ? mLastByteArrayOffset :
                                             index;
}

Line ByteArrayTableLayout::lineAtCIndex(Address index) const
{
    return (index <= mByteArrayOffset) ?     mCoordRange.start().line() :
           (index >= mLastByteArrayOffset) ? mCoordRange.end().line() :
                                             lineAtIndex(index);
}

Coord ByteArrayTableLayout::coordOfCIndex(Address index) const
{
    return (index <= mByteArrayOffset) ?     mCoordRange.start() :
           (index >= mLastByteArrayOffset) ? mCoordRange.end() :
                                             coordOfIndex(index);
}

Address ByteArrayTableLayout::indexAtFirstLinePosition(Line line) const
{
    return (line == mCoordRange.start().line()) ? mByteArrayOffset : line* mNoOfBytesPerLine - mRelativeStartOffset + mByteArrayOffset;
}

Address ByteArrayTableLayout::indexAtLastLinePosition(Line line) const
{
    return (line == mCoordRange.end().line()) ? mLastByteArrayOffset : (line + 1) * mNoOfBytesPerLine - mRelativeStartOffset + mByteArrayOffset - 1;
}

Address ByteArrayTableLayout::indexAtCoord(const Coord& coord) const
{
    return coord.indexByLineWidth(mNoOfBytesPerLine) - mRelativeStartOffset + mByteArrayOffset;
}

Line ByteArrayTableLayout::lineAtIndex(Address index) const
{
    return (index + mRelativeStartOffset - mByteArrayOffset) / mNoOfBytesPerLine;
}

Coord ByteArrayTableLayout::coordOfIndex(Address index) const
{
    return Coord::fromIndex(index + mRelativeStartOffset - mByteArrayOffset, mNoOfBytesPerLine);
}

CoordRange ByteArrayTableLayout::coordRangeOfIndizes(const AddressRange& indizes) const
{
    return CoordRange(
        Coord::fromIndex(indizes.start() + mRelativeStartOffset - mByteArrayOffset, mNoOfBytesPerLine),
        Coord::fromIndex(indizes.end() + mRelativeStartOffset - mByteArrayOffset,   mNoOfBytesPerLine));
}

Address ByteArrayTableLayout::correctIndex(Address index) const
{
    return (index <= mByteArrayOffset) ?     mByteArrayOffset :
           (index >= mLastByteArrayOffset) ? mLastByteArrayOffset :
                                             index;
}

Coord ByteArrayTableLayout::correctCoord(const Coord& coord) const
{
    return (coord <= mCoordRange.start()) ?     mCoordRange.start() :
           (coord >= mCoordRange.end()) ?       mCoordRange.end() :
           (coord.pos() >= mNoOfBytesPerLine) ? Coord(mNoOfBytesPerLine - 1, coord.line()) :
                                                coord;
}

bool ByteArrayTableLayout::atFirstLinePosition(const Coord& coord) const
{
    return (coord.line() == mCoordRange.start().line()) ? coord.pos() == mCoordRange.start().pos() :
                                                          coord.pos() == 0;
}

bool ByteArrayTableLayout::atLastLinePosition(const Coord& coord) const
{
    return (coord.line() == mCoordRange.end().line()) ? coord.pos() == mCoordRange.end().pos() :
                                                        coord.pos() == mNoOfBytesPerLine - 1;
}

LinePositionRange ByteArrayTableLayout::linePositions(Line line) const
{
    return LinePositionRange(firstLinePosition(line), lastLinePosition(line));
}

LinePosition ByteArrayTableLayout::firstLinePosition(const Coord& coord) const
{
    return (mCoordRange.start().isLaterInLineThan(coord)) ? mCoordRange.start().pos() : coord.pos();
}

LinePosition ByteArrayTableLayout::lastLinePosition(const Coord& coord) const
{
    return (mCoordRange.end().isPriorInLineThan(coord)) ? mCoordRange.end().pos() : coord.pos();
}

LinePosition ByteArrayTableLayout::firstLinePosition(Line line) const
{
    return line == mCoordRange.start().line() ? mCoordRange.start().pos() : 0;
}

LinePosition ByteArrayTableLayout::lastLinePosition(Line line) const
{
    return (line == mCoordRange.end().line()) ? mCoordRange.end().pos() : mNoOfBytesPerLine - 1;
}

bool ByteArrayTableLayout::hasContent(Line line) const
{
    return mCoordRange.includesLine(line);
}

}
