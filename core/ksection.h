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
    KSection( int startIndex, int endIndex );
    KSection( const KSection &other );
    KSection();
    ~KSection();

  public:
    KSection &operator=( const KSection &other );

  public:
    bool operator==( const KSection &other ) const;

  public:
    void setByWidth( int other, int width );
    // TODO: the following functions do not match the ones from KRange exactly
    // better use RightBefore/Behind or ExactlyBefore/Behind or else 
    // alignAfter, alignBefore keep width, alignStartBehind, alignEndBefore perhaps
    /** sets the first index of the section's range one behind the other's end
      * If one of both is invalid the behaviour is undefined
      */
    void setStartBehind( const KSection &other );
    void setStartBehind( int index );
    /** sets the first index of the section's range one behind the other's end
      * If one of both is invalid or the other' start is 0 the behaviour is undefined
      */
    void setEndBefore( const KSection &other );
    void setEndBefore( int index );
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
    int beforeStart() const;
    int behindEnd() const;

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

inline KSection::KSection( int startIndex, int endIndex ) : KRange<int>(startIndex,endIndex) {}
inline KSection::KSection( const KSection &other ) : KRange<int>(other.start(),other.end()) {}
inline KSection::KSection()  {}
inline KSection::~KSection() {}

inline bool KSection::operator==( const KSection &other ) const { return KRange<int>::operator==(other); }

inline KSection &KSection::operator=( const KSection &other ) { KRange<int>::operator=(other); return *this; }

inline int KSection::width()       const { return isValid() ? end()-start()+1 : 0; }
inline int KSection::beforeStart() const { return start()-1; }
inline int KSection::behindEnd()   const { return end()+1; }

inline void KSection::setByWidth( int other, int width )  { setStart( other ); setEnd( other+width-1 ); }
inline void KSection::setStartByWidth( int width )  { setStart( end()-width+1 ); }
inline void KSection::setEndByWidth( int width )    { setEnd( start()+width-1 ); }
inline void KSection::setStartBehind( const KSection &other )  { setStart( other.end()+1 ); }
inline void KSection::setStartBehind( int index )  { setStart( index+1 ); }
inline void KSection::setEndBefore( const KSection &other )    { setEnd( other.start()-1 ); }
inline void KSection::setEndBefore( int index )    { setEnd( index-1 ); }
inline void KSection::restrictEndByWidth( int width ) { restrictEndTo( start()+width-1 ); }

inline void KSection::moveToStart( int other ) { setEnd( other+width()-1 ); setStart( other ); }
inline void KSection::moveToEnd( int end )   { setStart( end-width()+1 ); setEnd( end ); }

inline KSection KSection::splitAt( int index )
{
    const int secondEnd = end();
    setEndBefore( index );
    return KSection( index, secondEnd );
}
inline KSection KSection::splitAtLocal( int index )
{
    const int secondEnd = end();
    setEndByWidth( index );
    return KSection( behindEnd(), secondEnd );
}
inline KSection KSection::remove( const KSection &removeSection )
{
    const int secondEnd = end();
    setEndBefore( removeSection );
    return KSection( removeSection.behindEnd(), secondEnd );
}
inline KSection KSection::removeLocal( const KSection &removeSection )
{
    const int secondEnd = end();
    setEndByWidth( removeSection.start() );
    return KSection( start()+removeSection.behindEnd(), secondEnd );
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
inline bool KSection::isJoinable( const KSection &other ) const { return Start <= other.end()+1 && other.start()-1 <= End; }

inline bool KSection::prepend( const KSection &other )
{ const bool mergeable = ( other.end()+1 == start() ); if( mergeable ) setStart( other.start() ); return mergeable; }
inline bool KSection::append( const KSection &other )
{ const bool mergeable = ( end()+1 == other.start() ); if( mergeable ) setEnd( other.end() ); return mergeable; }

}

#endif
