/***************************************************************************
                          charcolumnrenderer.cpp  -  description
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


#include "charcolumnrenderer.h"

// lib
#include "abstractcolumnframerenderer.h"
// Okteta ui
#include <kdatalayout.h>
// Qt
#include <QtGui/QPainter>


static const bool     DefaultShowsNonprinting = false;
static const char     DefaultSubstituteChar =   (char)'.';
static const char     DefaultUndefinedChar =    (char)'?';


CharColumnRenderer::CharColumnRenderer( AbstractColumnFrameRenderer *columnFrameRenderer,
                                        const KHECore::KAbstractByteArrayModel *byteArrayModel,
                                        const KHEUI::KDataLayout *layout )
 : ByteColumnRenderer( columnFrameRenderer, byteArrayModel, layout ),
   mShowsNonprinting( DefaultShowsNonprinting ),
   mSubstituteChar( DefaultSubstituteChar ),
   mUndefinedChar( DefaultUndefinedChar )
{
    setSpacing( 0, 0, 0 );
}



void CharColumnRenderer::drawByte( QPainter *painter, char /*byte*/, KHECore::KChar byteChar, const QColor &color ) const
{
    // turn into a drawable String
    QString byteString( byteChar.isUndefined() ?                     KHECore::KChar(mUndefinedChar) :
                        !(mShowsNonprinting || byteChar.isPrint()) ? KHECore::KChar(mSubstituteChar) :
                        byteChar );

    painter->setPen( color );
    painter->drawText( 0, mDigitBaseLine, byteString );
}


CharColumnRenderer::~CharColumnRenderer() {}
