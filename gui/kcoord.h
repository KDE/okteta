/***************************************************************************
                          kcoord.h  -  description
                             -------------------
    begin                : Don Mai 29 2003
    copyright            : (C) 2003 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License version 2 as published by the Free Software Foundation.       *
 *                                                                         *
 ***************************************************************************/


#ifndef KHE_UI_KDATACOORD_H
#define KHE_UI_KDATACOORD_H


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
class KCoord
{
  public:
    /** constructs a section by width
     * @param SI starting index
     * @param W width of the section
     */
    static KCoord fromIndex( int Index, int LineWidth );
  public:
    /** creates a coord with 0,0 */
    KCoord();
    KCoord( int P, int L );
    KCoord( const KCoord &C );
    KCoord &operator=( const KCoord &c );
    ~KCoord();


  public: // logic
    bool operator==( const KCoord &c ) const;
    bool operator!=( const KCoord &c ) const;
    bool operator<( const KCoord &c ) const;
    bool operator<=( const KCoord &c ) const;
    bool operator>( const KCoord &c ) const;
    bool operator>=( const KCoord &c ) const;

    /** tests if the coord is prior in the same line than the given coord.
     * If at least one of both is invalid the result is undefined.
     * @return true if the pos is left to the pos of C and both are in the same line, otherwise false.
     */
    bool isPriorInLineThan( const KCoord &C ) const;
    /** tests if the coord is later in the same line than the given coord.
     * If at least one of both is invalid the result is undefined.
     * @return true if the pos is right to the pos of C and both are in the same line, otherwise false 
     */
    bool isLaterInLineThan( const KCoord &C ) const;
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
    void goLineStart( const KCoord &C );
    /** sets the position to the given pos or 
      * if the line is the same as that of the given coord to the position of that. 
      * If one or more of the coords is invalid the behaviour is undefined.
      * @param L last position in normal line
      * @param C a possible line end coord
      */
    void goLineEnd( int L, const KCoord &C );
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


inline KCoord::KCoord() : Pos( 0 ), Line( 0 ) {}
inline KCoord::KCoord( int P, int L ) : Pos( P ), Line( L ) {}

inline KCoord KCoord::fromIndex( int Index, int LineWidth )
{
  int Line = Index / LineWidth;
  int Pos  = Index - Line*LineWidth;
  return KCoord( Pos, Line );
}

inline KCoord::KCoord( const KCoord &C ) : Pos( C.Pos ), Line( C.Line ) {}
inline KCoord &KCoord::operator=( const KCoord &C ) { Pos = C.Pos; Line = C.Line; return *this; }
inline KCoord::~KCoord() {}

inline bool KCoord::operator==( const KCoord &C ) const { return Pos == C.Pos && Line == C.Line; }
inline bool KCoord::operator!=( const KCoord &C ) const { return !(*this == C); }

inline bool KCoord::operator<( const KCoord &C ) const
{ return Line < C.Line || (Line == C.Line && Pos<C.Pos); }
inline bool KCoord::operator<=( const KCoord &C ) const
{ return Line < C.Line || (Line == C.Line && Pos<=C.Pos); }
inline bool KCoord::operator>( const KCoord &C ) const
{ return Line > C.Line || (Line == C.Line && Pos>C.Pos); }
inline bool KCoord::operator>=( const KCoord &C ) const
{ return Line > C.Line || (Line == C.Line && Pos>=C.Pos); }

inline int  KCoord::pos()         const { return Pos; }
inline int  KCoord::line()        const { return Line; }
inline bool KCoord::isValid()     const { return Line >= 0 && Pos >= 0; }

inline void KCoord::setByIndexNWidth( int Index, int LineWidth )
{
  Line = Index / LineWidth;
  Pos  = Index - Line*LineWidth;
}

inline void KCoord::set( int P, int L )
{
  Pos  = P;
  Line = L;
}
inline void KCoord::setPos( int P )  { Pos  = P; }
inline void KCoord::setLine( int L ) { Line = L; }

inline void KCoord::goCRight( int MaxPos )
{
  if( isBeforeLineEnd(MaxPos) )
    goRight();
  else
    gotoStartOfNextLine();
}
inline void KCoord::goCLeft( int MaxPos )
{
  if( isBehindLineStart() )
    goLeft();
  else
    gotoEndOfPreviousLine( MaxPos );
}

inline void KCoord::goRight() { ++Pos; }
inline void KCoord::goLeft()  { --Pos; }
inline void KCoord::goRight( unsigned int P ) { Pos += P; }
inline void KCoord::goLeft( unsigned int P )  { Pos -= P; }

inline void KCoord::gotoStart() { Pos = Line = 0; }

inline void KCoord::gotoEndOfPreviousLine( int LastPos )
{
  --Line;
  Pos = LastPos;
}

inline void KCoord::gotoStartOfNextLine()
{
  ++Line;
  Pos = 0;
}


inline void KCoord::goLineStart( const KCoord &C )
{
  Pos = ( Line == C.Line ) ? C.Pos : 0;
}

inline void KCoord::goLineEnd( int L, const KCoord &C )
{
  Pos = ( Line == C.Line ) ? C.Pos : L;
}

inline void KCoord::goUp()           { --Line; }
inline void KCoord::goDown()         { ++Line; }
inline void KCoord::goUp( unsigned int L )    { Line -= L; }
inline void KCoord::goDown( unsigned int L )  { Line += L; }


inline int KCoord::indexByLineWidth( int LineWidth ) const
{
  return Line * LineWidth + Pos;
}


inline bool KCoord::isPriorInLineThan( const KCoord &C ) const
{
  return Line == C.Line && Pos < C.Pos;
}

inline bool KCoord::isLaterInLineThan( const KCoord &C ) const
{
  return Line == C.Line && Pos > C.Pos;
}

inline bool KCoord::isBelow( int L ) const { return Line > L; }
inline bool KCoord::isAbove( int L ) const { return Line < L; }

inline bool KCoord::isBehindLineStart()           const { return Pos > 0; }
inline bool KCoord::isBeforeLineEnd( int MaxPos ) const { return Pos < MaxPos; }

inline bool KCoord::isAtStart()                   const { return Pos == 0 && Line == 0; }

inline KCoord operator+( const KCoord &C, int p )
{
  return KCoord( C.pos()+p, C.line() );
}

}

#endif
