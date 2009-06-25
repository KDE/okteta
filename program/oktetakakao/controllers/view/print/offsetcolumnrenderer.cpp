/*
    This file is part of the Okteta Kakao module, part of the KDE project.

    Copyright 2003,2008 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "offsetcolumnrenderer.h"

// Qt
#include <QtGui/QPainter>


namespace KHEPrint
{

OffsetColumnRenderer::OffsetColumnRenderer( int firstLineOffset, int delta, KOffsetFormat::KFormat format )
 : AbstractColumnRenderer(),
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

}
