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
// Qt
#include <QTypeInfo>

namespace Okteta {

template<>
struct RangeNullValue<Coord>
{
    constexpr Coord operator()() const
    {
        return Coord(-1, -1);
    }
};


/** describes a range in the buffercoord
 * @author Friedrich W. H.  Kossebau
 */
class CoordRange : public Range<Coord>
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
    CoordRange(LinePositionRange posRange, LineRange lineRange);
    CoordRange(const CoordRange& other) = default; // trivial
    CoordRange(CoordRange&& other) = default; // trivial
    CoordRange();
    ~CoordRange() = default; // trivial

public:
    CoordRange& operator=(const CoordRange& other) = default; // trivial
    CoordRange& operator=(CoordRange&& other) = default; // trivial

public:
    [[nodiscard]]
    bool operator==(const CoordRange& other) const;

public:
    /** calculates the number of coords that are covered if a line has the given length.
     * If the range is invalid the behaviour is undefined.
     * @param lineWidth
     * @return the number of points covered if a line has a length of LineLength.
     */
    [[nodiscard]]
    Size width(LinePositionSize lineWidth) const;
    /** calculates the number of lines that are covered by the range.
     * If the range is invalid the behaviour is undefined.
     * @return number of lines covered
     */
    [[nodiscard]]
    LineSize lines() const;
    /** tests if the given line is included by the range.
     * If the range is invalid or the line < 0 the behaviour is undefined.
     * @param line index of line
     * @return @c true if Line is included, otherwise @c false
     */
    [[nodiscard]]
    bool includesLine(Line line) const;
};

inline CoordRange::CoordRange(Coord start, Coord end) : Range<Coord>(start, end) {}
inline CoordRange::CoordRange(LinePositionRange posRange, LineRange lineRange)
    : Range<Coord>(Coord(posRange.start(), lineRange.start()), Coord(posRange.end(), lineRange.end()))
{}
inline CoordRange::CoordRange() = default;

inline bool CoordRange::operator==(const CoordRange& other) const { return Range<Coord>::operator==(other); }

inline Size CoordRange::width(LinePositionSize lineWidth)   const { return lineWidth * (lines() - 1) + end().pos() - start().pos() + 1; }
inline LineSize CoordRange::lines()                         const { return end().line() - start().line() + 1; }
inline bool CoordRange::includesLine(Line line)             const { return (start().line() <= line && line <= end().line()); }

}

Q_DECLARE_TYPEINFO(Okteta::CoordRange, Q_MOVABLE_TYPE);

#endif
