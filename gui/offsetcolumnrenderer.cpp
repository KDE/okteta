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
#include "bytearraytablelayout.h"
#include <columnsview.h>
// Qt
#include <QtGui/QPainter>


namespace KHEUI
{

static const int leftOffsetMargin = 2;
static const int rightOffsetMargin = 2;


OffsetColumnRenderer::OffsetColumnRenderer( ColumnsView* columnsView,
    ByteArrayTableLayout* layout, KOffsetFormat::KFormat format )
 : AbstractColumnRenderer( columnsView ),
   mLayout( layout ),
   mDigitWidth( 0 ),
   mDigitBaseLine( 0 ),
   mFormat( KOffsetFormat::None )
{
    setFormat( format );
}

void OffsetColumnRenderer::renderLine( QPainter* painter, int lineIndex )
{
    const QWidget *viewport = columnsView()->viewport();

    const int offset = mLayout->firstLineOffset() + mLayout->noOfBytesPerLine() * lineIndex;
    printFunction()( mCodedOffset, offset );

    const QColor &buttonColor = viewport->palette().buttonText().color();
    painter->setPen( buttonColor );
    painter->drawText( leftOffsetMargin, mDigitBaseLine, QString().append(mCodedOffset) );
}

void OffsetColumnRenderer::renderColumnBackground( QPainter* painter, const KPixelXs& _Xs, const KPixelYs& Ys )
{
    KPixelXs Xs( _Xs );
    restrictToXSpan( &Xs );

    const QBrush& buttonBrush = columnsView()->viewport()->palette().button();
    painter->fillRect( Xs.start(), Ys.start(), Xs.width(), Ys.width(), buttonBrush );
}

void OffsetColumnRenderer::renderFirstLine( QPainter *painter, const KPixelXs &, int firstLineIndex )
{
    mRenderLineIndex = firstLineIndex;
    renderLine( painter, mRenderLineIndex++ );
}


void OffsetColumnRenderer::renderNextLine( QPainter *painter )
{
    renderLine( painter, mRenderLineIndex++ );
}


void OffsetColumnRenderer::renderColumn( QPainter* painter, const KPixelXs& Xs, const KPixelYs& Ys )
{
    renderColumnBackground( painter, Xs, Ys );
}


void OffsetColumnRenderer::renderEmptyColumn( QPainter* painter, const KPixelXs& Xs, const KPixelYs& Ys )
{
    renderColumnBackground( painter, Xs, Ys );
}

void OffsetColumnRenderer::setFormat( KOffsetFormat::KFormat format )
{
    // no changes?
    if( mFormat == format )
        return;

    mFormat = format;

    mCodingWidth = KOffsetFormat::codingWidth( mFormat );
    PrintFunction = KOffsetFormat::printFunction( mFormat );

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
    setWidth( mCodingWidth * mDigitWidth + leftOffsetMargin + rightOffsetMargin );
}


OffsetColumnRenderer::~OffsetColumnRenderer() {}

}
