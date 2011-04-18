/*
    This file is part of the Okteta Core library, made within the KDE community.

    Copyright 2003,2007-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef KDE_NUMBERRANGE_H
#define KDE_NUMBERRANGE_H

// lib
#include "range.h"


namespace KDE
{

/** describes a range of numbers which have a distance of 1 each
  *@author Friedrich W. H.  Kossebau
  */
template<typename N, typename S = N>
class NumberRange : public Range<N>
{
  public:
    /** constructs a range by width
     * @param startIndex starting index
     * @param width width of the range
     */
    static NumberRange fromWidth( N startIndex, S width );
    static NumberRange fromWidth( S width );
  public:
    /** constructs a range
     * @param startIndex starting index
     * @param endIndex end index
     */
    // FIXME: gcc 3.4.5 on windows gives an internal_compiler_error if ctor/dtor are defined with the keyword inline
    // thus put the function bodies here
    NumberRange( N startIndex, N endIndex ) : Range<N>(startIndex,endIndex) {}
    NumberRange( const NumberRange& other ) : Range<N>(other.start(),other.end()) {}
    NumberRange() {}

    ~NumberRange() {}

  public:
    NumberRange& operator=( const NumberRange& other );

  public:
    bool operator==( const NumberRange& other ) const;

  public:
    void setByWidth( N other, S width );
    /** sets the first index of the range's range one behind the other's end
      * If one of both is invalid the behaviour is undefined
      */
    void setStartNextBehind( const NumberRange& other );
    void setStartNextBehind( N index );
    /** sets the first index of the range's range one behind the other's end
      * If one of both is invalid or the other' start is 0 the behaviour is undefined
      */
    void setEndNextBefore( const NumberRange& other );
    void setEndNextBefore( N index );
    /** sets the first index of the range's range to be width-1 before the end
     * If the range is invalid the behaviour is undefined
     */
    void setStartByWidth( S width );
    /** sets the last index of the range's range to be width-1 behind the start
      * If the range is invalid the behaviour is undefined
      */
    void setEndByWidth( S width );
    /** restricts the end by width. If the range is invalid the behaviour is undefined */
    void restrictEndByWidth( S width );
    /** moves the range defined by a new start.
     * If the range is invalid the behaviour is undefined
     */
    void moveToStart( N other );
    /** moves the range defined by a new end.
     * If the range is invalid the behaviour is undefined
     */
    void moveToEnd( N end );

    void adaptToReplacement( N offset, S removedLength, S insertedLength );

    NumberRange splitAt( N index );
    NumberRange splitAtLocal( N index );
    NumberRange remove( const NumberRange& removeRange );
    NumberRange removeLocal( const NumberRange& removeRange );

    bool prepend( const NumberRange& other );
    bool append( const NumberRange& other );

  public:
    /**
     * @return the numbered of included indizes or 0, if the range is invalid
     */
    S width() const;
    N nextBeforeStart() const;
    N nextBehindEnd() const;

  public:
    /** @return index relative to the start */
    N localIndex( N index ) const;
    NumberRange localRange( const NumberRange& other ) const;
    /** @return range given by local  */
    NumberRange subRange( const NumberRange& localRange ) const;
    /**
     * @return the needed start so that other gets included, undefined if any is invalid
     */
    N startForInclude( const NumberRange& other ) const;
    /** @returns true if both range . If one of both is invalid the behaviour is undefined */
    bool isJoinable( const NumberRange& other ) const;
};


template<typename N,typename S>
inline NumberRange<N,S> NumberRange<N,S>::fromWidth( N startIndex, S width ) { return NumberRange(startIndex,startIndex+width-1); }
template<typename N,typename S>
inline NumberRange<N,S> NumberRange<N,S>::fromWidth( S width ) { return NumberRange<N,S>(0,width-1); }

template<typename N,typename S>
inline bool NumberRange<N,S>::operator==( const NumberRange<N,S>& other ) const { return Range<N>::operator==(other); }

template<typename N,typename S>
inline NumberRange<N,S>& NumberRange<N,S>::operator=( const NumberRange<N,S>& other ) { Range<N>::operator=(other); return *this; }

template<typename N,typename S>
inline S NumberRange<N,S>::width()       const { return Range<N>::isValid() ? Range<N>::end()-Range<N>::start()+1 : 0; }
template<typename N,typename S>
inline N NumberRange<N,S>::nextBeforeStart() const { return Range<N>::start()-1; }
template<typename N,typename S>
inline N NumberRange<N,S>::nextBehindEnd()   const { return Range<N>::end()+1; }

template<typename N,typename S>
inline void NumberRange<N,S>::setByWidth( N other, S width )  { Range<N>::setStart( other ); Range<N>::setEnd( other+width-1 ); }
template<typename N,typename S>
inline void NumberRange<N,S>::setStartByWidth( S width )  { Range<N>::setStart( Range<N>::end()-width+1 ); }
template<typename N,typename S>
inline void NumberRange<N,S>::setEndByWidth( S width )    { Range<N>::setEnd( Range<N>::start()+width-1 ); }
template<typename N,typename S>
inline void NumberRange<N,S>::setStartNextBehind( const NumberRange<N,S>& other )  { Range<N>::setStart( other.nextBehindEnd() ); }
template<typename N,typename S>
inline void NumberRange<N,S>::setStartNextBehind( N index )  { Range<N>::setStart( index+1 ); }
template<typename N,typename S>
inline void NumberRange<N,S>::setEndNextBefore( const NumberRange<N,S>& other )    { Range<N>::setEnd( other.nextBeforeStart() ); }
template<typename N,typename S>
inline void NumberRange<N,S>::setEndNextBefore( N index )    { Range<N>::setEnd( index-1 ); }
template<typename N,typename S>
inline void NumberRange<N,S>::restrictEndByWidth( S width ) { Range<N>::restrictEndTo( Range<N>::start()+width-1 ); }

template<typename N,typename S>
inline void NumberRange<N,S>::moveToStart( N other ) { Range<N>::setEnd( other+width()-1 ); Range<N>::setStart( other ); }
template<typename N,typename S>
inline void NumberRange<N,S>::moveToEnd( N end )   { Range<N>::setStart( end-width()+1 ); Range<N>::setEnd( end ); }

template<typename N,typename S>
inline NumberRange<N,S> NumberRange<N,S>::splitAt( N index )
{
    const N secondEnd = Range<N>::end();
    setEndNextBefore( index );
    return NumberRange<N,S>( index, secondEnd );
}
template<typename N,typename S>
inline NumberRange<N,S> NumberRange<N,S>::splitAtLocal( N index )
{
    const N secondEnd = Range<N>::end();
    setEndByWidth( index );
    return NumberRange<N,S>( nextBehindEnd(), secondEnd );
}
template<typename N,typename S>
inline NumberRange<N,S> NumberRange<N,S>::remove( const NumberRange<N,S>& removeRange )
{
    const N secondEnd = Range<N>::end();
    setEndNextBefore( removeRange );
    return NumberRange<N,S>( removeRange.nextBehindEnd(), secondEnd );
}
template<typename N,typename S>
inline NumberRange<N,S> NumberRange<N,S>::removeLocal( const NumberRange<N,S>& removeRange )
{
    const N secondEnd = Range<N>::end();
    setEndByWidth( removeRange.start() );
    return NumberRange<N,S>( Range<N>::start()+removeRange.nextBehindEnd(), secondEnd );
}

template<typename N,typename S>
inline N NumberRange<N,S>::localIndex( N index ) const { return index - Range<N>::start(); }
template<typename N,typename S>
inline NumberRange<N,S> NumberRange<N,S>::localRange( const NumberRange<N,S>& other ) const
{ return NumberRange<N,S>( other.start()-Range<N>::start(), other.end()-Range<N>::start() ); }
template<typename N,typename S>
inline NumberRange<N,S> NumberRange<N,S>::subRange( const NumberRange<N,S>& localRange ) const
{ return NumberRange<N,S>( localRange.start()+Range<N>::start(), localRange.end()+Range<N>::start() ); }

template<typename N,typename S>
inline N NumberRange<N,S>::startForInclude( const NumberRange<N,S>& other ) const
{
  return Range<N>::startsBehind(other.start()) ? other.start() :
         Range<N>::endsBefore(other.end()) ?     other.end()-width()+1 :
         Range<N>::start();
}
template<typename N,typename S>
inline bool NumberRange<N,S>::isJoinable( const NumberRange<N,S>& other ) const
{ return Range<N>::start() <= other.nextBehindEnd() && other.nextBeforeStart() <= Range<N>::end(); }

template<typename N,typename S>
inline bool NumberRange<N,S>::prepend( const NumberRange<N,S>& other )
{ const bool mergeable = ( other.nextBehindEnd() == Range<N>::start() ); if( mergeable ) Range<N>::setStart( other.start() ); return mergeable; }
template<typename N,typename S>
inline bool NumberRange<N,S>::append( const NumberRange<N,S>& other )
{ const bool mergeable = ( nextBehindEnd() == other.start() ); if( mergeable ) Range<N>::setEnd( other.end() ); return mergeable; }

template<typename N,typename S>
inline void NumberRange<N,S>::adaptToReplacement( N offset, S removedLength, S insertedLength )
{
    // nothing to adapt or not affected at all??
    if( ! Range<N>::isValid() || Range<N>::endsBefore(offset-1) )
        return;

    // indirectly affected?
    if( !Range<N>::startsBefore(offset+removedLength) )
    {
        Range<N>::moveBy( insertedLength-removedLength );
    }
    // changes overlap, oh well
    else
    {
        // only inserted?
        if( removedLength==0 )
        {
            if( Range<N>::startsBefore(offset) && !Range<N>::endsBefore(offset-1) )
                Range<N>::moveEndBy( insertedLength );
        }
        // only removed?
        else if( insertedLength==0 )
        {
            Range<N>::extendStartTo(offset);
            Range<N>::moveEndBy( -removedLength );
            Range<N>::extendEndTo(offset-1);
            // equals "if( End>offset+removedLength ) End -= removedLength; else End = offset-1;"
        }
        else
        {
            if( Range<N>::startsBehind(offset) )
                Range<N>::setStart( offset+insertedLength );
            if( Range<N>::endsBefore(offset+removedLength-1) )
                Range<N>::setEnd( offset-1 );
            else
                Range<N>::moveEndBy( insertedLength-removedLength );
        }
    }
}

}

#endif
