/***************************************************************************
                          krange.h  -  description
                             -------------------
    begin                : Sun 03.08.2003
    copyright            : (C) 2003 by Friedrich W. H. Kossebau
    email                : Friedrich.W.H@Kossebau.de
****************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License version 2 as published by the Free Software Foundation.       *
 *                                                                         *
 ***************************************************************************/


#ifndef KRANGE_H
#define KRANGE_H


namespace KHE
{

/** This template describes a range.
  * A range is something with a start and an end.
  * The start is a value relative before the end.
  * The distance cannot be estimated.
  *
  *@author Friedrich W. H.  Kossebau
  */
template<class T>
class KRange
{
  public:
    KRange( T S, T E ) : Start( S ), End( E )  {}
    KRange() : Start( null() ), End( null() )  {}
    ~KRange() {}

  public:
    KRange &operator=( const KRange &R ) { Start = R.Start; End = R.End; return *this; }

  public:
    bool operator==( const KRange &R ) const { return Start == R.Start && End == R.End; }

  public: // modification access
    /** sets the first and the last index of the section's range */
    void set( T S, T E ) { Start = S; End = E; }
    /** sets the first index of the section's range */
    void setStart( T S )  { Start = S; }
    /** sets the last index of the section's range */
    void setEnd( T E )    { End = E; }
    /** sets the section to null */
    void unset()           { Start = End = null(); }
    /** sets the first index of the section's range */
    void setStartByWidth( T W )  { Start = End-W+1; }
    /** sets the last index of the section's range */
    void setEndByWidth( T W )    { End = Start+W-1; }
    ///** translate the section by offset. If the section is invalid the behaviour is undefined */
    //void translate( T Offset ) { Start += Offset; End += Offset; }
    /** restricts the section to Limit. If one of both is invalid the behaviour is undefined */
    void restrictTo( const KRange &Limit )
    { if( Start < Limit.start() ) Start = Limit.start(); if( End > Limit.end() ) End = Limit.end(); }
    /** restricts the section's start to Limit. If the section is invalid the behaviour is undefined */
    void restrictStartTo( T Limit )  { if( Start < Limit ) Start = Limit; }
    /** restricts the section's end to Limit. If the section is invalid the behaviour is undefined */
    void restrictEndTo( T Limit )    { if( End > Limit ) End = Limit; }
    /** extends the section to Limit. If one of both is invalid the behaviour is undefined */
    void extendTo( const KRange &Limit )
    { if( Start > Limit.start() ) Start = Limit.start(); if( End < Limit.end() ) End = Limit.end(); }
    /** extends the section's start to Limit. If the section is invalid the behaviour is undefined */
    void extendStartTo( T Limit )  { if( Start > Limit ) Start = Limit; }
    /** extends the section's end to Limit. If the section is invalid the behaviour is undefined */
    void extendEndTo( T Limit )    { if( End < Limit ) End = Limit; }

  public: // value access
    T start() const { return Start; }
    T end() const   { return End; }


  public: // logic access
    /** returns true if Value is covered */
    bool includes( T Value )     const { return Value <= End && Value >= Start; }
    /** returns true if section is before index. if section is invalid the behaviour is undefined */
    bool endsBefore( T Value )   const { return End < Value; }
    /** returns true if section is behind index. if section is invalid the behaviour is undefined */
    bool startsBehind( T Value ) const { return Start > Value; }
    /** returns true is the section starts before index. If the section is invalid the behaviour is undefined */
    bool startsBefore( T Value ) const { return Start < Value; }
    /** returns true is the section end later then index. If the section is invalid the behaviour is undefined */
    bool endsBehind( T Value )   const { return End > Value; }

    /** returns true is the section covers R. If one of both is invalid the behaviour is undefined */
    bool includes( const KRange &R )     const { return End >= R.End && Start <= R.Start; }
    /** returns true is the section ends before R starts. If one of both is invalid the behaviour is undefined */
    bool endsBefore( const KRange &R )   const { return End < R.Start; }
    /** returns true is the section starts later than R ends. If one of both is invalid the behaviour is undefined */
    bool startsBehind( const KRange &R ) const { return Start > R.End; }
    /** returns true is the section starts prior than R. If one of both is invalid the behaviour is undefined */
    bool startsBefore( const KRange &R ) const { return Start < R.Start; }
    /** returns true is the section ends later than R. If one of both is invalid the behaviour is undefined */
    bool endsBehind( const KRange &R )   const { return End > R.End; }
    /** returns true is the section shares at least one index with R. If one of both is invalid the behaviour is undefined */
    bool overlaps( const KRange &R ) const { return Start <= R.End && End >= R.Start; }

    /** returns true if the section covers at least one index */
    bool isValid() const { return Start != null() && Start <= End; }
    /** returns true if the section has not been set */
    bool isNull() const { return Start == null() && End == null(); }


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
