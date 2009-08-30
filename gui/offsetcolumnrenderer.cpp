/*
    This file is part of the Okteta Gui library, part of the KDE project.

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

// lib
#include <abstractcolumnstylist.h>
// lib
#include "bytearraytablelayout.h"
// Qt
#include <QtGui/QPainter>
#include <QtGui/QPalette>


namespace Okteta
{

static const int leftOffsetMargin = 2;
static const int rightOffsetMargin = 2;


OffsetColumnRenderer::OffsetColumnRenderer( AbstractColumnStylist* stylist,
    ByteArrayTableLayout* layout, OffsetFormat::Format format )
 : AbstractColumnRenderer( stylist ),
   mLayout( layout ),
   mDigitWidth( 0 ),
   mDigitBaseLine( 0 ),
   mFormat( OffsetFormat::None )
{
    setFormat( format );
}

void OffsetColumnRenderer::renderLine( QPainter* painter, int lineIndex )
{
    const int offset = mLayout->firstLineOffset() + mLayout->noOfBytesPerLine() * lineIndex;
    printFunction()( mCodedOffset, offset );

    const QColor& buttonColor = stylist()->palette().buttonText().color();
    painter->setPen( buttonColor );
    painter->drawText( leftOffsetMargin, mDigitBaseLine, QString().append(mCodedOffset) );
}

void OffsetColumnRenderer::renderColumnBackground( QPainter* painter, const PixelXs& _Xs, const PixelYs& Ys )
{
    PixelXs Xs( _Xs );
    restrictToXSpan( &Xs );

    const QBrush& buttonBrush = stylist()->palette().button();
    painter->fillRect( Xs.start(), Ys.start(), Xs.width(), Ys.width(), buttonBrush );
}

void OffsetColumnRenderer::renderFirstLine( QPainter *painter, const PixelXs &, int firstLineIndex )
{
    mRenderLineIndex = firstLineIndex;
    renderLine( painter, mRenderLineIndex++ );
}


void OffsetColumnRenderer::renderNextLine( QPainter *painter )
{
    renderLine( painter, mRenderLineIndex++ );
}


void OffsetColumnRenderer::renderColumn( QPainter* painter, const PixelXs& Xs, const PixelYs& Ys )
{
    renderColumnBackground( painter, Xs, Ys );
}


void OffsetColumnRenderer::renderEmptyColumn( QPainter* painter, const PixelXs& Xs, const PixelYs& Ys )
{
    renderColumnBackground( painter, Xs, Ys );
}

void OffsetColumnRenderer::setFormat( OffsetFormat::Format format )
{
    // no changes?
    if( mFormat == format )
        return;

    mFormat = format;

    mCodingWidth = OffsetFormat::codingWidth( mFormat );
    PrintFunction = OffsetFormat::printFunction( mFormat );

    recalcX();
}

void OffsetColumnRenderer::setMetrics( PixelX digitWidth, PixelY digitBaseLine )
{
    mDigitBaseLine = digitBaseLine;
    setDigitWidth( digitWidth );
}

void OffsetColumnRenderer::setDigitWidth( PixelX digitWidth )
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
    setWidth( mCodingWidth * mDigitWidth + leftOffsetMargin + rightOffsetMargin );
}


OffsetColumnRenderer::~OffsetColumnRenderer() {}

}
