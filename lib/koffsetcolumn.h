/***************************************************************************
                          koffsetcolumn.h  -  description
                             -------------------
    begin                : Mit Mai 14 2003
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


#ifndef KHE_KOFFSETCOLUMN_H
#define KHE_KOFFSETCOLUMN_H

// lib specific
#include "koffsetformat.h"
#include "kcolumn.h"

namespace KHE
{

/**
  *@author Friedrich W. H. Kossebau
  */

class KOffsetColumn : public KColumn
{
  public:
    KOffsetColumn( KColumnsView *V, int FLO, int D, KOffsetFormat::KFormat F );
    virtual ~KOffsetColumn();

  public:  // KColumn API
    virtual void paintFirstLine( QPainter *P, KPixelXs Xs, int FirstLine );
    virtual void paintNextLine( QPainter *P );
    virtual void paintEmptyColumn( QPainter *P, KPixelXs Xs, KPixelYs Ys );


  public:
    void setFirstLineOffset( int FLO );
    void setDelta( int D );

    void setFormat( KOffsetFormat::KFormat F );
    /** sets width of digits and recalculates depend sizes  */
    void setDigitWidth( KPixelX DW );
    /** */
    void setMetrics( KPixelX DW, KPixelY DBL );

  public: // read access
    int delta() const;
    int firstLineOffset() const;
    int codingWidth() const;
    KOffsetFormat::print printFunction() const;


  protected:
    /** recalculates all x values */
    void recalcX();
    /** paints full line */
    void paintLine( QPainter *P, int Line );


  protected: // user settings
    /** starting offset of the first line
      * if different from StartOffset results in leading space
      */
    int FirstLineOffset;
    /** offset delta per line */
    int Delta;

  protected: // pixel related
    /** size of the line margin */
    int Margin;
    /** */
    KPixelX DigitWidth;
    /** */
    KPixelY DigitBaseLine;

  protected: // general layout
    KOffsetFormat::KFormat Format;

    int CodingWidth;
    KOffsetFormat::print PrintFunction;

    /** buffer to hold the formatted coding */
    mutable char CodedOffset[KOffsetFormat::MaxFormatWidth+1];

  protected: // firstnext trips related
    /** */
    int PaintLine;
};


inline int KOffsetColumn::firstLineOffset()  const  { return FirstLineOffset; }
inline void KOffsetColumn::setFirstLineOffset( int FLO ) { FirstLineOffset = FLO; }
inline int KOffsetColumn::delta() const { return Delta; }
inline void KOffsetColumn::setDelta( int D )  { Delta = D; }

inline int KOffsetColumn::codingWidth()                       const { return CodingWidth; }
inline KOffsetFormat::print KOffsetColumn::printFunction()    const { return PrintFunction; }

}

#endif
