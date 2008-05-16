/*
    This file is part of the Okteta Core library, part of the KDE project.

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

#ifndef KHE_KSECTION_H
#define KHE_KSECTION_H

// lib
#include "krange.h"
#include "oktetacore_export.h"


namespace KHE
{

/** describes a section of indizes
  *@author Friedrich W. H.  Kossebau
  */
class OKTETACORE_EXPORT KSection : public KRange<int>
{
  public:
    /** constructs a section by width
     * @param startIndex starting index
     * @param width width of the section
     */
    static KSection fromWidth( int startIndex, int width );
    static KSection fromWidth( int width );
  public:
    /** constructs a section
     * @param startIndex starting index
     * @param endIndex end index
     */
    // FIXME: gcc 3.4.5 on windows gives an internal_compiler_error if ctor/dtor are defined with the keyword inline
    // thus put the function bodies here
    KSection( int startIndex, int endIndex ) : KRange<int>(startIndex,endIndex) {}
    KSection( const KSection &other ) : KRange<int>(other.start(),other.end()) {}
    KSection() {}
    ~KSection() {}

  public:
    KSection &operator=( const KSection &other );

  public:
    bool operator==( const KSection &other ) const;

  public:
    void setByWidth( int other, int width );
    /** sets the first index of the section's range one behind the other's end
      * If one of both is invalid the behaviour is undefined
      */
    void setStartNextBehind( const KSection &other );
    void setStartNextBehind( int index );
    /** sets the first index of the section's range one behind the other's end
      * If one of both is invalid or the other' start is 0 the behaviour is undefined
      */
    void setEndNextBefore( const KSection &other );
    void setEndNextBefore( int index );
    /** sets the first index of the section's range to be width-1 before the end
     * If the section is invalid the behaviour is undefined
     */
    void setStartByWidth( int width );
    /** sets the last index of the section's range to be width-1 behind the start
      * If the section is invalid the behaviour is undefined
      */
    void setEndByWidth( int width );
    /** restricts the end by width. If the section is invalid the behaviour is undefined */
    void restrictEndByWidth( int width );
    /** moves the range defined by a new start.
     * If the range is invalid the behaviour is undefined
     */
    void moveToStart( int other );
    /** moves the range defined by a new end.
     * If the range is invalid the behaviour is undefined
     */
    void moveToEnd( int end );

    void adaptToReplacement( int Pos, int removedLength, int insertedLength );

    KSection splitAt( int index );
    KSection splitAtLocal( int index );
    KSection remove( const KSection &removeSection );
    KSection removeLocal( const KSection &removeSection );

    bool prepend( const KSection &other );
    bool append( const KSection &other );

  public:
    /**
     * @return the numbered of included indizes or 0, if the section is invalid
     */
    int width() const;
    int nextBeforeStart() const;
    int nextBehindEnd() const;

  public:
    /** @return index relative to the start */
    int localIndex( int index ) const;
    KSection localSection( const KSection &other ) const;
    /** @return section given by local  */
    KSection subSection( const KSection &localSection ) const;
    /**
     * @return the needed start so that other gets included, undefined if any is invalid
     */
    int startForInclude( const KSection &other ) const;
    /** @returns true if both section . If one of both is invalid the behaviour is undefined */
    bool isJoinable( const KSection &other ) const;
};

inline KSection KSection::fromWidth( int startIndex, int width ) { return KSection(startIndex,startIndex+width-1); }
inline KSection KSection::fromWidth( int width ) { return KSection(0,width-1); }

inline bool KSection::operator==( const KSection &other ) const { return KRange<int>::operator==(other); }

inline KSection &KSection::operator=( const KSection &other ) { KRange<int>::operator=(other); return *this; }

inline int KSection::width()       const { return isValid() ? end()-start()+1 : 0; }
inline int KSection::nextBeforeStart() const { return start()-1; }
inline int KSection::nextBehindEnd()   const { return end()+1; }

inline void KSection::setByWidth( int other, int width )  { setStart( other ); setEnd( other+width-1 ); }
inline void KSection::setStartByWidth( int width )  { setStart( end()-width+1 ); }
inline void KSection::setEndByWidth( int width )    { setEnd( start()+width-1 ); }
inline void KSection::setStartNextBehind( const KSection &other )  { setStart( other.nextBehindEnd() ); }
inline void KSection::setStartNextBehind( int index )  { setStart( index+1 ); }
inline void KSection::setEndNextBefore( const KSection &other )    { setEnd( other.nextBeforeStart() ); }
inline void KSection::setEndNextBefore( int index )    { setEnd( index-1 ); }
inline void KSection::restrictEndByWidth( int width ) { restrictEndTo( start()+width-1 ); }

inline void KSection::moveToStart( int other ) { setEnd( other+width()-1 ); setStart( other ); }
inline void KSection::moveToEnd( int end )   { setStart( end-width()+1 ); setEnd( end ); }

inline KSection KSection::splitAt( int index )
{
    const int secondEnd = end();
    setEndNextBefore( index );
    return KSection( index, secondEnd );
}
inline KSection KSection::splitAtLocal( int index )
{
    const int secondEnd = end();
    setEndByWidth( index );
    return KSection( nextBehindEnd(), secondEnd );
}
inline KSection KSection::remove( const KSection &removeSection )
{
    const int secondEnd = end();
    setEndNextBefore( removeSection );
    return KSection( removeSection.nextBehindEnd(), secondEnd );
}
inline KSection KSection::removeLocal( const KSection &removeSection )
{
    const int secondEnd = end();
    setEndByWidth( removeSection.start() );
    return KSection( start()+removeSection.nextBehindEnd(), secondEnd );
}

inline int KSection::localIndex( int index ) const { return index - start(); }
inline KSection KSection::localSection( const KSection &other ) const
{ return KSection( other.start()-start(), other.end()-start() ); }
inline KSection KSection::subSection( const KSection &localSection ) const
{ return KSection( localSection.start()+start(), localSection.end()+start() ); }

inline int KSection::startForInclude( const KSection &other ) const
{
  return startsBehind(other.start()) ? other.start() :
         endsBefore(other.end()) ?     other.end()-width()+1 :
         start();
}
inline bool KSection::isJoinable( const KSection &other ) const
{ return Start <= other.nextBehindEnd() && other.nextBeforeStart() <= End; }

inline bool KSection::prepend( const KSection &other )
{ const bool mergeable = ( other.nextBehindEnd() == start() ); if( mergeable ) setStart( other.start() ); return mergeable; }
inline bool KSection::append( const KSection &other )
{ const bool mergeable = ( nextBehindEnd() == other.start() ); if( mergeable ) setEnd( other.end() ); return mergeable; }

}

#endif
