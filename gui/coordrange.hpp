/*
    This file is part of the Okteta Gui library, made within the KDE community.

    Copyright 2003,2009 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef OKTETA_COORDRANGE_HPP
#define OKTETA_COORDRANGE_HPP

// lib
#include <okteta/coord.hpp>
#include <okteta/linepositionrange.hpp>
#include <okteta/linerange.hpp>
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
    CoordRange(Coord start, Coord end);
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

inline CoordRange::CoordRange(Coord start, Coord end) : BaseCoordRange(start, end) {}
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
