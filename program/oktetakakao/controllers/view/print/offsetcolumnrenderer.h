/***************************************************************************
                          offsetcolumnrenderer.h  -  description
                             -------------------
    begin                : Mit Mai 14 2003
    copyright            : 2003 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License version 2 as published by the Free Software Foundation.       *
 *                                                                         *
 ***************************************************************************/


#ifndef OFFSETCOLUMNRENDERER_H
#define OFFSETCOLUMNRENDERER_H

// lib
#include "abstractcolumnrenderer.h"
// Okteta ui
#include <koffsetformat.h>


/**
  *@author Friedrich W. H. Kossebau
  */

class OffsetColumnRenderer : public AbstractColumnRenderer
{
  public:
    OffsetColumnRenderer( AbstractColumnFrameRenderer *columnFrameRenderer,
                          int firstLineOffset, int delta, KOffsetFormat::KFormat format );
    virtual ~OffsetColumnRenderer();

  public:  // AbstractColumnRenderer API
    virtual void renderFirstLine( QPainter *painter, const KPixelXs &Xs, int firstLine );
    virtual void renderNextLine( QPainter *painter );
    virtual void renderEmptyColumn( QPainter *painter, const KPixelXs &Xs, const KPixelYs &Ys );


  public:
    void setFirstLineOffset( int firstLineOffset );
    void setDelta( int delta );

    void setFormat( KOffsetFormat::KFormat format );
    /** sets width of digits and recalculates depend sizes  */
    void setDigitWidth( KPixelX digitWidth );
    /** */
    void setMetrics( KPixelX digitWidth, KPixelY digitBaseLine );

  public: // read access
    int delta() const;
    int firstLineOffset() const;
    int codingWidth() const;
    KOffsetFormat::print printFunction() const;


  protected:
    /** recalculates all x values */
    void recalcX();
    /** paints full line */
    void renderLine( QPainter *painter, int line );


  protected: // user settings
    /** starting offset of the first line
      * if different from StartOffset results in leading space
      */
    int mFirstLineOffset;
    /** offset delta per line */
    int mDeltaPerLine;

  protected: // pixel related
    /** size of the line margin */
    int mMargin;
    /** */
    KPixelX mDigitWidth;
    /** */
    KPixelY mDigitBaseLine;

  protected: // general layout
    KOffsetFormat::KFormat mFormat;

    int mCodingWidth;
    KOffsetFormat::print mPrintFunction;

    /** buffer to hold the formatted coding */
    mutable char mCodedOffset[KOffsetFormat::MaxFormatWidth+1];

  protected: // firstnext trips related
    /** */
    int mRenderLine;
};


inline int OffsetColumnRenderer::firstLineOffset()                 const { return mFirstLineOffset; }
inline int OffsetColumnRenderer::delta()                           const { return mDeltaPerLine; }
inline int OffsetColumnRenderer::codingWidth()                     const { return mCodingWidth; }
inline KOffsetFormat::print OffsetColumnRenderer::printFunction()  const { return mPrintFunction; }

inline void OffsetColumnRenderer::setFirstLineOffset( int firstLineOffset ) { mFirstLineOffset = firstLineOffset; }
inline void OffsetColumnRenderer::setDelta( int delta )                     { mDeltaPerLine = delta; }

#endif
