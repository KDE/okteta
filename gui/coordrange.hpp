/*
    This file is part of the Okteta Gui library, made within the KDE community.

    SPDX-FileCopyrightText: 2003, 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_COORDRANGE_HPP
#define OKTETA_COORDRANGE_HPP

// lib
#include "coord.hpp"
#include "linepositionrange.hpp"
#include "linerange.hpp"
// Okteta core
#include <Okteta/Range>

namespace Okteta {

using BaseCoordRange = Range<Coord>;

template <>
inline const Coord BaseCoordRange::null() const
{ return Coord(-1, -1); }


/** describes a range in the buffercoord
 * @author Friedrich W. H.  Kossebau
 */
class CoordRange : public BaseCoordRange
{
public:
    /**
     * @param start start coord
     * @param end end coord
     */
    CoordRange(const Coord& start, const Coord& end);
    /**
     * @param posRange start and end pos
     * @param lineRange start and end line
     */
    CoordRange(const LinePositionRange& posRange, const LineRange& lineRange);
    CoordRange(const CoordRange& other);
    CoordRange();
    ~CoordRange();

public:
    CoordRange& operator=(const CoordRange& other);

public:
    bool operator==(const CoordRange& other) const;

public:
    /** calculates the number of coords that are covered if a line has the given length.
     * If the range is invalid the behaviour is undefined.
     * @param lineWidth
     * @return the number of points covered if a line has a length of LineLength.
     */
    Size width(LinePositionSize lineWidth) const;
    /** calculates the number of lines that are covered by the range.
     * If the range is invalid the behaviour is undefined.
     * @return number of lines covered
     */
    LineSize lines() const;
    /** tests if the given line is included by the range.
     * If the range is invalid or the line < 0 the behaviour is undefined.
     * @param line index of line
     * @return @c true if Line is included, otherwise @c false
     */
    bool includesLine(Line line) const;
};

inline CoordRange::CoordRange(const Coord& start, const Coord& end) : BaseCoordRange(start, end) {}
inline CoordRange::CoordRange(const LinePositionRange& posRange, const LineRange& lineRange)
    : BaseCoordRange(Coord(posRange.start(), lineRange.start()), Coord(posRange.end(), lineRange.end()))
{}
inline CoordRange::CoordRange(const CoordRange& other) = default;
inline CoordRange::CoordRange() = default;

inline CoordRange::~CoordRange() = default;

inline CoordRange& CoordRange::operator=(const CoordRange& other) = default;

inline bool CoordRange::operator==(const CoordRange& other) const { return BaseCoordRange::operator==(other); }

inline Size CoordRange::width(LinePositionSize lineWidth)   const { return lineWidth * (lines() - 1) + end().pos() - start().pos() + 1; }
inline LineSize CoordRange::lines()                         const { return end().line() - start().line() + 1; }
inline bool CoordRange::includesLine(Line line)             const { return (start().line() <= line && line <= end().line()); }

}

#endif
