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

#ifndef KHE_SECTION_H
#define KHE_SECTION_H

// lib
#include "range.h"
#include "oktetacore_export.h"


namespace KHE
{

/** describes a section of indizes
  *@author Friedrich W. H.  Kossebau
  */
class OKTETACORE_EXPORT Section : public Range<int>
{
  public:
    /** constructs a section by width
     * @param startIndex starting index
     * @param width width of the section
     */
    static Section fromWidth( int startIndex, int width );
    static Section fromWidth( int width );
  public:
    /** constructs a section
     * @param startIndex starting index
     * @param endIndex end index
     */
    // FIXME: gcc 3.4.5 on windows gives an internal_compiler_error if ctor/dtor are defined with the keyword inline
    // thus put the function bodies here
    Section( int startIndex, int endIndex ) : Range<int>(startIndex,endIndex) {}
    Section( const Section &other ) : Range<int>(other.start(),other.end()) {}
    Section() {}
    ~Section() {}

  public:
    Section &operator=( const Section &other );

  public:
    bool operator==( const Section &other ) const;

  public:
    void setByWidth( int other, int width );
    /** sets the first index of the section's range one behind the other's end
      * If one of both is invalid the behaviour is undefined
      */
    void setStartNextBehind( const Section &other );
    void setStartNextBehind( int index );
    /** sets the first index of the section's range one behind the other's end
      * If one of both is invalid or the other' start is 0 the behaviour is undefined
      */
    void setEndNextBefore( const Section &other );
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

    Section splitAt( int index );
    Section splitAtLocal( int index );
    Section remove( const Section &removeSection );
    Section removeLocal( const Section &removeSection );

    bool prepend( const Section &other );
    bool append( const Section &other );

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
    Section localSection( const Section &other ) const;
    /** @return section given by local  */
    Section subSection( const Section &localSection ) const;
    /**
     * @return the needed start so that other gets included, undefined if any is invalid
     */
    int startForInclude( const Section &other ) const;
    /** @returns true if both section . If one of both is invalid the behaviour is undefined */
    bool isJoinable( const Section &other ) const;
};

inline Section Section::fromWidth( int startIndex, int width ) { return Section(startIndex,startIndex+width-1); }
inline Section Section::fromWidth( int width ) { return Section(0,width-1); }

inline bool Section::operator==( const Section &other ) const { return Range<int>::operator==(other); }

inline Section &Section::operator=( const Section &other ) { Range<int>::operator=(other); return *this; }

inline int Section::width()       const { return isValid() ? end()-start()+1 : 0; }
inline int Section::nextBeforeStart() const { return start()-1; }
inline int Section::nextBehindEnd()   const { return end()+1; }

inline void Section::setByWidth( int other, int width )  { setStart( other ); setEnd( other+width-1 ); }
inline void Section::setStartByWidth( int width )  { setStart( end()-width+1 ); }
inline void Section::setEndByWidth( int width )    { setEnd( start()+width-1 ); }
inline void Section::setStartNextBehind( const Section &other )  { setStart( other.nextBehindEnd() ); }
inline void Section::setStartNextBehind( int index )  { setStart( index+1 ); }
inline void Section::setEndNextBefore( const Section &other )    { setEnd( other.nextBeforeStart() ); }
inline void Section::setEndNextBefore( int index )    { setEnd( index-1 ); }
inline void Section::restrictEndByWidth( int width ) { restrictEndTo( start()+width-1 ); }

inline void Section::moveToStart( int other ) { setEnd( other+width()-1 ); setStart( other ); }
inline void Section::moveToEnd( int end )   { setStart( end-width()+1 ); setEnd( end ); }

inline Section Section::splitAt( int index )
{
    const int secondEnd = end();
    setEndNextBefore( index );
    return Section( index, secondEnd );
}
inline Section Section::splitAtLocal( int index )
{
    const int secondEnd = end();
    setEndByWidth( index );
    return Section( nextBehindEnd(), secondEnd );
}
inline Section Section::remove( const Section &removeSection )
{
    const int secondEnd = end();
    setEndNextBefore( removeSection );
    return Section( removeSection.nextBehindEnd(), secondEnd );
}
inline Section Section::removeLocal( const Section &removeSection )
{
    const int secondEnd = end();
    setEndByWidth( removeSection.start() );
    return Section( start()+removeSection.nextBehindEnd(), secondEnd );
}

inline int Section::localIndex( int index ) const { return index - start(); }
inline Section Section::localSection( const Section &other ) const
{ return Section( other.start()-start(), other.end()-start() ); }
inline Section Section::subSection( const Section &localSection ) const
{ return Section( localSection.start()+start(), localSection.end()+start() ); }

inline int Section::startForInclude( const Section &other ) const
{
  return startsBehind(other.start()) ? other.start() :
         endsBefore(other.end()) ?     other.end()-width()+1 :
         start();
}
inline bool Section::isJoinable( const Section &other ) const
{ return Start <= other.nextBehindEnd() && other.nextBeforeStart() <= End; }

inline bool Section::prepend( const Section &other )
{ const bool mergeable = ( other.nextBehindEnd() == start() ); if( mergeable ) setStart( other.start() ); return mergeable; }
inline bool Section::append( const Section &other )
{ const bool mergeable = ( nextBehindEnd() == other.start() ); if( mergeable ) setEnd( other.end() ); return mergeable; }

}

#endif
