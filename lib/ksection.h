/***************************************************************************
                          ksection.h  -  description
                             -------------------
    begin                : Sun 22.06.2003
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


#ifndef KHE_KSECTION_H
#define KHE_KSECTION_H


#include "krange.h"

namespace KHE
{

/** describes a section of indizes
  *@author Friedrich W. H.  Kossebau
  */
class KSection : public KRange<int>
{
  public:
    /** constructs a section
     * @param SI starting index
     * @param EI end index
     */
    KSection( int SI, int EI );
    /** constructs a section
     * @param SI starting index
     * @param W width of the section
     */
    KSection( int SI, int W, bool );
    KSection();
    ~KSection();

  public:
    KSection &operator=( const KSection &S );

  public:
    bool operator==( const KSection &S ) const;

  public:
    void setByWidth( int S, int Width );
    /** sets the first index of the section's range one behind the other's end
      * If one of both is invalid the behaviour is undefined
      */
    void setStartBehind( KSection S );
    /** sets the first index of the section's range one behind the other's end
      * If one of both is invalid or the other' start is 0 the behaviour is undefined
      */
    void setEndBefore( KSection S );
    /** sets the first index of the section's range to be width-1 before the end
     * If the section is invalid the behaviour is undefined
     */
    void setStartByWidth( int Width );
    /** sets the last index of the section's range to be width-1 behind the start
      * If the section is invalid the behaviour is undefined
      */
    void setEndByWidth( int Width );
    /** moves the range defined by a new start.
     * If the range is invalid the behaviour is undefined
     */
    void moveToStart( int S );
    /** moves the range defined by a new start.
     * If the range is invalid the behaviour is undefined
     */
    void moveToEnd( int E );

  public:
    /**
     * @return the numbered of included indizes or 0, if the section is invalid
     */
    int width() const;
};


inline KSection::KSection( int SI, int EI ) : KRange<int>(SI,EI) {}
inline KSection::KSection( int SI, int W, bool ) : KRange<int>(SI,SI+W-1) {}
inline KSection::KSection()  {}
inline KSection::~KSection() {}

inline bool KSection::operator==( const KSection &S ) const { return KRange<int>::operator==(S); }

inline KSection &KSection::operator=( const KSection &S ) { KRange<int>::operator=(S); return *this; }

inline int KSection::width() const { return isValid() ? end()-start()+1 : 0; }

inline void KSection::setByWidth( int S, int Width )  { setStart( S ); setEnd( S+Width-1 ); }
inline void KSection::setStartByWidth( int Width )  { setStart( end()-Width+1 ); }
inline void KSection::setEndByWidth( int Width )    { setEnd( start()+Width-1 ); }
inline void KSection::setStartBehind( KSection S )  { setStart( S.end()+1 ); }
inline void KSection::setEndBefore( KSection S )    { setEnd( S.start()-1 ); }

inline void KSection::moveToStart( int S ) { setEnd( S+width()-1 ); setStart( S ); }
inline void KSection::moveToEnd( int E )   { setStart( E-width()+1 ); setEnd( E ); }
}

#endif
