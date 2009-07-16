/*
    This file is part of the Okteta Gui library, part of the KDE project.

    Copyright 2003 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef OKTETA_COORDRANGE_H
#define OKTETA_COORDRANGE_H

// lib
#include "coord.h"
// Okteta core
#include <range.h>
#include <section.h>


namespace KDE
{

typedef Range<Okteta::Coord> KBaseCoordRange;

template<>
inline const Okteta::Coord KBaseCoordRange::null() const 
{ return Okteta::Coord(-1,-1); }

}

namespace Okteta
{

/** describes a range in the buffercoord
  *@author Friedrich W. H.  Kossebau
  */
class CoordRange : public KDE::KBaseCoordRange
{
  public:
    /** 
      * @param SC start coord
      * @param EC end coord
      */
    CoordRange( const Coord &SC, const Coord &EC );
    /** 
      * @param Pos start and end pos
      * @param Lines start and end line
      */
    CoordRange( const KDE::Section &Pos, const KDE::Section &Lines );
    CoordRange();
    ~CoordRange();

  public:
    CoordRange &operator=( const CoordRange &S );

  public:
    bool operator==( const CoordRange &S ) const;

  public:
    /** calculates the number of coords that are covered if a line has the given length.
      * If the range is invalid the behaviour is undefined.
      * @param LineLength 
      * @return the number of points covered if a line has a length of LineLength.
      */
    int width( int LineLength ) const;
    /** calculates the number of lines that are covered by the range.
      * If the range is invalid the behaviour is undefined.
      * @return number of lines covered
      */
    int lines() const;
    /** tests if the given line is included by the range. 
      * If the range is invalid or the line < 0 the behaviour is undefined.      
      * @param Line index of line
      * @return @c true if Line is included, otherwise @c false
      */
    bool includesLine( int Line ) const;
};


inline CoordRange::CoordRange( const Coord &SC, const Coord &EC ) : KDE::KBaseCoordRange(SC,EC) {}
inline CoordRange::CoordRange( const KDE::Section &Pos, const KDE::Section &Lines )
 : KDE::KBaseCoordRange( Coord(Pos.start(),Lines.start()), Coord(Pos.end(),Lines.end()) ) {}
inline CoordRange::CoordRange()  {}
inline CoordRange::~CoordRange() {}

inline bool CoordRange::operator==( const CoordRange &R ) const { return  KDE::KBaseCoordRange::operator==(R); }

inline CoordRange &CoordRange::operator=( const CoordRange &R ) {  KDE::KBaseCoordRange::operator=(R); return *this; }

inline int CoordRange::width( int LineLength )   const { return LineLength*(lines()-1) + End.pos() - Start.pos()+1; }
inline int CoordRange::lines()                   const { return End.line() - Start.line() + 1; }
inline bool CoordRange::includesLine( int Line ) const { return Line >= Start.line() && Line <= End.line(); }

}

#endif
