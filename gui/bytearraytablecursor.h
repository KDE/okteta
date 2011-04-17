/*
    This file is part of the Okteta Gui library, made within the KDE community.

    Copyright 2003,2008-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef OKTETA_BYTEARRAYTABLECURSOR_H
#define OKTETA_BYTEARRAYTABLECURSOR_H

// lib
#include "coord.h"


namespace Okteta
{
class ArrayChangeMetricsList;
class ByteArrayTableLayout;


/**@short navigates through the buffer in an abstract way, based on the layout
  *
  * The cursor is allowed to access every coord that has content as
  * described in the layout. It holds the coord of the actual position
  * and the according index in the data array.
  *
  * To enable the cursor to be placed behind the last position in a line
  * (e.g, to mark all data in the line without placing the cursor to the
  * beginning of the next line) there is a flag mBehind that should be read
  * as that the real index the cursor is at is the current one + 1
  * (as returned by realIndex())
  *
  * For appending new data to the buffer there is also the need to be able
  * to place the cursor at a position behind the last byte. This can be
  * enabled by calling setAppendPosEnabled(true). If the cursor is placed to
  * this position it gets the (real) index of the last byte + 1. As this
  * index does not point to an existing byte validIndex() returns -1.
  * Check for atAppendPos() to see whether cursor is at this position.
  *
  * If the buffer is empty there is no navigation possible, of course.
  * The cursor will be placed to coord 0/0 with index 1, mBehind=false.
  *
  *@author Friedrich W. H. Kossebau
  */
class ByteArrayTableCursor
{
  public:
    explicit ByteArrayTableCursor( const ByteArrayTableLayout* layout );
    ~ByteArrayTableCursor();

  public: // modificator
    void setAppendPosEnabled( bool appendPosEnabled = true );

  public: // state value access
    /** the index that is drawn at the actual coord */
    Address index() const;
    /** the pos of the actual coord */
    LinePosition pos() const;
    /** the line of the actual coord */
    Line line() const;
    /** the actual coord */
    Coord coord() const;
    /** true if the cursor is located to the right of the actual coord but still shown at the coord */
    bool isBehind() const;
    /** returns the real index. That is if the cursor is tagged as "behind" the current index
      * it's real index is the next one.
      * Attention: this could be outside the data's range if the cursor is behind the last byte!
      */
    Address realIndex() const;
    /** returns the true index if it is valid index that is it is inside the data's range.
      * Otherwise -1 is returned
      */
    Address validIndex() const;

    //bool isValid() const;
    /**
     *  Default is false.
     */
    bool appendPosEnabled() const;

  public: // index calculation service
    /** returns the index at the start of the cursor's line */
    Address indexAtLineStart() const;
    /** returns the index at the end of the cursor's line */
    Address indexAtLineEnd() const;

  public: // navigation commands
    void gotoIndex( Address index );
    void gotoCoord( const Coord& coord );
    void gotoCIndex( Address index );
    void gotoCCoord( const Coord& coord );
    /** sets the index to the real index, i.e. if "behind" one index, sets it to the next.
      * Undefined if the real index is invalid, or on the append pos if not allowed.
      */
    void gotoRealIndex();

    void gotoPreviousByte();
    void gotoNextByte();
    void gotoPreviousByte( Size indexSteps );
    void gotoNextByte( Size indexSteps );
    void gotoUp();
    void gotoDown();
    void gotoLineStart();
    void gotoLineEnd();
    void gotoStart();
    /** sets the index behind the last index.
      * If appendPosEnabled is true, this will be the last index + 1,
      * otherwise it will be the last index and the flag behind.
      */
    void gotoEnd();
    void gotoPageUp();
    void gotoPageDown();

    /** puts the cursor behind the actual position if it isn't already*/
    // TODO: make protected again
    void stepBehind();
    void updateCoord();
    void adaptToChanges( const ArrayChangeMetricsList& changeList, Size oldLength );

  public: // logical state access
    bool atStart() const;
    bool atEnd() const;
    /** could only be true in InsertMode: Cursor is behind the last byte */
    bool atAppendPos() const;
    bool atLineStart() const;
    bool atLineEnd() const;


  protected:
    /** if newpos allowed steps at a coord behind the last existing
      * or, if that is at a line end, behind the line
      * does not check for empty content!
      */
    void stepToEnd();

  private:
    /** layout, tells how the column is organized  */
    const ByteArrayTableLayout* mLayout;

    /** Position in buffer */
    Address mIndex;
    /** Position and Line */
    Coord mCoord;

    /** tells whether the cursor is actually behind the actual position.
      * This is used for selection to the end of a line or of the whole buffer.
      */
    bool mBehind :1;

    /** tells whether there could be a position behind the end of the layout */
    bool mAppendPosEnabled :1;
};


inline Address ByteArrayTableCursor::index()      const { return mIndex; }
inline LinePosition ByteArrayTableCursor::pos()   const { return mCoord.pos(); }
inline Line ByteArrayTableCursor::line()          const { return mCoord.line(); }
inline Coord ByteArrayTableCursor::coord()        const { return mCoord; }
inline bool ByteArrayTableCursor::isBehind()      const { return mBehind; }
inline Address ByteArrayTableCursor::realIndex()  const { return mBehind ? mIndex + 1 : mIndex; }
inline bool ByteArrayTableCursor::appendPosEnabled() const { return mAppendPosEnabled; }

inline void ByteArrayTableCursor::stepBehind() { mBehind = true; }

//inline bool ByteArrayTableCursor::isValid()  const { return mIndex != -1; }

}

#endif
