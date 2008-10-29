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

#include "bordercolumnrenderer.h"

// lib
#include "columnsview.h"
// Qt
#include <QtGui/QPainter>
#include <QtGui/QStyle>


namespace KHEUI
{

static const KPixelX BorderMargin = 4;
static const KPixelX LineWidth = 1;
static const KPixelX BorderWidth = 2 * BorderMargin + LineWidth;
static const KPixelX LineX = BorderMargin;


BorderColumnRenderer::BorderColumnRenderer( ColumnsView *columnsView, bool lineDrawn )
 : ColumnRenderer( columnsView ),
   mLineDrawn( lineDrawn )
{
    setWidth( mLineDrawn ? BorderWidth : BorderMargin );
}

void BorderColumnRenderer::renderColumn( QPainter* painter, const KPixelXs& Xs, const KPixelYs& Ys )
{
    ColumnRenderer::renderColumn( painter, Xs,Ys );

    renderBorderLine( painter, Xs,Ys );
}

void BorderColumnRenderer::renderEmptyColumn( QPainter *painter, const KPixelXs &Xs, const KPixelYs &Ys )
{
    ColumnRenderer::renderEmptyColumn( painter, Xs,Ys );

    renderBorderLine( painter, Xs,Ys );
}

void BorderColumnRenderer::renderBorderLine( QPainter* painter, const KPixelXs& Xs, const KPixelYs& Ys )
{
    const KPixelX viewGlobalLineX = x() + LineX;

    if( mLineDrawn && Xs.includes(viewGlobalLineX) )
    {
        const QWidget* viewport = columnsView()->viewport();
        const int lineColor = viewport->style()->styleHint( QStyle::SH_Table_GridLineColor, 0, viewport );

        painter->setPen( lineColor != -1 ? (QRgb)lineColor : viewport->palette().mid().color() );
        painter->drawLine( viewGlobalLineX, Ys.start(), viewGlobalLineX, Ys.end() ) ;
    }
}

BorderColumnRenderer::~BorderColumnRenderer() {}

}
