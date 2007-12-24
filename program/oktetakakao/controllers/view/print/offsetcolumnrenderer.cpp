/***************************************************************************
                          offsetcolumnrenderer.cpp  -  description
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


#include "offsetcolumnrenderer.h"

// Qt
#include <QtGui/QPainter>


OffsetColumnRenderer::OffsetColumnRenderer( AbstractColumnFrameRenderer *columnFrameRenderer,
                                            int firstLineOffset, int delta, KOffsetFormat::KFormat format )
 : AbstractColumnRenderer( columnFrameRenderer ),
   mFirstLineOffset( firstLineOffset ),
   mDeltaPerLine( delta ),
   mMargin( 0 ),
   mDigitWidth( 0 ),
   mDigitBaseLine( 0 ),
   mFormat( KOffsetFormat::None )
{
    setFormat( format );
}


void OffsetColumnRenderer::renderLine( QPainter *painter, int line )
{
    printFunction()( mCodedOffset, mFirstLineOffset+mDeltaPerLine*line );

    painter->setPen( Qt::black );
    painter->drawText( 0, mDigitBaseLine, QString().append(mCodedOffset) );
}


void OffsetColumnRenderer::renderFirstLine( QPainter *painter, const KPixelXs &, int firstLine )
{
    mRenderLine = firstLine;
    renderLine( painter, mRenderLine++ );
}


void OffsetColumnRenderer::renderNextLine( QPainter *painter )
{
    renderLine( painter, mRenderLine++ );
}


void OffsetColumnRenderer::renderEmptyColumn( QPainter *painter, const KPixelXs &_Xs, const KPixelYs &Ys )
{
    KPixelXs Xs( _Xs );
    restrictToXSpan( &Xs );

    painter->fillRect( Xs.start(), Ys.start(), Xs.width(), Ys.width(), Qt::white );
}

void OffsetColumnRenderer::setFormat( KOffsetFormat::KFormat format )
{
    // no changes?
    if( mFormat == format )
        return;

    mFormat = format;

    mCodingWidth = KOffsetFormat::codingWidth( mFormat );
    mPrintFunction = KOffsetFormat::printFunction( mFormat );

    recalcX();
}

void OffsetColumnRenderer::setMetrics( KPixelX digitWidth, KPixelY digitBaseLine )
{
    mDigitBaseLine = digitBaseLine;
    setDigitWidth( digitWidth );
}

void OffsetColumnRenderer::setDigitWidth( KPixelX digitWidth )
{
    // no changes?
    if( mDigitWidth == digitWidth )
        return;

    mDigitWidth = digitWidth;

    recalcX();
}

void OffsetColumnRenderer::recalcX()
{
    // recalculate depend sizes
    setWidth( mCodingWidth * mDigitWidth );
}


OffsetColumnRenderer::~OffsetColumnRenderer() {}
