/***************************************************************************
                          kbuffercoord.h  -  description
                             -------------------
    begin                : Don Mai 29 2003
    copyright            : (C) 2003 by Friedrich W. H. Kossebau
    email                : Friedrich.W.H@Kossebau.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License version 2 as published by the Free Software Foundation.       *
 *                                                                         *
 ***************************************************************************/


#ifndef KBUFFERCOORD_H
#define KBUFFERCOORD_H


namespace KHE
{

/**
  *@author Friedrich W. H. Kossebau
  */
class KBufferCoord
{
  public:
    KBufferCoord();
    KBufferCoord( int P, int L );
    KBufferCoord( int P, int L, bool /*dummy*/ );
    KBufferCoord( const KBufferCoord &C );
    KBufferCoord &operator=( const KBufferCoord &c );
    ~KBufferCoord();


  public:
    bool operator==( const KBufferCoord &c ) const;
    bool operator!=( const KBufferCoord &c ) const;
    bool operator<( const KBufferCoord &c ) const;
    bool operator<=( const KBufferCoord &c ) const;
    bool operator>( const KBufferCoord &c ) const;
    bool operator>=( const KBufferCoord &c ) const;
    bool isPriorInLineThan( const KBufferCoord &C ) const;
    bool isLaterInLineThan( const KBufferCoord &C ) const;
    bool isBelow( int L ) const;
    bool isAbove( int L ) const;

    int indexByLineWidth( int LineWidth ) const;

  public:
    void setByIndexNWidth( int Index, int LineWidth );
    void set( int P, int L );
    void setPos( int P );
    void setLine( int L );
    bool goLeft();
    bool goRight( int MaxPos );
    void goRight();
    void gotoEndOfPreviousLine( int LastPos );
    void gotoStartOfNextLine();
    void goLineStart( const KBufferCoord &C );
    void goLineEnd( int L, const KBufferCoord &C );
    void goUp();
    void goDown();
    void goUp( int L );
    void goDown( int L );

  public: // state value access
    int pos() const;
    int line() const;

  private:
    /** Position in Line */
    int Pos;
    /** Line */
    int Line;
};


inline KBufferCoord::KBufferCoord() : Pos( 0 ), Line( 0 ) {}
inline KBufferCoord::KBufferCoord( int P, int L ) : Pos( P ), Line( L ) {}
inline KBufferCoord::KBufferCoord( int Index, int LineWidth, bool )
{
  Line = Index / LineWidth;
  Pos  = Index - Line*LineWidth;
}

inline KBufferCoord::KBufferCoord( const KBufferCoord &C ) : Pos( C.Pos ), Line( C.Line ) {}
inline KBufferCoord &KBufferCoord::operator=( const KBufferCoord &C ) { Pos = C.Pos; Line = C.Line; return *this; }
inline KBufferCoord::~KBufferCoord() {}

inline bool KBufferCoord::operator==( const KBufferCoord &C ) const { return Pos == C.Pos && Line == C.Line; }
inline bool KBufferCoord::operator!=( const KBufferCoord &C ) const { return !(*this == C); }

inline bool KBufferCoord::operator<( const KBufferCoord &C ) const
{ return Line < C.Line || (Line == C.Line && Pos<C.Pos); }
inline bool KBufferCoord::operator<=( const KBufferCoord &C ) const
{ return Line < C.Line || (Line == C.Line && Pos<=C.Pos); }
inline bool KBufferCoord::operator>( const KBufferCoord &C ) const
{ return Line > C.Line || (Line == C.Line && Pos>C.Pos); }
inline bool KBufferCoord::operator>=( const KBufferCoord &C ) const
{ return Line > C.Line || (Line == C.Line && Pos>=C.Pos); }

inline int KBufferCoord::pos()         const { return Pos; }
inline int KBufferCoord::line()        const { return Line; }

inline void KBufferCoord::setByIndexNWidth( int Index, int LineWidth )
{
  Line = Index / LineWidth;
  Pos  = Index - Line*LineWidth;
}

inline void KBufferCoord::set( int P, int L )
{
  Pos  = P;
  Line = L;
}
inline void KBufferCoord::setPos( int P )  { Pos  = P; }
inline void KBufferCoord::setLine( int L ) { Line = L; }

inline bool KBufferCoord::goLeft() { if( Pos > 0 ) { --Pos; return true; } else return false; }
inline bool KBufferCoord::goRight( int MaxPos ) { if( Pos < MaxPos ) { ++Pos; return true; } else return false; }
inline void KBufferCoord::goRight() { ++Pos; }

inline void KBufferCoord::gotoEndOfPreviousLine( int LastPos )
{
  --Line;
  Pos = LastPos;
}

inline void KBufferCoord::gotoStartOfNextLine()
{
  ++Line;
  Pos = 0;
}


inline void KBufferCoord::goLineStart( const KBufferCoord &C )
{
  Pos = ( Line == C.Line ) ? C.Pos : 0;
}

inline void KBufferCoord::goLineEnd( int L, const KBufferCoord &C )
{
  Pos = ( Line == C.Line ) ? C.Pos : L;
}

inline void KBufferCoord::goUp()           { --Line; }
inline void KBufferCoord::goDown()         { ++Line; }
inline void KBufferCoord::goUp( int L )    { Line -= L; }
inline void KBufferCoord::goDown( int L )  { Line += L; }


inline int KBufferCoord::indexByLineWidth( int LineWidth ) const
{
  return Line * LineWidth + Pos;
}


inline bool KBufferCoord::isPriorInLineThan( const KBufferCoord &C ) const
{
  return Line == C.Line && Pos < C.Pos;
}

inline bool KBufferCoord::isLaterInLineThan( const KBufferCoord &C ) const
{
  return Line == C.Line && Pos > C.Pos;
}

inline bool KBufferCoord::isBelow( int L ) const
{
  return Line > L;
}

inline bool KBufferCoord::isAbove( int L ) const
{
  return Line < L;
}


inline KBufferCoord operator+( const KBufferCoord &C, int p )
{
  return KBufferCoord( C.pos()+p, C.line() );
}

}

#endif
