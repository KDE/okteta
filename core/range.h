/*
    This file is part of the Okteta Core library, made within the KDE community.

    Copyright 2003,2007-2008 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef KDE_RANGE_H
#define KDE_RANGE_H


namespace KDE
{

/** This template describes a range.
  * A range is something with a start and an end.
  * The start is a value relative before the end.
  * The distance cannot be estimated.
  *
  *@author Friedrich W. H.  Kossebau
  */
template<typename T>
class Range
{
  public:
    Range( T S, T E ) : Start( S ), End( E )  {}
    Range() : Start( null() ), End( null() )  {}
    ~Range() {}

  public:
    Range &operator=( const Range &R ) { Start = R.Start; End = R.End; return *this; }

  public:
    bool operator==( const Range &R ) const
         { return (Start == R.Start && End == R.End) || (!isValid() && !R.isValid()); }

  public: // modification access
    /** sets the first and the last index of the range */
    void set( T S, T E ) { Start = S; End = E; }
    /** sets the first and the last index of the range */
    void set( const Range &R ) { Start = R.Start; End = R.End; }
    /** sets the first index of the range */
    void setStart( T S )  { Start = S; }
    /** sets the last index of the range */
    void setEnd( T E )    { End = E; }
    /** sets the range to null */
    void unset()           { Start = End = null(); }
    /** restricts the range to Limit. If one of both ranges is invalid the behaviour is undefined */
    void restrictTo( const Range &Limit )
    { if( Start < Limit.start() ) Start = Limit.start(); if( End > Limit.end() ) End = Limit.end(); }
    /** restricts the start to Limit. If the range is invalid the behaviour is undefined */
    void restrictStartTo( T Limit )  { if( Start < Limit ) Start = Limit; }
    /** restricts the end to Limit. If the range is invalid the behaviour is undefined */
    void restrictEndTo( T Limit )    { if( End > Limit ) End = Limit; }
    /** extends the range to Limit. If one of both is invalid the behaviour is undefined */
    void extendTo( const Range &Limit )
    { if( Start > Limit.start() ) Start = Limit.start(); if( End < Limit.end() ) End = Limit.end(); }
    /** extends the start to Limit. If the range is invalid the behaviour is undefined */
    void extendStartTo( T Limit )  { if( Start > Limit ) Start = Limit; }
    /** extends the end to Limit. If the range is invalid the behaviour is undefined */
    void extendEndTo( T Limit )    { if( End < Limit ) End = Limit; }
    /** moves the start by D. If the range is invalid the behaviour is undefined */
    void moveStartBy( T D )  { Start += D; }
    /** moves the end by D. If the range is invalid the behaviour is undefined */
    void moveEndBy( T D )    { End += D; }
    /** moves the range by D. If the range is invalid the behaviour is undefined */
    void moveBy( T D )      { Start += D; End += D; }

  public: // value access
    /** @return start */
    T start() const { return Start; }
    /** @return end */
    T end() const   { return End; }


  public: // logic access
    /** returns true if Value is covered */
    bool includes( T Value )     const { return Start <= Value && Value <= End; }
    /** returns true if Value is covered and not at a side */
    bool includesInside( T Value )     const { return Start < Value && Value < End; }
    /** returns true if range is behind index. if range is invalid the behaviour is undefined */
    bool startsBehind( T Value ) const { return Value < Start; }
    /** returns true is the range starts before index. If the range is invalid the behaviour is undefined */
    bool startsBefore( T Value ) const { return Start < Value; }
    /** returns true if the range ends later than index. If the range is invalid the behaviour is undefined */
    bool endsBehind( T Value )   const { return Value < End; }
    /** returns true if range is before index. if range is invalid the behaviour is undefined */
    bool endsBefore( T Value )   const { return End < Value; }

    /** returns true is the range covers R. If one of both is invalid the behaviour is undefined */
    bool includes( const Range &R )     const { return R.End <= End && Start <= R.Start; }
    /** returns true is the range covers R. If one of both is invalid the behaviour is undefined */
    bool includesInside( const Range &R ) const { return R.End < End && Start < R.Start; }
    /** returns true is the range ends before R starts. If one of both is invalid the behaviour is undefined */
    bool endsBefore( const Range &R )   const { return End < R.Start; }
    /** returns true is the range starts later than R ends. If one of both is invalid the behaviour is undefined */
    bool startsBehind( const Range &R ) const { return R.End < Start; }
    /** returns true is the range starts prior than R. If one of both is invalid the behaviour is undefined */
    bool startsBefore( const Range &R ) const { return Start < R.Start; }
    /** returns true is the range ends later than R. If one of both is invalid the behaviour is undefined */
    bool endsBehind( const Range &R )   const { return R.End < End; }
    /** returns true is the range shares at least one index with R. If one of both is invalid the behaviour is undefined */
    bool overlaps( const Range &R ) const { return Start <= R.End && R.Start <= End; }

    // TODO: this is wrong, a empty range is valid, too
    /** returns true if the range covers at least one index */
    bool isValid() const { return Start != null() && Start <= End; }
    /** returns true if the range has not been set */
    bool isEmpty() const { return Start == null() && End == null(); }


  protected:
    /** delivers a null element. Should be specialiced for complexer types. */
    const T null () const { return T(-1);}

  protected:
    /** first value of the range */
    T Start;
    /** last value of the range */
    T End;
};

}

#endif
