/*
    This file is part of the Okteta Gui library, part of the KDE project.

    Copyright 2003 Friedrich W. H. Kossebau <kossebau@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in mSection 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef KHE_UI_SELECTION_H
#define KHE_UI_SELECTION_H

// commonlib
#include <ksection.h>


namespace KHEUI
{

/** This class describes a selected section of the buffer.
  * As it is used as selection controlled by
  * mouse and keyboard commands it offers two ways to set its range:
  * - by giving the startposition (of the cursor) of an interactive selection
  *   and the subsequent end positions (until selection is finished)
  * - direct setting (as provided by KHE::KSection)
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
    explicit Selection( int index );
    /** creates an invalid selection */
    Selection();
    ~Selection();

  public:
    Selection &operator=( const Selection &other );
    Selection &operator=( const KHE::KSection &section );

  public: // modification access
    /** starts the selection.
      * For this the anchor, start and end are set to the given index,
      * so the initial selection is empty.
      * @param index index in front of which the selection begins
      */
    void setStart( int index );
    /** sets the end of the current selection.
      * If the end is before the start the selection will reach from the given index 
      * @param index index in front of which the selection ends
      */
    void setEnd( int index );
    /** sets the selection to be invalid
      */
    void cancel();
    /** sets the anchor to the start or the end.
      * @param if true to the start, otherwise to the end
      * If the selection has not started the behaviour is undefined.
      */
    void setForward( bool forward = true );
    /** swaps anchor from start to end or vice versa.
      * If the selection has not started the behaviour is undefined.
      */
    void reverse();

    void adaptToReplacement( int pos, int removedLength, int insertedLength );
    void adaptToSwap( int firstOffset, int secondOffset, int secondLength );

  public: // value access
    /** 
      * @return anchor value
      */
    int anchor() const;
    int start() const;
    int end() const;
    /** 
      * @return section
      */
    const KHE::KSection &section() const;


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
    /** mSection */
    KHE::KSection mSection;
    /** cursor index where the selection starts */
    int mAnchor;
};


inline Selection::Selection() : mAnchor( -1 ) {}
inline Selection::Selection( int index ) : mAnchor( index )  {}
inline Selection::~Selection() {}

inline Selection &Selection::operator=( const Selection &other )
{
    mSection = other.mSection;
    mAnchor = other.mAnchor;
    return *this;
}

inline Selection &Selection::operator=( const KHE::KSection &section )
{
    mSection = section;
    mAnchor = section.start();
    return *this;
}


inline void Selection::setStart( int index )
{
    mAnchor = index;
    mSection.unset();
}


inline void Selection::setEnd( int index )
{
    // nothing selected?
    if( index == mAnchor )
        mSection.unset();
    // selecting forwards?
    else if( index > mAnchor )
    {
        mSection.setStart( mAnchor );
        mSection.setEnd( index-1 );
    }
    // selecting backwards
    else
    {
        mSection.setStart( index );
        mSection.setEnd( mAnchor-1 );
    }
}

inline void Selection::reverse()
{
    mAnchor = isForward() ? mSection.end()+1 : mSection.start();
}

inline void Selection::setForward( bool Forward )
{
    mAnchor = Forward ? mSection.start() : mSection.end()+1;
}

inline const KHE::KSection &Selection::section() const { return mSection; }
inline int Selection::anchor()              const { return mAnchor; }
inline int Selection::start()               const { return mSection.start(); }
inline int Selection::end()                 const { return mSection.end(); }

inline void Selection::cancel() { mAnchor = -1; mSection.unset(); }

inline bool Selection::isValid()     const { return mSection.isValid(); }
inline bool Selection::started()     const { return mAnchor != -1; }
inline bool Selection::justStarted() const { return mAnchor != -1 && mSection.start() == -1; }
inline bool Selection::isForward()   const { return mAnchor == mSection.start(); }

inline void Selection::adaptToReplacement( int pos, int removedLength, int insertedLength )
{
    mSection.adaptToReplacement( pos, removedLength, insertedLength );
    mAnchor = isForward() ? mSection.start() : mSection.end()+1;
}

inline void Selection::adaptToSwap( int firstOffset, int secondOffset, int secondLength )
{
    // no intersection?
    if( mSection.end() < firstOffset || mSection.start() > secondOffset+secondLength-1 )
        return;

    const KHE::KSection firstSection( firstOffset, secondOffset-1 );
    if( firstSection.includes(mSection) )
        mSection.moveBy( secondLength );
    else
    {
        const KHE::KSection secondSection = KHE::KSection::fromWidth( secondOffset, secondLength );
        if( secondSection.includes(mSection) )
            mSection.moveBy( -firstSection.width() );
        else
            mSection.unset();
    }
}

}

#endif
