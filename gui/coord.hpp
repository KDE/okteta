/*
    This file is part of the Okteta Gui library, made within the KDE community.

    SPDX-FileCopyrightText: 2003, 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_COORD_HPP
#define OKTETA_COORD_HPP

// lib
#include "lineposition.hpp"
#include "line.hpp"
// Okteta core
#include <Okteta/Address>
#include <Okteta/Size>

namespace Okteta {

/**
 * a class which represents a coord in a 2-dim. system
 *
 * It consists of a line number and a position in the line.
 * The coord starts at (0,0). Line numbers increase downwards, positions to the right.
 * With any of both at a negative number the coord is invalid.
 * The index at coord(0,0) is 0.
 *
 * @author Friedrich W. H. Kossebau
 */
class Coord
{
public:
    /** constructs a section by width
     * @param index starting index
     * @param lineWidth width of the section
     */
    static Coord fromIndex(Address index, LinePositionSize lineWidth);

public:
    /** creates a coord with 0,0 */
    Coord() = default;
    constexpr Coord(LinePosition pos, Line line);
    Coord(const Coord& other) = default;
    Coord& operator=(const Coord& other) = default;

    ~Coord() = default;

public: // logic
    constexpr bool operator==(const Coord& other) const;
    constexpr bool operator!=(const Coord& other) const;
    constexpr bool operator<(const Coord& other) const;
    constexpr bool operator<=(const Coord& other) const;
    constexpr bool operator>(const Coord& other) const;
    constexpr bool operator>=(const Coord& other) const;

    /** tests if the coord is prior in the same line than the given coord.
     * If at least one of both is invalid the result is undefined.
     * @return true if the pos is left to the pos of other and both are in the same line, otherwise false.
     */
    constexpr bool isPriorInLineThan(const Coord& other) const;
    /** tests if the coord is later in the same line than the given coord.
     * If at least one of both is invalid the result is undefined.
     * @return true if the pos is right to the pos of other and both are in the same line, otherwise false
     */
    constexpr bool isLaterInLineThan(const Coord& other) const;
    /** @return true if the line is below lines, otherwise false */
    constexpr bool isBelow(Line line) const;
    /** @return true if the line is above lines, otherwise false */
    constexpr bool isAbove(Line line) const;
    /** @return true if the coord is at (0,0) */
    constexpr bool isAtStart() const;
    /** @return true if the pos is greater than 0, otherwise false */
    constexpr bool isBehindLineStart() const;
    /** @return true if the pos is smaller than maxPos, otherwise false */
    constexpr bool isBeforeLineEnd(LinePosition maxPos) const;

    /** calculates the index the coord is at with a given line width
     * If the coord is invalid the result is undefined.
     * @param lineWidth given width of line
     * @return index the coord is at
     */
    constexpr Address indexByLineWidth(LinePositionSize lineWidth) const;

public:
    /** set the coord by calculating it for an index with a given line width
     * @param index index in the buffer
     * @param lineWidth given line width
     */
    void setByIndexNWidth(Address index, LinePositionSize lineWidth);
    /** sets both position and line */
    void set(LinePosition pos, Line line);
    /** sets the position */
    void setPos(LinePosition pos);
    /** sets the line */
    void setLine(Line line);

    /** moves the coord one position to the left. If the coord is invalid the result is undefined. */
    void goLeft();
    /** moves the coord a given number of positions to the left.
     * If the coord is invalid the result is undefined or the position smaller than the given number
     * the behaviour is undefined.
     * @param positions number of positions
     */
    void goLeft(LinePositionSize positions);
    /** moves the coord one position to the left, or if the position is already at the line start
     * to the given position in the previous line. If the coord is invalid the result is undefined.
     * @param maxPos maximal allowed position
     */
    void goCLeft(LinePosition maxPos);
    /** moves the coord one position to the right. If the coord is invalid the result is undefined. */
    void goRight();
    /** moves the coord a given number of positions to the right. If the coord is invalid the result is undefined.
     * @param positions number of positions
     */
    void goRight(LinePositionSize positions);
    /** moves the coord one position to the right, or if the position has already reached or passed maxPos
     * to the first position in the next line. If the coord is invalid the result is undefined.
     * @param maxPos maximal allowed position
     */
    void goCRight(LinePosition maxPos);
    /** sets coord to (0,0) */
    void gotoStart();
    void gotoEndOfPreviousLine(LinePosition lastPos);
    /** sets the coord to the start of the next line.
     * If the coord is invalid the behaviour is undefined.
     */
    void gotoStartOfNextLine();
    /** sets the position to the start of the line or
     * if the line is the same as that of the given coord to the position of it.
     * If one or more of the coords is invalid the behaviour is undefined.
     * @param other a possible line start coord
     */
    void goLineStart(const Coord& other);
    /** sets the position to the given pos or
     * if the line is the same as that of the given coord to the position of that.
     * If one or more of the coords is invalid the behaviour is undefined.
     * @param lastPos last position in normal line
     * @param other a possible line end coord
     */
    void goLineEnd(LinePosition lastPos, const Coord& other);
    /** moves the coord 1 lines upwards. There is no check whether the first line is overstepped. */
    void goUp();
    /** moves the coord lines lines downwards. */
    void goDown();
    /** moves the coord lines lines upwards. There is no check whether the first line is overstepped.
     * @param lines number of lines
     */
    void goUp(LineSize lines);
    /** moves the coord lines lines downwards.
     * @param lines number of lines
     */
    void goDown(LineSize lines);

public: // state value access
    /** @return the pos in the line */
    constexpr LinePosition pos() const;
    /** @return the line number */
    constexpr Line line() const;
    /** @return true if the coord is valid */
    constexpr bool isValid() const;

private: // member variables
    /** Position in mLine */
    LinePosition mPos = 0;
    /** mLine */
    Line mLine = 0;
};

inline constexpr Coord::Coord(LinePosition pos, Line line)
    : mPos(pos)
    , mLine(line)
{}

inline Coord Coord::fromIndex(Address index, LinePositionSize lineWidth)
{
    const Line line = index / lineWidth;
    const LinePosition pos  = index - line * lineWidth;
    return Coord(pos, line);
}

inline constexpr bool Coord::operator==(const Coord& other) const { return mPos == other.mPos && mLine == other.mLine; }
inline constexpr bool Coord::operator!=(const Coord& other) const { return !(*this == other); }

inline constexpr bool Coord::operator<(const Coord& other) const
{ return mLine < other.mLine || (mLine == other.mLine && mPos < other.mPos); }
inline constexpr bool Coord::operator<=(const Coord& other) const
{ return mLine < other.mLine || (mLine == other.mLine && mPos <= other.mPos); }
inline constexpr bool Coord::operator>(const Coord& other) const
{ return mLine > other.mLine || (mLine == other.mLine && mPos > other.mPos); }
inline constexpr bool Coord::operator>=(const Coord& other) const
{ return mLine > other.mLine || (mLine == other.mLine && mPos >= other.mPos); }

inline constexpr LinePosition Coord::pos() const { return mPos; }
inline constexpr Line Coord::line()        const { return mLine; }
inline constexpr bool Coord::isValid()      const { return mLine >= 0 && mPos >= 0; }

inline void Coord::setByIndexNWidth(Address index, LinePositionSize lineWidth)
{
    mLine = index / lineWidth;
    mPos  = index - mLine * lineWidth;
}

inline void Coord::set(LinePosition pos, Line line)
{
    mPos  = pos;
    mLine = line;
}
inline void Coord::setPos(LinePosition pos)  { mPos  = pos; }
inline void Coord::setLine(Line line) { mLine = line; }

inline void Coord::goCRight(LinePosition maxPos)
{
    if (isBeforeLineEnd(maxPos)) {
        goRight();
    } else {
        gotoStartOfNextLine();
    }
}
inline void Coord::goCLeft(LinePosition maxPos)
{
    if (isBehindLineStart()) {
        goLeft();
    } else {
        gotoEndOfPreviousLine(maxPos);
    }
}

inline void Coord::goRight() { ++mPos; }
inline void Coord::goLeft()  { --mPos; }
inline void Coord::goRight(LinePositionSize positions) { mPos += positions; }
inline void Coord::goLeft(LinePositionSize positions)  { mPos -= positions; }

inline void Coord::gotoStart() { mPos = mLine = 0; }

inline void Coord::gotoEndOfPreviousLine(LinePosition lastPos)
{
    --mLine;
    mPos = lastPos;
}

inline void Coord::gotoStartOfNextLine()
{
    ++mLine;
    mPos = 0;
}

inline void Coord::goLineStart(const Coord& other)
{
    mPos = (mLine == other.mLine) ? other.mPos : 0;
}

inline void Coord::goLineEnd(LinePosition lastPos, const Coord& other)
{
    mPos = (mLine == other.mLine) ? other.mPos : lastPos;
}

inline void Coord::goUp()           { --mLine; }
inline void Coord::goDown()         { ++mLine; }
inline void Coord::goUp(LineSize lines)    { mLine -= lines; }
inline void Coord::goDown(LineSize lines)  { mLine += lines; }

inline constexpr Address Coord::indexByLineWidth(LinePositionSize lineWidth) const
{
    return mLine * lineWidth + mPos;
}

inline constexpr bool Coord::isPriorInLineThan(const Coord& other) const
{
    return mLine == other.mLine && mPos < other.mPos;
}

inline constexpr bool Coord::isLaterInLineThan(const Coord& other) const
{
    return mLine == other.mLine && mPos > other.mPos;
}

inline constexpr bool Coord::isBelow(Line line) const { return mLine > line; }
inline constexpr bool Coord::isAbove(Line line) const { return mLine < line; }

inline constexpr bool Coord::isBehindLineStart()           const { return mPos > 0; }
inline constexpr bool Coord::isBeforeLineEnd(LinePosition maxPos) const { return mPos < maxPos; }

inline constexpr bool Coord::isAtStart()                   const { return mPos == 0 && mLine == 0; }

inline Coord operator+(const Coord& other, LinePosition pos)
{
    return Coord(other.pos() + pos, other.line());
}

}

#endif
