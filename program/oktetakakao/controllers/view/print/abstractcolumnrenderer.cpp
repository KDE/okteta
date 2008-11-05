/*
    This file is part of the Okteta Kakao module, part of the KDE project.

    Copyright 2003,2007-2008 Friedrich W. H. Kossebau <kossebau@kde.org>

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

// lib
#include "abstractcolumnframerenderer.h"
// Qt
#include <QtGui/QPainter>

namespace KHEPrint
{

class AbstractColumnRendererPrivate
{
  public:
    AbstractColumnRendererPrivate( AbstractColumnFrameRenderer *columnFrameRenderer );
  public:
    void renderBlankLine( QPainter *painter ) const;
    void renderEmptyColumn( QPainter *painter, const KPixelXs &_Xs, const KPixelYs &Ys );

  public: // general column data
    /** pointer to the view */
    AbstractColumnFrameRenderer *mColumnFrameRenderer;
    /** should Column be displayed? */
    bool mVisible;

    /** buffered value */
    KPixelY mLineHeight;

    /** left offset x in pixel */
    KPixelXs mXSpan;
};

AbstractColumnRendererPrivate::AbstractColumnRendererPrivate( AbstractColumnFrameRenderer *columnFrameRenderer )
 : mColumnFrameRenderer( columnFrameRenderer ),
   mVisible( true ),  //TODO: would false be better?
   mLineHeight( columnFrameRenderer->lineHeight() ),
   mXSpan( KHE::Section::fromWidth(0,0) )
{
}

void AbstractColumnRendererPrivate::renderBlankLine( QPainter *painter ) const
{
    if( mLineHeight > 0 )
    {
        painter->fillRect( 0,0, mXSpan.width(),mLineHeight,
                           Qt::white );
    }
}


void AbstractColumnRendererPrivate::renderEmptyColumn( QPainter *painter, const KPixelXs &_Xs, const KPixelYs &Ys )
{
    KPixelXs Xs( _Xs );
    Xs.restrictTo( mXSpan );

    painter->fillRect( Xs.start(),Ys.start(), Xs.width(),Ys.width(),
                       Qt::white );
}


AbstractColumnRenderer::AbstractColumnRenderer( AbstractColumnFrameRenderer *columnFrameRenderer )
 : d( new AbstractColumnRendererPrivate(columnFrameRenderer) )
{
    columnFrameRenderer->addColumn( this );
}

AbstractColumnFrameRenderer *AbstractColumnRenderer::columnFrameRenderer() const { return d->mColumnFrameRenderer; }

KPixelX AbstractColumnRenderer::x()            const { return d->mXSpan.start(); }
KPixelX AbstractColumnRenderer::rightX()       const { return d->mXSpan.end(); }
KPixelX AbstractColumnRenderer::width()        const { return d->mXSpan.width(); }
bool    AbstractColumnRenderer::isVisible()    const { return d->mVisible; }
KPixelX AbstractColumnRenderer::visibleWidth() const { return d->mVisible ? d->mXSpan.width(): 0; }
KPixelY AbstractColumnRenderer::lineHeight()   const { return d->mLineHeight; }

void AbstractColumnRenderer::setX( KPixelX NewX )             { d->mXSpan.moveToStart( NewX ); }
void AbstractColumnRenderer::setWidth( KPixelX Width )        { d->mXSpan.setEndByWidth( Width ); }
void AbstractColumnRenderer::setVisible( bool mVisible )       { d->mVisible = mVisible; }
void AbstractColumnRenderer::setLineHeight( KPixelY Height )  { d->mLineHeight = Height; }

void AbstractColumnRenderer::restrictToXSpan( KPixelXs *Xs ) const { Xs->restrictTo( d->mXSpan ); }

bool AbstractColumnRenderer::overlaps( const KPixelXs &Xs ) const { return d->mXSpan.overlaps(Xs); }


void AbstractColumnRenderer::renderFirstLine( QPainter *painter, const KPixelXs &, int firstLine )
{
Q_UNUSED( painter )
Q_UNUSED( firstLine )
//     d->renderBlankLine( painter );
}

void AbstractColumnRenderer::renderNextLine( QPainter *painter )
{
Q_UNUSED( painter )
//     d->renderBlankLine( painter );
}

void AbstractColumnRenderer::renderBlankLine( QPainter *painter ) const
{
    d->renderBlankLine( painter );
}

void AbstractColumnRenderer::renderColumn( QPainter *painter, const KPixelXs &Xs, const KPixelYs &Ys )
{
    d->renderEmptyColumn( painter, Xs, Ys );
}

void AbstractColumnRenderer::renderEmptyColumn( QPainter *painter, const KPixelXs &Xs, const KPixelYs &Ys )
{
    d->renderEmptyColumn( painter, Xs, Ys );
}

AbstractColumnRenderer::~AbstractColumnRenderer()
{
    delete d;
}

}

