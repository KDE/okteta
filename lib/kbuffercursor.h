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


#ifndef KBUFFERCURSOR_H
#define KBUFFERCURSOR_H

#include "kbuffercoord.h"

namespace KHE
{

class KBufferLayout;

/*
TODO: two edit possibilitites for multidigit representation of bytes
1. edit digit by digit
(I can't help it but it seems only useful when in overwriting mode)
each highlighted digit is substituted by the one that has been inputted
if we want to enable inserting we have to make clear what happens
when the cursor is before the first digit. Perhaps this could be solved by
an intermediate cursor step that shows up _between_ the chars.
what about backspace and del? should only work with modificator (Ctrl) for
the whole chars (in opposite to khexedit1). Or work different when "inside" a byte
(dealing with digits) and "outside" (dealing with whole bytes)

2. edit by full substitute:
if the first digit key is pressed after the switch on another char
a) the whole char is set to 0
b) a new char is inserted at place, set to 0
and the new value is edited (starting with
the key's digit) like with a lineedit.
A Enter or a cursor move finish that chars input
When the entered digits fill up the number of possible digits or do not allow
another digit (e.g. dez: 62) the cursor jumps to the next char


INSERT vs. OVERWRITE:
in insert-mode we put an additional break between chars to offer the possibility
to insert new bytes
LineStart is 0 (insert in front of char), LineEnd is the last (or in front of the last)

an edited multidigit char is surrounded by a rectangle, so we can see whether a char is
actually edited

if readonly does it make sense to go through the digits one by one? No!
*/

/** handles all the actions onto the buffer
  * navigates through the buffer in an abstract way, based on the layout
  * doesn't need to know about left-to-right or right-to-left
  *@author Friedrich W. H. Kossebau
  */
class KBufferCursor
{
  public:
    KBufferCursor( const KBufferLayout *L );
//     KBufferCursor( const KBufferCursor &c );
//     KBufferCursor &operator=( const KBufferCursor &c );
    ~KBufferCursor();


  public:
    bool operator==( const KBufferCursor &c ) const;
    bool operator!=( const KBufferCursor &c ) const { return !(*this == c); }

  public:
    void setNewPosAllowed( bool NPA=true );

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
    /** returns the true index. That is if the cursor is tagged as "behind" the actual index
      * it's true index is the next one.
      * Attention: this could be outside the data's range if the cursor is behind the last byte!
      */
    int trueIndex() const;
    /** returns the true index if it is valid index that is it is inside the data's range.
      * Otherwise -1 is returned
      */
    int validIndex() const;

    bool isValid() const;
    bool newPosAllowed() const;

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
    void gotoTrueIndex();

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

    void updateCoord();


  public: // logical state access
    bool atStart() const;
    bool atEnd() const;
    /** could only be true in InsertMode: Cursor is behind the last byte */
    bool isBehindEnd() const;
    bool atLineStart() const;
    bool atLineEnd() const;


  protected:
    /** if newpos allowed steps at a coord behind the last existing
      * or, if that is at a line end, behind the line */
    void stepToEnd();

  private:
    /** layout, tells how the column is organized  */
    const KBufferLayout *Layout;

    /** Position in buffer */
    int Index;
    /** Position and Line */
    KBufferCoord Coord;

    /** tells whether the cursor is actually behind the actual position.
      * This is used for selection to the end of a line or the whole buffer.
      */
    bool Behind : 1;

    /** tells whether there could be a position behind the end of the layout */
    bool NewPosAllowed : 1;
};


inline int KBufferCursor::index()          const { return Index; }
inline int KBufferCursor::pos()            const { return Coord.pos(); }
inline int KBufferCursor::line()           const { return Coord.line(); }
inline KBufferCoord KBufferCursor::coord() const { return Coord; }
inline bool KBufferCursor::isBehind()      const { return Behind; }
inline int KBufferCursor::trueIndex()      const { return Index + (Behind?1:0); }

inline bool KBufferCursor::isValid()  const { return Index != -1; }

}

#endif
