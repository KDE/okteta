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

#include "bordercolumnrenderer.h"

// lib
#include <abstractcolumnstylist.h>
// Qt
#include <QtGui/QPainter>
#include <QtGui/QStyle>


namespace Okteta
{

static const PixelX BorderMargin = 4;
static const PixelX LineWidth = 1;
static const PixelX BorderWidth = 2 * BorderMargin + LineWidth;
static const PixelX LineX = BorderMargin;


BorderColumnRenderer::BorderColumnRenderer( AbstractColumnStylist* stylist, bool lineDrawn, bool inEmpty )
  : AbstractColumnRenderer( stylist ),
    mLineDrawn( lineDrawn ),
    mInEmpty( inEmpty )
{
    setWidth( mLineDrawn ? BorderWidth : BorderMargin );
}

void BorderColumnRenderer::renderColumn( QPainter* painter, const PixelXRange& Xs, const PixelYRange& Ys )
{
    AbstractColumnRenderer::renderColumn( painter, Xs,Ys );

    renderBorderLine( painter, Xs,Ys );
}

void BorderColumnRenderer::renderEmptyColumn( QPainter *painter, const PixelXRange &Xs, const PixelYRange &Ys )
{
    AbstractColumnRenderer::renderEmptyColumn( painter, Xs,Ys );

    if( mInEmpty )
        renderBorderLine( painter, Xs,Ys );
}

void BorderColumnRenderer::renderBorderLine( QPainter* painter, const PixelXRange& Xs, const PixelYRange& Ys )
{
    const PixelX viewGlobalLineX = x() + LineX;

    if( mLineDrawn && Xs.includes(viewGlobalLineX) )
    {
        const int lineColor = -1; // TODO: viewport->style()->styleHint( QStyle::SH_Table_GridLineColor, 0, viewport );

        painter->setPen( lineColor != -1 ? (QRgb)lineColor : stylist()->palette().mid().color() );
        painter->drawLine( viewGlobalLineX, Ys.start(), viewGlobalLineX, Ys.end() ) ;
    }
}

BorderColumnRenderer::~BorderColumnRenderer() {}

}
