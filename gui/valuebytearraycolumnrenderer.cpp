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

#include "valuebytearraycolumnrenderer.h"

// lib
#include "columnsview.h"
#include "kdatacursor.h"
#include "bytearraytablelayout.h"
#include "kdataranges.h"
#include "helper.h"
// Okteta core
#include <kcharcodec.h>
// KDE
#include <KColorScheme>
// Qt
#include <QtGui/QPainter>


namespace KHEUI
{

static const KHECore::KCoding DefaultValueCoding =    KHECore::HexadecimalCoding;
static const KHECore::KCoding NotDefaultCoding = KHECore::DecimalCoding;

static const int DefaultBinaryGapWidth = 1;


ValueByteArrayColumnRenderer::ValueByteArrayColumnRenderer( ColumnsView *columnsView,
    KHECore::KAbstractByteArrayModel *byteArrayModel, ByteArrayTableLayout *layout, KDataRanges *ranges )
 : AbstractByteArrayColumnRenderer( columnsView, byteArrayModel, layout, ranges ),
   mValueCoding( NotDefaultCoding ),
   mValueCodec( 0 ),
   mBinaryGapWidth( DefaultBinaryGapWidth )
{
    setValueCoding( DefaultValueCoding );
}


bool ValueByteArrayColumnRenderer::setValueCoding( KHECore::KCoding valueCoding )
{
    // no changes?
    if( mValueCoding == valueCoding )
        return false;

    delete mValueCodec;

    mValueCoding = valueCoding;
    mValueCodec = KHECore::ValueCodec::createCodec( mValueCoding );
    mDecodedByteText.resize( mValueCodec->encodingWidth() );

    // recalculate depend sizes
    recalcByteWidth();

    if( mLinePosLeftPixelX )
        recalcX();
    return true;
}


bool ValueByteArrayColumnRenderer::setBinaryGapWidth( KPixelX binaryGapWidth )
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
    int byteWidth = mValueCodec->encodingWidth() * mDigitWidth;

    if( mValueCoding == KHECore::BinaryCoding )
    {
        mBinaryHalfOffset = 4 * mDigitWidth + mBinaryGapWidth;
        byteWidth += mBinaryGapWidth;
    }
    setByteWidth( byteWidth );
}


// perhaps sometimes there will be a grammar
void ValueByteArrayColumnRenderer::renderEditedByte( QPainter *painter, char byte, const QString &EditBuffer )
{
    const KHECore::KChar byteChar = mCharCodec->decode( byte );

    const QPalette &palette = columnsView()->viewport()->palette();
    KColorScheme colorScheme( palette.currentColorGroup(), KColorScheme::View );
    const KColorScheme::ForegroundRole foregroundRole =
        mByteTypeColored ? foregroundRoleForChar(byteChar): KColorScheme::NormalText;
    const QBrush brush = colorScheme.foreground( foregroundRole );
    painter->fillRect( 0,0, byteWidth(),lineHeight(), brush );

    const QBrush backgroundBrush = colorScheme.background();
    const QColor &charColor = backgroundBrush.color();
    renderCode( painter, EditBuffer, charColor );
}


void ValueByteArrayColumnRenderer::renderByteText( QPainter *painter,
                                                   char byte, KHECore::KChar byteChar, const QColor &color ) const
{
Q_UNUSED( byteChar )

    mValueCodec->encode( mDecodedByteText, 0, byte );
    renderCode( painter, mDecodedByteText, color );
}


void ValueByteArrayColumnRenderer::renderCode( QPainter *painter, const QString &code, const QColor &color ) const
{
    painter->setPen( color );
    if( mValueCoding == KHECore::BinaryCoding )
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
    delete mValueCodec;
}

}
