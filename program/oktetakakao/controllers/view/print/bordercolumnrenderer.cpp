/***************************************************************************
                          bordercolumnrenderer.cpp  -  description
                             -------------------
    begin                : Mit Mai 21 2003
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


#include "bordercolumnrenderer.h"

// lib
#include "abstractcolumnframerenderer.h"
// Qt
#include <QtGui/QPainter>


static const KPixelX DefaultWidth = 9;
static const KPixelX LineX = DefaultWidth / 2;


BorderColumnRenderer::BorderColumnRenderer( AbstractColumnFrameRenderer *columnFrameRenderer, bool middle, bool inEmpty )
 : AbstractColumnRenderer( columnFrameRenderer ),
   mMiddle( middle ), mInEmpty( inEmpty )
{
    setWidth( middle ? DefaultWidth : LineX-1 );
}

void BorderColumnRenderer::renderColumn( QPainter *painter, const KPixelXs &Xs, const KPixelYs &Ys )
{
    AbstractColumnRenderer::renderColumn( painter, Xs,Ys );

    const KPixelX lineX = x() + LineX;

    if( mMiddle && Xs.includes(lineX) )
    {
        painter->setPen( Qt::black );
        painter->drawLine( lineX, Ys.start(), lineX, Ys.end() ) ;
    }
}

void BorderColumnRenderer::renderEmptyColumn( QPainter *painter, const KPixelXs &Xs, const KPixelYs &Ys )
{
    AbstractColumnRenderer::renderEmptyColumn( painter, Xs,Ys );

    if( mInEmpty )
    {
        const KPixelX lineX = x() + LineX;

        if( mMiddle && Xs.includes(lineX) )
        {
            painter->setPen( Qt::black );
            painter->drawLine( lineX, Ys.start(), lineX, Ys.end() ) ;
        }
    }
}

BorderColumnRenderer::~BorderColumnRenderer() {}
