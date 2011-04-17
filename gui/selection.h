/*
    This file is part of the Okteta Gui library, made within the KDE community.

    Copyright 2003,2008-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in mRange 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef OKTETA_SELECTION_H
#define OKTETA_SELECTION_H

// lib
#include "addressrange.h"


namespace Okteta
{

/** This class describes a selected range of the buffer.
  * As it is used as selection controlled by
  * mouse and keyboard commands it offers two ways to set its range:
  * - by giving the startposition (of the cursor) of an interactive selection
  *   and the subsequent end positions (until selection is finished)
  * - direct setting (as provided by AddressRange)
  *
  * the interactive selection takes care that
  *
  *@author Friedrich W. H.  Kossebau
  */
class Selection
{
  public:
    /** creates a selection with a given start.
      * @param index index in front of which the selection begins
      */
    explicit Selection( Address index );
    /** creates an invalid selection */
    Selection();

    ~Selection();

  public:
    Selection& operator=( const Selection& other );
    Selection& operator=( const AddressRange& range );

  public: // modification access
    /** starts the selection.
      * For this the anchor, start and end are set to the given index,
      * so the initial selection is empty.
      * @param index index in front of which the selection begins
      */
    void setStart( Address index );
    /** sets the end of the current selection.
      * If the end is before the start the selection will reach from the given index 
      * @param index index in front of which the selection ends
      */
    void setEnd( Address index );
    /** sets the selection to be invalid
      */
    void cancel();
    /** sets the anchor to the start or the end.
      * @param forward true to the start, otherwise to the end
      * If the selection has not started the behaviour is undefined.
      */
    void setForward( bool forward = true );
    /** swaps anchor from start to end or vice versa.
      * If the selection has not started the behaviour is undefined.
      */
    void reverse();

    void adaptToReplacement( Address pos, Size removedLength, Size insertedLength );
    void adaptToSwap( Address firstOffset, Address secondOffset, Size secondLength );

  public: // value access
    /** 
      * @return anchor value
      */
    Address anchor() const;
    Address start() const;
    Address end() const;
    Address nextBeforeStart() const;
    Address nextBehindEnd() const;
    /** 
      * @return range
      */
    const AddressRange& range() const;


  public: // logic access
    bool isValid() const;
    /** 
      * @return @c true if the anchor has been set, otherwise @c false.
      */
    bool started() const;
    /** 
      * @return @c true if the anchor has been set and the selection is empty, otherwise @c false.
      */
    bool justStarted() const;
    /** 
      * @return @c true if the anchor is at the begin of the selection 
      */
    bool isForward() const;

  protected:
    /** mRange */
    AddressRange mRange;
    /** cursor index where the selection starts */
    Address mAnchor;
};


inline Selection::Selection() : mAnchor( -1 ) {}
inline Selection::Selection( Address index ) : mAnchor( index )  {}
inline Selection::~Selection() {}

inline Selection &Selection::operator=( const Selection& other )
{
    mRange = other.mRange;
    mAnchor = other.mAnchor;
    return *this;
}

inline Selection &Selection::operator=( const AddressRange& range )
{
    mRange = range;
    mAnchor = range.start();
    return *this;
}


inline void Selection::setStart( Address index )
{
    mAnchor = index;
    mRange.unset();
}


inline void Selection::setEnd( Address index )
{
    // nothing selected?
    if( index == mAnchor )
        mRange.unset();
    // selecting forwards?
    else if( index > mAnchor )
    {
        mRange.setStart( mAnchor );
        mRange.setEnd( index-1 );
    }
    // selecting backwards
    else
    {
        mRange.setStart( index );
        mRange.setEnd( mAnchor-1 );
    }
}

inline void Selection::reverse()
{
    mAnchor = isForward() ? mRange.nextBehindEnd() : mRange.start();
}

inline void Selection::setForward( bool Forward )
{
    mAnchor = Forward ? mRange.start() : mRange.nextBehindEnd();
}

inline const AddressRange& Selection::range()   const { return mRange; }
inline Address Selection::anchor()              const { return mAnchor; }
inline Address Selection::start()               const { return mRange.start(); }
inline Address Selection::end()                 const { return mRange.end(); }
inline Address Selection::nextBeforeStart()     const { return mRange.nextBeforeStart(); }
inline Address Selection::nextBehindEnd()       const { return mRange.nextBehindEnd(); }

inline void Selection::cancel() { mAnchor = -1; mRange.unset(); }

inline bool Selection::isValid()     const { return mRange.isValid(); }
inline bool Selection::started()     const { return mAnchor != -1; }
inline bool Selection::justStarted() const { return mAnchor != -1 && mRange.start() == -1; }
inline bool Selection::isForward()   const { return mAnchor == mRange.start(); }

inline void Selection::adaptToReplacement( Address pos, Size removedLength, Size insertedLength )
{
    mRange.adaptToReplacement( pos, removedLength, insertedLength );
    mAnchor = isForward() ? mRange.start() : mRange.nextBehindEnd();
}

inline void Selection::adaptToSwap( Address firstOffset, Address secondOffset, Size secondLength )
{
    // no intersection?
    if( mRange.end() < firstOffset || mRange.start() > secondOffset+secondLength-1 )
        return;

    const AddressRange firstSection( firstOffset, secondOffset-1 );
    if( firstSection.includes(mRange) )
        mRange.moveBy( secondLength );
    else
    {
        const AddressRange secondRange = AddressRange::fromWidth( secondOffset, secondLength );
        if( secondRange.includes(mRange) )
            mRange.moveBy( -firstSection.width() );
        else
            mRange.unset();
    }
}

}

#endif
