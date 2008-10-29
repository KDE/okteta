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

#include "valuecolumnrenderer.h"

// lib
#include "abstractcolumnframerenderer.h"
// Okteta gui
#include <helper.h>
// Okteta core
#include <kcharcodec.h>
// Qt
#include <QtGui/QPainter>


namespace KHEPrint
{

static const KHECore::ValueCoding DefaultCoding =    KHECore::HexadecimalCoding;
static const KHECore::ValueCoding NotDefaultCoding = KHECore::DecimalCoding;

static const int DefaultBinaryGapWidth = 1;


ValueColumnRenderer::ValueColumnRenderer( AbstractColumnFrameRenderer *columnFrameRenderer,
                                          const KHECore::KAbstractByteArrayModel *byteArrayModel,
                                          const KHE::KSection &renderIndizes,
                                          const KHEUI::ByteArrayTableLayout *layout )
 : ByteColumnRenderer( columnFrameRenderer, byteArrayModel, renderIndizes, layout ),
   mCoding( NotDefaultCoding ),
   mByteCodec( 0 ),
   mBinaryGapWidth( DefaultBinaryGapWidth )
{
    setValueCoding( DefaultCoding );
}


bool ValueColumnRenderer::setValueCoding( KHECore::ValueCoding valueCoding )
{
    // no changes?
    if( mCoding == valueCoding )
        return false;

    delete mByteCodec;

    mCoding = valueCoding;
    mByteCodec = KHECore::ValueCodec::createCodec( mCoding );
    mCodedByte.resize( mByteCodec->encodingWidth() );

    // recalculate depend sizes
    recalcByteWidth();

    if( mPosX )
        recalcX();
    return true;
}


bool ValueColumnRenderer::setBinaryGapWidth( KPixelX binaryGapWidth )
{
    // no changes?
    if( mBinaryGapWidth == binaryGapWidth )
        return false;

    mBinaryGapWidth = binaryGapWidth;

    // recalculate depend sizes
     recalcByteWidth();

    if( mPosX )
        recalcX();
    return true;
}


void ValueColumnRenderer::recalcByteWidth()
{
    mByteWidth = mByteCodec->encodingWidth() * mDigitWidth;

    if( mCoding == KHECore::BinaryCoding )
    {
        mBinaryHalfOffset = 4 * mDigitWidth + mBinaryGapWidth;
        mByteWidth += mBinaryGapWidth;
    }
}

void ValueColumnRenderer::drawByte( QPainter *painter, char byte, KHECore::KChar byteChar, const QColor &color ) const
{
Q_UNUSED( byteChar )
    mByteCodec->encode( mCodedByte, 0, byte );
    drawCode( painter, mCodedByte, color );
}


void ValueColumnRenderer::drawCode( QPainter *painter, const QString &code, const QColor &color ) const
{
    painter->setPen( color );
    if( mCoding == KHECore::BinaryCoding )
    {
        // leave a gap in the middle
        painter->drawText( 0, mDigitBaseLine, code.left(4) );
        painter->drawText( mBinaryHalfOffset, mDigitBaseLine, code.right(4) );
    }
    else
        painter->drawText( 0, mDigitBaseLine, code );
}

ValueColumnRenderer::~ValueColumnRenderer()
{
    delete mByteCodec;
}

}
