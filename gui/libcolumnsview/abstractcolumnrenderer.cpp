/*
    This file is part of the Okteta Gui library, made within the KDE community.

    Copyright 2003,2007-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "abstractcolumnrenderer.h"
#include "abstractcolumnrenderer_p.h"

// lib
#include "columnsview.h"
// Qt
#include <QtGui/QPainter>


namespace Okteta
{

AbstractColumnRenderer::AbstractColumnRenderer( AbstractColumnStylist* stylist )
 : d( new AbstractColumnRendererPrivate(stylist) )
{
}

AbstractColumnStylist* AbstractColumnRenderer::stylist() const { return d->mStylist; }

PixelX AbstractColumnRenderer::x()            const { return d->mXSpan.start(); }
PixelX AbstractColumnRenderer::rightX()       const { return d->mXSpan.end(); }
PixelX AbstractColumnRenderer::width()        const { return d->mXSpan.width(); }
bool   AbstractColumnRenderer::isVisible()    const { return d->mIsVisible; }
PixelX AbstractColumnRenderer::visibleWidth() const { return d->mIsVisible ? d->mXSpan.width(): 0; }
PixelY AbstractColumnRenderer::lineHeight()   const { return d->mLineHeight; }

void AbstractColumnRenderer::setX( PixelX x )                    { d->mXSpan.moveToStart( x ); }
void AbstractColumnRenderer::setWidth( PixelX width )            { d->mXSpan.setEndByWidth( width ); }
void AbstractColumnRenderer::setVisible( bool isVisible )        { d->mIsVisible = isVisible; }
void AbstractColumnRenderer::setLineHeight( PixelY lineHeight )  { d->mLineHeight = lineHeight; }

void AbstractColumnRenderer::restrictToXSpan( PixelXRange* xSpan ) const { xSpan->restrictTo( d->mXSpan ); }

bool AbstractColumnRenderer::overlaps( const PixelXRange& xSpan ) const { return d->mXSpan.overlaps(xSpan); }


void AbstractColumnRenderer::renderFirstLine( QPainter* painter, const PixelXRange& xSpan, int firstLineIndex )
{
Q_UNUSED( xSpan )
Q_UNUSED( firstLineIndex )
Q_UNUSED( painter )
//     d->renderBlankLine( painter );
}

void AbstractColumnRenderer::renderNextLine( QPainter* painter )
{
Q_UNUSED( painter )
//     d->renderBlankLine( painter );
}

void AbstractColumnRenderer::renderBlankLine( QPainter* painter ) const
{
    d->renderBlankLine( painter );
}

void AbstractColumnRenderer::renderColumn( QPainter* painter, const PixelXRange& xSpan, const PixelYRange& ySpan )
{
    d->renderEmptyColumn( painter, xSpan, ySpan );
}

void AbstractColumnRenderer::renderEmptyColumn( QPainter* painter, const PixelXRange& xSpan, const PixelYRange& ySpan )
{
    d->renderEmptyColumn( painter, xSpan, ySpan );
}

AbstractColumnRenderer::~AbstractColumnRenderer()
{
    delete d;
}

}
