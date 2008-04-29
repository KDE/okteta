/*
    This file is part of the Okteta Gui library, part of the KDE project.

    Copyright 2003,2007,2008 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "columnrenderer.h"

// lib
#include "columnsview.h"
// Qt
#include <QtGui/QPainter>


namespace KHEUI {

class ColumnRendererPrivate
{
  public:
    ColumnRendererPrivate( ColumnsView *V );
  public:
    void renderBlankLine( QPainter *painter ) const;
    void renderEmptyColumn( QPainter *painter, const KPixelXs &_Xs, const KPixelYs &Ys );

  public: // general column data
    /** pointer to the view */
    ColumnsView *View;
    /** should Column be displayed? */
    bool Visible;

    /** buffered value */
    KPixelY LineHeight;

    /** left offset x in pixel */
    KPixelXs XSpan;
};

ColumnRendererPrivate::ColumnRendererPrivate( ColumnsView *V )
 : View( V ),
   Visible( true ),  //TODO: would false be better?
   LineHeight( V->lineHeight() ),
   XSpan( KHE::KSection::fromWidth(0,0) )
{
}

void ColumnRendererPrivate::renderBlankLine( QPainter *painter ) const
{
    if( LineHeight > 0 )
    {
        const QWidget *Viewport = View->viewport();
        painter->fillRect( 0,0, XSpan.width(),LineHeight,
                           Viewport->palette().brush(Viewport->backgroundRole()) );
    }
}


void ColumnRendererPrivate::renderEmptyColumn( QPainter *painter, const KPixelXs &_Xs, const KPixelYs &Ys )
{
    KPixelXs Xs( _Xs );
    Xs.restrictTo( XSpan );

    const QWidget *Viewport = View->viewport();
    painter->fillRect( Xs.start(),Ys.start(), Xs.width(),Ys.width(),
                       Viewport->palette().brush(Viewport->backgroundRole()) );
}


ColumnRenderer::ColumnRenderer( ColumnsView *V )
 : d( new ColumnRendererPrivate(V) )
{
    V->addColumn( this );
}

ColumnsView *ColumnRenderer::columnsView() const { return d->View; }

KPixelX ColumnRenderer::x()            const { return d->XSpan.start(); }
KPixelX ColumnRenderer::rightX()       const { return d->XSpan.end(); }
KPixelX ColumnRenderer::width()        const { return d->XSpan.width(); }
bool    ColumnRenderer::isVisible()    const { return d->Visible; }
KPixelX ColumnRenderer::visibleWidth() const { return d->Visible ? d->XSpan.width(): 0; }
KPixelY ColumnRenderer::lineHeight()   const { return d->LineHeight; }

void ColumnRenderer::setX( KPixelX NewX )             { d->XSpan.moveToStart( NewX ); }
void ColumnRenderer::setWidth( KPixelX Width )        { d->XSpan.setEndByWidth( Width ); }
void ColumnRenderer::setVisible( bool Visible )       { d->Visible = Visible; }
void ColumnRenderer::setLineHeight( KPixelY Height )  { d->LineHeight = Height; }

void ColumnRenderer::restrictToXSpan( KPixelXs *Xs ) const { Xs->restrictTo( d->XSpan ); }

bool ColumnRenderer::overlaps( const KPixelXs &Xs ) const { return d->XSpan.overlaps(Xs); }


void ColumnRenderer::renderFirstLine( QPainter *painter, const KPixelXs &Xs, int firstLineIndex )
{
Q_UNUSED( Xs )
Q_UNUSED( firstLineIndex )
    d->renderBlankLine( painter );
}

void ColumnRenderer::renderNextLine( QPainter *painter )
{
    d->renderBlankLine( painter );
}

void ColumnRenderer::renderBlankLine( QPainter *painter ) const
{
    d->renderBlankLine( painter );
}


void ColumnRenderer::renderEmptyColumn( QPainter *painter, const KPixelXs &Xs, const KPixelYs &Ys )
{
    d->renderEmptyColumn( painter, Xs, Ys );
}

ColumnRenderer::~ColumnRenderer()
{
    delete d;
}

}
