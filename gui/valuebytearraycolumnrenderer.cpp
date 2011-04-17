/*
    This file is part of the Okteta Gui library, made within the KDE community.

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

#include "valuebytearraycolumnrenderer.h"

// lib
#include "helper.h"
#include "oktetagui.h"
// lib
#include <abstractcolumnstylist.h>
// Okteta core
#include <valuecodec.h>
#include <charcodec.h>
// KDE
#include <KColorScheme>
// Qt
#include <QtGui/QPainter>


namespace Okteta
{


ValueByteArrayColumnRenderer::ValueByteArrayColumnRenderer( AbstractColumnStylist* stylist,
    AbstractByteArrayModel* byteArrayModel, ByteArrayTableLayout* layout, ByteArrayTableRanges* ranges )
 : AbstractByteArrayColumnRenderer( stylist, byteArrayModel, layout, ranges ),
   mValueCodec( 0 ),
   mBinaryGapWidth( DefaultBinaryGapWidth )
{
}

void ValueByteArrayColumnRenderer::setValueCodec( ValueCoding valueCoding, const ValueCodec* valueCodec )
{
    mValueCoding = valueCoding;
    mValueCodec = valueCodec;
    mDecodedByteText.resize( mValueCodec->encodingWidth() );

    // recalculate depend sizes
    recalcByteWidth();

    if( mLinePosLeftPixelX )
        recalcX();
}


bool ValueByteArrayColumnRenderer::setBinaryGapWidth( PixelX binaryGapWidth )
{
    // no changes?
    if( mBinaryGapWidth == binaryGapWidth )
        return false;

    mBinaryGapWidth = binaryGapWidth;

    // recalculate depend sizes
    recalcByteWidth();

    if( mLinePosLeftPixelX )
        recalcX();
    return true;
}


void ValueByteArrayColumnRenderer::recalcByteWidth()
{
    // use 0 as reference, using a fixed font should always yield same width
    mValueCodec->encode( mDecodedByteText, 0, Byte(0) );
    if( mValueCoding == BinaryCoding )
    {
        const int binaryHalfWidth = mFontMetrics.width( mDecodedByteText.left(4) );
        mBinaryHalfOffset = binaryHalfWidth + mBinaryGapWidth;
        setByteWidth( mBinaryHalfOffset + binaryHalfWidth );
    }
    else
        setByteWidth( mFontMetrics.width(mDecodedByteText) );
}


// perhaps sometimes there will be a grammar
void ValueByteArrayColumnRenderer::renderEditedByte( QPainter* painter, Byte byte, const QString& editBuffer )
{
    const Character byteChar = mCharCodec->decode( byte );

    const QPalette& palette = stylist()->palette();
    KColorScheme colorScheme( palette.currentColorGroup(), KColorScheme::View );
    const KColorScheme::ForegroundRole foregroundRole =
        mByteTypeColored ? foregroundRoleForChar(byteChar): KColorScheme::NormalText;
    const QBrush brush = colorScheme.foreground( foregroundRole );
    painter->fillRect( 0,0, byteWidth(),lineHeight(), brush );

    const QBrush backgroundBrush = colorScheme.background();
    const QColor &charColor = backgroundBrush.color();
    renderCode( painter, editBuffer, charColor );
}


void ValueByteArrayColumnRenderer::renderByteText( QPainter* painter,
                                                   Byte byte, Character byteChar, const QColor& color ) const
{
Q_UNUSED( byteChar )

    mValueCodec->encode( mDecodedByteText, 0, byte );
    renderCode( painter, mDecodedByteText, color );
}


void ValueByteArrayColumnRenderer::renderCode( QPainter *painter, const QString &code, const QColor &color ) const
{
    painter->setPen( color );
    if( mValueCoding == Okteta::BinaryCoding )
    {
        // leave a gap in the middle
        painter->drawText( 0, mDigitBaseLine, code.left(4) );
        painter->drawText( mBinaryHalfOffset, mDigitBaseLine, code.right(4) );
    }
    else
        painter->drawText( 0, mDigitBaseLine, code );
}


ValueByteArrayColumnRenderer::~ValueByteArrayColumnRenderer()
{
}

}
