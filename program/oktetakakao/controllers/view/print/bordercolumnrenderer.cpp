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
