/***************************************************************************
                          kbuffercursor.h  -  description
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


#ifndef KHE_KBUFFERCURSOR_H
#define KHE_KBUFFERCURSOR_H

#include "kbuffercoord.h"


namespace KHE
{

class KBufferLayout;


/**@short navigates through the buffer in an abstract way, based on the layout
  *
  * The cursor is allowed to access every coord that has content as
  * described in the layout. It holds the coord of the actual position
  * and the according index in the data array.
  *
  * To enable the cursor to be placed behind the last position in a line
  * (e.g, to mark all data in the line without placing the cursor to the
  * beginning of the next line) there is a flag Behind that should be read
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
  * The cursor will be placed to coord 0/0 with index 1, Behind=false.
  *
  *@author Friedrich W. H. Kossebau
  */
class KBufferCursor
{
  public:
    KBufferCursor( const KBufferLayout *L );
    ~KBufferCursor();


  public:
    bool operator==( const KBufferCursor &c ) const;
    bool operator!=( const KBufferCursor &c ) const { return !(*this == c); }

  public: // modificator
    void setAppendPosEnabled( bool APE=true );

  public: // state value access
    /** the index that is drawn at the actual coord */
    int index() const;
    /** the pos of the actual coord */
    int pos() const;
    /** the line of the actual coord */
    int line() const;
    /** the actual coord */
    KBufferCoord coord() const;
    /** true if the cursor is located to the right of the actual coord but still shown at the coord */
    bool isBehind() const;
    /** returns the real index. That is if the cursor is tagged as "behind" the current index
      * it's real index is the next one.
      * Attention: this could be outside the data's range if the cursor is behind the last byte!
      */
    int realIndex() const;
    /** returns the true index if it is valid index that is it is inside the data's range.
      * Otherwise -1 is returned
      */
    int validIndex() const;

    //bool isValid() const;
    bool appendPosEnabled() const;

  public: // index calculation service
    /** returns the index at the start of the cursor's line */
    int indexAtLineStart() const;
    /** returns the index at the end of the cursor's line */
    int indexAtLineEnd() const;

  public: // navigation commands
    void gotoIndex( int I );
    void gotoCoord( const KBufferCoord &C );
    void gotoCIndex( int I );
    void gotoCCoord( const KBufferCoord &C );
    void gotoRealIndex();

    void gotoPreviousByte();
    void gotoNextByte();
    void gotoPreviousByte( int D );
    void gotoNextByte( int D );
    void gotoNextByteInLine();
    void gotoUp();
    void gotoDown();
    void gotoLineStart();
    void gotoLineEnd();
    void gotoStart();
    void gotoEnd();
    void gotoPageUp();
    void gotoPageDown();

    /** puts the cursor behind the actual position if it isn't already*/
    void stepBehind();
    void updateCoord();


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
    const KBufferLayout *Layout;

    /** Position in buffer */
    int Index;
    /** Position and Line */
    KBufferCoord Coord;

    /** tells whether the cursor is actually behind the actual position.
      * This is used for selection to the end of a line or of the whole buffer.
      */
    bool Behind : 1;

    /** tells whether there could be a position behind the end of the layout */
    bool AppendPosEnabled : 1;
};


inline int KBufferCursor::index()          const { return Index; }
inline int KBufferCursor::pos()            const { return Coord.pos(); }
inline int KBufferCursor::line()           const { return Coord.line(); }
inline KBufferCoord KBufferCursor::coord() const { return Coord; }
inline bool KBufferCursor::isBehind()      const { return Behind; }
inline int KBufferCursor::realIndex()      const { return Behind ? Index + 1 : Index; }

inline void KBufferCursor::stepBehind() { Behind = true; }

//inline bool KBufferCursor::isValid()  const { return Index != -1; }

}

#endif
