/***************************************************************************
                          valuecolumnrenderer.cpp  -  description
                             -------------------
    begin                : Mit Sep 3 2003
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


#include "valuecolumnrenderer.h"

// lib
#include "abstractcolumnframerenderer.h"
// Okteta ui
#include <kdatalayout.h>
#include <helper.h>
// Okteta core
#include <kcharcodec.h>
// Qt
#include <QtGui/QPainter>


static const KHECore::KCoding DefaultCoding =    KHECore::HexadecimalCoding;
static const KHECore::KCoding NotDefaultCoding = KHECore::DecimalCoding;

static const int DefaultBinaryGapWidth = 1;


ValueColumnRenderer::ValueColumnRenderer( AbstractColumnFrameRenderer *columnFrameRenderer,
                                          const KHECore::KAbstractByteArrayModel *byteArrayModel,
                                          const KHEUI::KDataLayout *layout )
 : ByteColumnRenderer( columnFrameRenderer, byteArrayModel, layout ),
   mCoding( NotDefaultCoding ),
   mByteCodec( 0 ),
   mBinaryGapWidth( DefaultBinaryGapWidth )
{
    setCoding( DefaultCoding );
}


bool ValueColumnRenderer::setCoding( KHECore::KCoding coding )
{
    // no changes?
    if( mCoding == coding )
        return false;

    delete mByteCodec;

    mCoding = coding;
    mByteCodec = KHECore::KByteCodec::createCodec( mCoding );
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
