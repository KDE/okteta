/***************************************************************************
                          kbufferlayout.h  -  description
                             -------------------
    begin                : Thu Jun 12 2003
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


#ifndef KHE_KBUFFERLAYOUT_H
#define KHE_KBUFFERLAYOUT_H

// lib specific
#include "kcoordrange.h"
#include "ksection.h"

namespace KHE {

/**@short the logical layout of a plain buffer view
  *
  * Given the values for
  * * length of the buffer,
  * * number of bytes per line,
  * * a possible (relative) offset in the display, and
  * * the number of lines per page jump
  * the following values are calculated:
  * * starting line of display,
  * * starting position in this line,
  * * final line of display,
  * * final position in this line, and
  * * the total number of lines (is final line +1 or 0)
  *
  * This layout sees the buffer as a continous stream of byte,
  * thus uses each line after the start from the begin to the end.
  *
  * If the buffer is empty the end coord will be set one pos left to the start coord
  * to easen the cursor handling.
  *
  *@author Friedrich W. H.  Kossebau
  */
class KBufferLayout
{
  public:
    KBufferLayout( int NoBpL, int SO, int L );
    //KBufferLayout();
    ~KBufferLayout();


  public: // given values
    /** */
    int startOffset() const;
    /** returns number of bytes per line */
    int noOfBytesPerLine() const;
    /** returns the length of the displayed data (equals Buffer->size()) */
    int length() const;
    /** returns number of lines per visual page */
    int noOfLinesPerPage() const;

  public: // calculated values
    int startLine() const;
    int startPos() const;
    /** returns the coord of the start */
    KBufferCoord start() const;

    int finalLine() const;
    int finalPos() const;
    /** returns the coord of the end */
    KBufferCoord final() const;

    /** tells how much lines this layout needs (incl. blank leading lines due to StartOffset) */
    int noOfLines() const;


  public: // value calculation service
    /** calculates the index of the coord
      * If the coord is before the first coord the first index is returned,
      * if the coord is behind the last coord the last index is returned
      */
    int indexAtCCoord( const KBufferCoord &C ) const;
    /** calculates the index of the first pos in line.
      * If the line is below the first line the first index is returned,
      * if the line is above the last line the last index is returned
      */
    int indexAtCLineStart( int L ) const;
    /** calculates the index of last pos in line
      * If the line is below the first line the first index is returned,
      * if the line is above the last line the last index is returned
      */
    int indexAtCLineEnd( int L ) const;
    /** calculates the line in which index is found
      * If the index is below the first index the first line is returned,
      * if the index is above the last index the last line is returned
      */
    int lineAtCIndex( int Index ) const;
    /** calculates the coord in which index is found
      * If the index is below the first index the first coord is returned,
      * if the index is above the last index the last coord is returned
      */
    KBufferCoord coordOfCIndex( int Index ) const;

    /** calculates the index of coord. if coord is invalid the behaviour is undefinded */
    int indexAtCoord( const KBufferCoord &C ) const;
    /** calculates the index of the first pos in line. if line is invalid the behaviour is undefinded */
    int indexAtLineStart( int L ) const;
    /** calculates the index of last pos in line. if line is invalid the behaviour is undefinded */
    int indexAtLineEnd( int L ) const;
    /** calculates the line in which index is found. if index is invalid the behaviour is undefinded */
    int lineAtIndex( int Index ) const;
    /** calculates the coord in which index is found. if index is invalid the behaviour is undefinded */
    KBufferCoord coordOfIndex( int Index ) const;

    /** returns the used positions in line */
    KSection positions( int Line ) const;
    /** returns the first Pos in line. if line is invalid the behaviour is undefinded */
    int firstPos( int Line ) const;
    /** returns the last Pos in line. if line is invalid the behaviour is undefinded */
    int lastPos( int Line ) const;
    /** returns the valid Pos or the first Pos in line. if coord is invalid the behaviour is undefinded */
    int firstPos( const KBufferCoord &C ) const;
    /** returns the valid Pos or the last Pos in line. if coord is invalid the behaviour is undefinded */
    int lastPos( const KBufferCoord &C ) const;
    /** returns true if the line has content */
    bool hasContent( int Line ) const;
    /** returns true if the coord is the first in it's line. if coord is invalid the behaviour is undefinded */
    bool atLineStart( const KBufferCoord &C ) const;
    /** returns true if the coord is the last in it's line. if coord is invalid the behaviour is undefinded */
    bool atLineEnd( const KBufferCoord &C ) const;

    /** returns the index if valid or the nearest valid index */
    int correctIndex( int I ) const;
    /** returns the coord if valid or the nearest valid coord */
    KBufferCoord correctCoord( const KBufferCoord &C ) const;


  public: // modification access; return true if changes
     /** sets StartOffset, returns true if changed */
    bool setStartOffset( int SO );
    /** sets number of bytes per line, returns true if changed */
    bool setNoOfBytesPerLine( int N );
    /** sets number of lines per page */
    void setNoOfLinesPerPage( int N );
    /** sets length of data to display, returns true if changed */
    bool setLength( int L );


  protected:
    /** calculates the start coord by startoffset and number of bytes per line */
    void calcStart();
    /** calculates the final coord by startoffset, length, and number of bytes per line */
    void calcEnd();


 protected:
    /** how many chars per line */
    int NoOfBytesPerLine;
    /** starting offset of the displayed data */
    int StartOffset;
    /** length of the displayed buffer */
    int Length;
    /** number of lines that are moved by page up/down */
    int NoOfLinesPerPage;

  protected: // calculated values, buffered
    /** coord in which the start offset is (starting with 0) */
//    KBufferCoord Start;
    /** coord in which the last byte is (starting with 0) */
//    KBufferCoord Final;
    /** */
    KCoordRange ContentCoords;
};


inline int KBufferLayout::startOffset()       const { return StartOffset; }
inline int KBufferLayout::noOfBytesPerLine()  const { return NoOfBytesPerLine; }
inline int KBufferLayout::length()            const { return Length; }

inline KBufferCoord KBufferLayout::final()    const { return ContentCoords.end(); }
inline KBufferCoord KBufferLayout::start()    const { return ContentCoords.start(); }
inline int KBufferLayout::startPos()          const { return start().pos(); }
inline int KBufferLayout::finalPos()          const { return final().pos(); }
inline int KBufferLayout::startLine()         const { return start().line(); }
inline int KBufferLayout::finalLine()         const { return final().line(); }
inline int KBufferLayout::noOfLinesPerPage()  const { return NoOfLinesPerPage; }
inline int KBufferLayout::noOfLines()         const { return Length==0?0:final().line()+1; }

}

#endif
