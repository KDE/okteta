/***************************************************************************
                          kcoordrange.h  -  description
                             -------------------
    begin                : Sun 03.08.2003
    copyright            : (C) 2003 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
****************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License version 2 as published by the Free Software Foundation.       *
 *                                                                         *
 ***************************************************************************/


#ifndef KHE_UI_KCOORDRANGE_H
#define KHE_UI_KCOORDRANGE_H

// lib
#include "kcoord.h"
// Okteta core
#include <krange.h>
#include <ksection.h>


namespace KHE
{

typedef KRange<KHEUI::KCoord> KBaseCoordRange;

template<>
inline const KHEUI::KCoord KBaseCoordRange::null() const 
{ return KHEUI::KCoord(-1,-1); }

}

namespace KHEUI
{

/** describes a range in the buffercoord
  *@author Friedrich W. H.  Kossebau
  */
class KCoordRange : public KHE::KBaseCoordRange
{
  public:
    /** 
      * @param SC start coord
      * @param EC end coord
      */
    KCoordRange( const KCoord &SC, const KCoord &EC );
    /** 
      * @param Pos start and end pos
      * @param Lines start and end line
      */
    KCoordRange( const KHE::KSection &Pos, const KHE::KSection &Lines );
    KCoordRange();
    ~KCoordRange();

  public:
    KCoordRange &operator=( const KCoordRange &S );

  public:
    bool operator==( const KCoordRange &S ) const;

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


inline KCoordRange::KCoordRange( const KCoord &SC, const KCoord &EC ) : KHE::KBaseCoordRange(SC,EC) {}
inline KCoordRange::KCoordRange( const KHE::KSection &Pos, const KHE::KSection &Lines )
 : KHE::KBaseCoordRange( KCoord(Pos.start(),Lines.start()), KCoord(Pos.end(),Lines.end()) ) {}
inline KCoordRange::KCoordRange()  {}
inline KCoordRange::~KCoordRange() {}

inline bool KCoordRange::operator==( const KCoordRange &R ) const { return  KHE::KBaseCoordRange::operator==(R); }

inline KCoordRange &KCoordRange::operator=( const KCoordRange &R ) {  KHE::KBaseCoordRange::operator=(R); return *this; }

inline int KCoordRange::width( int LineLength )   const { return LineLength*(lines()-1) + End.pos() - Start.pos()+1; }
inline int KCoordRange::lines()                   const { return End.line() - Start.line() + 1; }
inline bool KCoordRange::includesLine( int Line ) const { return Line >= Start.line() && Line <= End.line(); }

}

#endif
