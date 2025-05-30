/*
    This file is part of the Okteta Gui library, made within the KDE community.

    SPDX-FileCopyrightText: 2003, 2008-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_BYTEARRAYTABLELAYOUT_HPP
#define OKTETA_BYTEARRAYTABLELAYOUT_HPP

// lib
#include "oktetagui_export.hpp"
#include "coordrange.hpp"
// Okteta core
#include <Okteta/AddressRange>

// TODO: rename things like startOffset and firstLineOffset, not really descriptive
namespace Okteta {

/**@short the logical layout of a byte array table for a view
 *
 * Given the values for
 * * number of bytes per line,
 * * a possible offset of the first line displayed,
 * * a possible offset of the displayed bytearray, and
 * * length of the byte array as well as
 * * offset in the byte array
 * * the number of lines per page jump
 * the following values are calculated:
 * * starting line of display,
 * * starting position in this line,
 * * final line of display,
 * * final position in this line, and
 * * the total number of lines (is final line +1 or 0)
 *
 * This layout sees the buffer as a continuous stream of byte,
 * thus uses each line after the start from the begin to the end.
 *
 * If the buffer is empty the end coord will be set one pos left to the start coord
 * to easen the cursor handling.
 *
 * @author Friedrich W. H.  Kossebau
 */
class OKTETAGUI_EXPORT ByteArrayTableLayout
{
public:
    ByteArrayTableLayout(Size noOfBytesPerLine, Address firstLineOffset, Address startOffset, Address byteArrayOffset, Size byteArrayLength);
    ByteArrayTableLayout() = delete;
    ByteArrayTableLayout(const ByteArrayTableLayout&) = delete;
    ByteArrayTableLayout(ByteArrayTableLayout&&) = delete;

    ~ByteArrayTableLayout();

    ByteArrayTableLayout& operator=(const ByteArrayTableLayout&) = delete;
    ByteArrayTableLayout& operator=(ByteArrayTableLayout&&) = delete;

public: // given values
    /** */
    [[nodiscard]]
    Address startOffset() const;
    /** */
    [[nodiscard]]
    Address firstLineOffset() const;
    /** returns number of bytes per line */
    [[nodiscard]]
    Size noOfBytesPerLine() const;
    /** returns the offset of the start of the displayed byte array section */
    [[nodiscard]]
    Address byteArrayOffset() const;
    /** returns the length of the displayed byte array section */
    [[nodiscard]]
    Size length() const;
    /** returns number of lines per visual page */
    [[nodiscard]]
    LineSize noOfLinesPerPage() const;

public: // calculated values
    [[nodiscard]]
    Line startLine() const;
    [[nodiscard]]
    LinePosition firstStartLinePosition() const;
    /** returns the coord of the start */
    [[nodiscard]]
    Coord startCoord() const;

    [[nodiscard]]
    Line finalLine() const;
    [[nodiscard]]
    LinePosition lastFinalLinePosition() const;
    /** returns the coord of the end */
    [[nodiscard]]
    Coord finalCoord() const;

    [[nodiscard]]
    Address lastByteArrayOffset() const;
    /** tells how much lines this layout needs (incl. blank leading lines due to mStartOffset and mFirstLineOffset) */
    [[nodiscard]]
    LineSize noOfLines() const;

public: // value calculation service
    /** calculates the index of the coord
     * If the coord is before the first coord the first index is returned,
     * if the coord is behind the last coord the last index is returned
     */
    [[nodiscard]]
    Address indexAtCCoord(Coord coord) const;
    /** calculates the index of the first pos in line.
     * If the line is below the first line the first index is returned,
     * if the line is above the last line the last index is returned
     */
    [[nodiscard]]
    Address indexAtCFirstLinePosition(Line line) const;
    /** calculates the index of last pos in line
     * If the line is below the first line the first index is returned,
     * if the line is above the last line the last index is returned
     */
    [[nodiscard]]
    Address indexAtCLastLinePosition(Line line) const;
    /** calculates the line in which index is found
     * If the index is below the first index the first line is returned,
     * if the index is above the last index the last line is returned
     */
    [[nodiscard]]
    Line lineAtCIndex(Address index) const;
    /** calculates the coord in which index is found
     * If the index is below the first index the first coord is returned,
     * if the index is above the last index the last coord is returned
     */
    [[nodiscard]]
    Coord coordOfCIndex(Address index) const;

    /** calculates the index of coord. if coord is invalid the behaviour is undefinded */
    [[nodiscard]]
    Address indexAtCoord(Coord coord) const;
    /** calculates the index of the first pos in line. if line is invalid the behaviour is undefinded */
    [[nodiscard]]
    Address indexAtFirstLinePosition(Line line) const;
    /** calculates the index of last pos in line. if line is invalid the behaviour is undefinded */
    [[nodiscard]]
    Address indexAtLastLinePosition(Line line) const;
    /** calculates the line in which index is found. if index is invalid the behaviour is undefinded */
    [[nodiscard]]
    Line lineAtIndex(Address index) const;
    /** calculates the coord in which index is found. if index is invalid the behaviour is undefinded */
    [[nodiscard]]
    Coord coordOfIndex(Address index) const;
    /** calculates the range of coords in which the indizes are found. if indizes are invalid the behaviour is undefinded */
    [[nodiscard]]
    CoordRange coordRangeOfIndizes(const AddressRange& indizes) const;

    /** returns the used positions in line */
    [[nodiscard]]
    LinePositionRange linePositions(Line line) const;
    /** returns the first Pos in line. if line is invalid the behaviour is undefinded */
    [[nodiscard]]
    LinePosition firstLinePosition(Line line) const;
    /** returns the last Pos in line. if line is invalid the behaviour is undefinded */
    [[nodiscard]]
    LinePosition lastLinePosition(Line line) const;
    /** returns the valid Pos or the first Pos in line. if coord is invalid the behaviour is undefinded */
    [[nodiscard]]
    LinePosition firstLinePosition(Coord coord) const;
    /** returns the valid Pos or the last Pos in line. if coord is invalid the behaviour is undefinded */
    [[nodiscard]]
    LinePosition lastLinePosition(Coord coord) const;
    /** returns true if the line has content */
    [[nodiscard]]
    bool hasContent(Line line) const;
    /** returns true if the coord is the first in it's line. if coord is invalid the behaviour is undefinded */
    [[nodiscard]]
    bool atFirstLinePosition(Coord coord) const;
    /** returns true if the coord is the last in it's line. if coord is invalid the behaviour is undefinded */
    [[nodiscard]]
    bool atLastLinePosition(Coord coord) const;

    /** returns the index if valid or the nearest valid index */
    [[nodiscard]]
    Address correctIndex(Address index) const;
    /** returns the coord if valid or the nearest valid coord */
    [[nodiscard]]
    Coord correctCoord(Coord coord) const;

public: // modification access; return true if changes
    /** sets mStartOffset, returns true if changed */
    bool setStartOffset(Address startOffset);
    /** sets mStartOffset, returns true if changed */
    bool setFirstLineOffset(Address firstLineOffset);
    /** sets number of bytes per line, returns true if changed */
    bool setNoOfBytesPerLine(LineSize noOfBytesPerLine);
    /** sets offset in the data to display, returns true if changed */
    bool setByteArrayOffset(Address byteArrayOffset);
    /** sets length of data to display, returns true if changed */
    bool setLength(Size length);
    /** sets number of lines per page, 1 as default */
    void setNoOfLinesPerPage(LineSize noOfLinesPerPage);

private:
    /** calculates the start coord by startoffset and number of bytes per line */
    OKTETAGUI_NO_EXPORT void calcStart();
    /** calculates the final coord by startoffset, length, and number of bytes per line */
    OKTETAGUI_NO_EXPORT void calcEnd();

private:
    /** how many chars per line */
    Size mNoOfBytesPerLine;
    /** starting offset of the first displayed line */
    Address mFirstLineOffset;
    /** starting offset of the displayed bytearray */
    Address mStartOffset;
    /** */
    Address mRelativeStartOffset;
    /** offset in the given bytearray */
    Address mByteArrayOffset;
    /** last offset in the displayed bytearray section */
    Address mLastByteArrayOffset;
    /** number of lines that are moved by page up/down */
    LineSize mNoOfLinesPerPage;

private: // calculated values, buffered
    /** */
    CoordRange mCoordRange;
};

inline Address ByteArrayTableLayout::startOffset()         const { return mStartOffset; }
inline Address ByteArrayTableLayout::firstLineOffset()     const { return mFirstLineOffset; }
inline Size ByteArrayTableLayout::noOfBytesPerLine()       const { return mNoOfBytesPerLine; }
inline Address ByteArrayTableLayout::byteArrayOffset()     const { return mByteArrayOffset; }
inline Size ByteArrayTableLayout::length()                 const { return mLastByteArrayOffset - mByteArrayOffset + 1; }
inline Address ByteArrayTableLayout::lastByteArrayOffset() const { return mLastByteArrayOffset; }

inline Coord ByteArrayTableLayout::finalCoord()               const { return mCoordRange.end(); }
inline Coord ByteArrayTableLayout::startCoord()               const { return mCoordRange.start(); }
inline LinePosition ByteArrayTableLayout::firstStartLinePosition() const { return startCoord().pos(); }
inline LinePosition ByteArrayTableLayout::lastFinalLinePosition() const { return finalCoord().pos(); }
inline Line ByteArrayTableLayout::startLine()                 const { return startCoord().line(); }
inline Line ByteArrayTableLayout::finalLine()                 const { return finalCoord().line(); }
inline LineSize ByteArrayTableLayout::noOfLinesPerPage()      const { return mNoOfLinesPerPage; }
inline LineSize ByteArrayTableLayout::noOfLines()             const { return mByteArrayOffset > mLastByteArrayOffset ? 0 : finalLine() + 1; }

}

#endif
