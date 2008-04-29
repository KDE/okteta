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

#ifndef KHE_UI_COORD_H
#define KHE_UI_COORD_H


namespace KHEUI
{

/**
  * a class which represents a coord in a 2-dim. system
  *
  * It consists of a line number and a position in the line.
  * The coord starts at (0,0). Line numbers increase downwards, positions to the right.
  * With any of both at a negative number the coord is invalid.
  * TODO: change as much as possible params to unsigned integer
  * 
  * @author Friedrich W. H. Kossebau
  */
class Coord
{
  public:
    /** constructs a section by width
     * @param SI starting index
     * @param W width of the section
     */
    static Coord fromIndex( int Index, int LineWidth );
  public:
    /** creates a coord with 0,0 */
    Coord();
    Coord( int P, int L );
    Coord( const Coord &C );
    Coord &operator=( const Coord &c );
    ~Coord();


  public: // logic
    bool operator==( const Coord &c ) const;
    bool operator!=( const Coord &c ) const;
    bool operator<( const Coord &c ) const;
    bool operator<=( const Coord &c ) const;
    bool operator>( const Coord &c ) const;
    bool operator>=( const Coord &c ) const;

    /** tests if the coord is prior in the same line than the given coord.
     * If at least one of both is invalid the result is undefined.
     * @return true if the pos is left to the pos of C and both are in the same line, otherwise false.
     */
    bool isPriorInLineThan( const Coord &C ) const;
    /** tests if the coord is later in the same line than the given coord.
     * If at least one of both is invalid the result is undefined.
     * @return true if the pos is right to the pos of C and both are in the same line, otherwise false 
     */
    bool isLaterInLineThan( const Coord &C ) const;
    /** @return true if the line is below L, otherwise false */
    bool isBelow( int L ) const;
    /** @return true if the line is above L, otherwise false */
    bool isAbove( int L ) const;
    /** @return true if the coord is at (0,0) */
    bool isAtStart() const;
    /** @return true if the pos is greater than 0, otherwise false */
    bool isBehindLineStart() const;
    /** @return true if the pos is smaller than MaxPos, otherwise false */
    bool isBeforeLineEnd( int MaxPos ) const;

    /** calculates the index the coord is at with a given line width
     * If the coord is invalid the result is undefined.
     * @param LineWidth given width of line
     * @return index the coord is at 
     */
    int indexByLineWidth( int LineWidth ) const;

  public:
    /** set the coord by calculating it for an index with a given line width
     * @param Index index in the buffer
     * @param LineWidth given line width
     */
    void setByIndexNWidth( int Index, int LineWidth );
    /** sets both position and line */
    void set( int P, int L );
    /** sets the position */
    void setPos( int P );
    /** sets the line */
    void setLine( int L );

    /** moves the coord one position to the left. If the coord is invalid the result is undefined. */
    void goLeft();
    /** moves the coord a given number of positions to the left. 
     * If the coord is invalid the result is undefined or the position smaller then the given number 
     * the behaviour is undefined.
     * @param P number of positions
     */
    void goLeft( unsigned int P );
    /** moves the coord one position to the left, or if the position is already at the line start
     * to the given position in the previous line. If the coord is invalid the result is undefined. 
     * @param MaxPos maximal allowed position
     */
    void goCLeft( int MaxPos );
    /** moves the coord one position to the right. If the coord is invalid the result is undefined. */
    void goRight();
    /** moves the coord a given number of positions to the right. If the coord is invalid the result is undefined.
     * @param P number of positions
     */
    void goRight( unsigned int P );
    /** moves the coord one position to the right, or if the position has already reached or passed MaxPos 
     * to the first position in the next line. If the coord is invalid the result is undefined. 
     * @param MaxPos maximal allowed position
     */
    void goCRight( int MaxPos );
    /** sets coord to (0,0) */
    void gotoStart();
    void gotoEndOfPreviousLine( int LastPos );
    /** sets the coord to the start of the next line. 
      * If the coord is invalid the behaviour is undefined.
      */
    void gotoStartOfNextLine();
    /** sets the position to the start of the line or 
      * if the line is the same as that of the given coord to the position of it. 
      * If one or more of the coords is invalid the behaviour is undefined.
      * @param C a possible line start coord
      */
    void goLineStart( const Coord &C );
    /** sets the position to the given pos or 
      * if the line is the same as that of the given coord to the position of that. 
      * If one or more of the coords is invalid the behaviour is undefined.
      * @param L last position in normal line
      * @param C a possible line end coord
      */
    void goLineEnd( int L, const Coord &C );
    /** moves the coord 1 lines upwards. There is no check whether the first line is overstepped. */
    void goUp();
    /** moves the coord L lines downwards. */
    void goDown();
    /** moves the coord L lines upwards. There is no check whether the first line is overstepped.
     * @param L number of lines
     */
    void goUp( unsigned int L );
    /** moves the coord L lines downwards. 
     * @param L number of lines
     */
    void goDown( unsigned int L );

  public: // state value access
    /** @return the pos in the line */
    int pos() const;
    /** @return the line number */
    int line() const;
    /** @return true if the coord is valid */
    bool isValid() const;

  private: // member variables
    /** Position in Line */
    int Pos;
    /** Line */
    int Line;
};


inline Coord::Coord() : Pos( 0 ), Line( 0 ) {}
inline Coord::Coord( int P, int L ) : Pos( P ), Line( L ) {}

inline Coord Coord::fromIndex( int Index, int LineWidth )
{
  int Line = Index / LineWidth;
  int Pos  = Index - Line*LineWidth;
  return Coord( Pos, Line );
}

inline Coord::Coord( const Coord &C ) : Pos( C.Pos ), Line( C.Line ) {}
inline Coord &Coord::operator=( const Coord &C ) { Pos = C.Pos; Line = C.Line; return *this; }
inline Coord::~Coord() {}

inline bool Coord::operator==( const Coord &C ) const { return Pos == C.Pos && Line == C.Line; }
inline bool Coord::operator!=( const Coord &C ) const { return !(*this == C); }

inline bool Coord::operator<( const Coord &C ) const
{ return Line < C.Line || (Line == C.Line && Pos<C.Pos); }
inline bool Coord::operator<=( const Coord &C ) const
{ return Line < C.Line || (Line == C.Line && Pos<=C.Pos); }
inline bool Coord::operator>( const Coord &C ) const
{ return Line > C.Line || (Line == C.Line && Pos>C.Pos); }
inline bool Coord::operator>=( const Coord &C ) const
{ return Line > C.Line || (Line == C.Line && Pos>=C.Pos); }

inline int  Coord::pos()         const { return Pos; }
inline int  Coord::line()        const { return Line; }
inline bool Coord::isValid()     const { return Line >= 0 && Pos >= 0; }

inline void Coord::setByIndexNWidth( int Index, int LineWidth )
{
  Line = Index / LineWidth;
  Pos  = Index - Line*LineWidth;
}

inline void Coord::set( int P, int L )
{
  Pos  = P;
  Line = L;
}
inline void Coord::setPos( int P )  { Pos  = P; }
inline void Coord::setLine( int L ) { Line = L; }

inline void Coord::goCRight( int MaxPos )
{
  if( isBeforeLineEnd(MaxPos) )
    goRight();
  else
    gotoStartOfNextLine();
}
inline void Coord::goCLeft( int MaxPos )
{
  if( isBehindLineStart() )
    goLeft();
  else
    gotoEndOfPreviousLine( MaxPos );
}

inline void Coord::goRight() { ++Pos; }
inline void Coord::goLeft()  { --Pos; }
inline void Coord::goRight( unsigned int P ) { Pos += P; }
inline void Coord::goLeft( unsigned int P )  { Pos -= P; }

inline void Coord::gotoStart() { Pos = Line = 0; }

inline void Coord::gotoEndOfPreviousLine( int LastPos )
{
  --Line;
  Pos = LastPos;
}

inline void Coord::gotoStartOfNextLine()
{
  ++Line;
  Pos = 0;
}


inline void Coord::goLineStart( const Coord &C )
{
  Pos = ( Line == C.Line ) ? C.Pos : 0;
}

inline void Coord::goLineEnd( int L, const Coord &C )
{
  Pos = ( Line == C.Line ) ? C.Pos : L;
}

inline void Coord::goUp()           { --Line; }
inline void Coord::goDown()         { ++Line; }
inline void Coord::goUp( unsigned int L )    { Line -= L; }
inline void Coord::goDown( unsigned int L )  { Line += L; }


inline int Coord::indexByLineWidth( int LineWidth ) const
{
  return Line * LineWidth + Pos;
}


inline bool Coord::isPriorInLineThan( const Coord &C ) const
{
  return Line == C.Line && Pos < C.Pos;
}

inline bool Coord::isLaterInLineThan( const Coord &C ) const
{
  return Line == C.Line && Pos > C.Pos;
}

inline bool Coord::isBelow( int L ) const { return Line > L; }
inline bool Coord::isAbove( int L ) const { return Line < L; }

inline bool Coord::isBehindLineStart()           const { return Pos > 0; }
inline bool Coord::isBeforeLineEnd( int MaxPos ) const { return Pos < MaxPos; }

inline bool Coord::isAtStart()                   const { return Pos == 0 && Line == 0; }

inline Coord operator+( const Coord &C, int p )
{
  return Coord( C.pos()+p, C.line() );
}

}

#endif
