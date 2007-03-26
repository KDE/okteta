/***************************************************************************
                          ksection.h  -  description
                             -------------------
    begin                : Sun 22.06.2003
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


#ifndef KHE_KSECTION_H
#define KHE_KSECTION_H

// lib specific
#include "krange.h"
#include "khe_export.h"


namespace KHE
{

/** describes a section of indizes
  *@author Friedrich W. H.  Kossebau
  */
class KHECORE_EXPORT KSection : public KRange<int>
{
  public:
    /** constructs a section by width
     * @param SI starting index
     * @param W width of the section
     */
    static KSection fromWidth( int SI, int W );
  public:
    /** constructs a section
     * @param SI starting index
     * @param EI end index
     */
    KSection( int SI, int EI );
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
    /** restricts the end by width. If the section is invalid the behaviour is undefined */
    void restrictEndByWidth( int Width );
    /** moves the range defined by a new start.
     * If the range is invalid the behaviour is undefined
     */
    void moveToStart( int S );
    /** moves the range defined by a new end.
     * If the range is invalid the behaviour is undefined
     */
    void moveToEnd( int E );

    void adaptToChange( int Pos, int RemovedLength, int InsertedLength );

  public:
    /**
     * @return the numbered of included indizes or 0, if the section is invalid
     */
    int width() const;
    int beforeStart() const;
    int behindEnd() const;

  public:
    /**
     * @return the needed start so that S gets included, undefined if any is invalid
     */
    int startForInclude( const KSection &S ) const;
    /** @returns true if both section . If one of both is invalid the behaviour is undefined */
    bool isJoinable( const KSection &S ) const;

};

inline KSection KSection::fromWidth( int SI, int W ) { return KSection(SI,SI+W-1); }

inline KSection::KSection( int SI, int EI ) : KRange<int>(SI,EI) {}
inline KSection::KSection()  {}
inline KSection::~KSection() {}

inline bool KSection::operator==( const KSection &S ) const { return KRange<int>::operator==(S); }

inline KSection &KSection::operator=( const KSection &S ) { KRange<int>::operator=(S); return *this; }

inline int KSection::width()       const { return isValid() ? end()-start()+1 : 0; }
inline int KSection::beforeStart() const { return start()-1; }
inline int KSection::behindEnd()   const { return end()+1; }

inline void KSection::setByWidth( int S, int Width )  { setStart( S ); setEnd( S+Width-1 ); }
inline void KSection::setStartByWidth( int Width )  { setStart( end()-Width+1 ); }
inline void KSection::setEndByWidth( int Width )    { setEnd( start()+Width-1 ); }
inline void KSection::setStartBehind( KSection S )  { setStart( S.end()+1 ); }
inline void KSection::setEndBefore( KSection S )    { setEnd( S.start()-1 ); }
inline void KSection::restrictEndByWidth( int Width ) { restrictEndTo( start()+Width-1 ); }

inline void KSection::moveToStart( int S ) { setEnd( S+width()-1 ); setStart( S ); }
inline void KSection::moveToEnd( int E )   { setStart( E-width()+1 ); setEnd( E ); }

inline int KSection::startForInclude( const KSection &S ) const
{
  return startsBehind(S.start()) ? S.start() :
         endsBefore(S.end()) ?     S.end()-width()+1 :
         start();
}
inline bool KSection::isJoinable( const KSection &S ) const { return Start <= S.end()+1 && S.start()-1 <= End; }

}

#endif
