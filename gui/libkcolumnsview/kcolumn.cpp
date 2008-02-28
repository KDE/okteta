/*
    This file is part of the Okteta Gui library, part of the KDE project.

    Copyright 2003,2007 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "kcolumn.h"

// lib
#include "kcolumnsview.h"
// Qt
#include <QtGui/QPainter>


namespace KHEUI {

class KColumnPrivate
{
  public:
    KColumnPrivate( KColumnsView *V );
  public:
    void paintBlankLine( QPainter *Painter ) const;
    void paintEmptyColumn( QPainter *Painter, const KPixelXs &_Xs, const KPixelYs &Ys );

  public: // general column data
    /** pointer to the view */
    KColumnsView *View;
    /** should Column be displayed? */
    bool Visible;

    /** buffered value */
    KPixelY LineHeight;

    /** left offset x in pixel */
    KPixelXs XSpan;
};

KColumnPrivate::KColumnPrivate( KColumnsView *V )
 : View( V ),
   Visible( true ),  //TODO: would false be better?
   LineHeight( V->lineHeight() ),
   XSpan( KHE::KSection::fromWidth(0,0) )
{
}

void KColumnPrivate::paintBlankLine( QPainter *Painter ) const
{
    if( LineHeight > 0 )
    {
        const QWidget *Viewport = View->viewport();
        Painter->fillRect( 0,0, XSpan.width(),LineHeight,
                           Viewport->palette().brush(Viewport->backgroundRole()) );
    }
}


void KColumnPrivate::paintEmptyColumn( QPainter *Painter, const KPixelXs &_Xs, const KPixelYs &Ys )
{
    KPixelXs Xs( _Xs );
    Xs.restrictTo( XSpan );

    const QWidget *Viewport = View->viewport();
    Painter->fillRect( Xs.start(),Ys.start(), Xs.width(),Ys.width(),
                       Viewport->palette().brush(Viewport->backgroundRole()) );
}


KColumn::KColumn( KColumnsView *V )
 : d( new KColumnPrivate(V) )
{
    V->addColumn( this );
}

KColumnsView *KColumn::columnsView() const { return d->View; }

KPixelX KColumn::x()            const { return d->XSpan.start(); }
KPixelX KColumn::rightX()       const { return d->XSpan.end(); }
KPixelX KColumn::width()        const { return d->XSpan.width(); }
bool    KColumn::isVisible()    const { return d->Visible; }
KPixelX KColumn::visibleWidth() const { return d->Visible ? d->XSpan.width(): 0; }
KPixelY KColumn::lineHeight()   const { return d->LineHeight; }

void KColumn::setX( KPixelX NewX )             { d->XSpan.moveToStart( NewX ); }
void KColumn::setWidth( KPixelX Width )        { d->XSpan.setEndByWidth( Width ); }
void KColumn::setVisible( bool Visible )       { d->Visible = Visible; }
void KColumn::setLineHeight( KPixelY Height )  { d->LineHeight = Height; }

void KColumn::restrictToXSpan( KPixelXs *Xs ) const { Xs->restrictTo( d->XSpan ); }

bool KColumn::overlaps( const KPixelXs &Xs ) const { return d->XSpan.overlaps(Xs); }


void KColumn::paintFirstLine( QPainter *Painter, const KPixelXs &, int /*FirstLine*/ )
{
    d->paintBlankLine( Painter );
}

void KColumn::paintNextLine( QPainter *Painter )
{
    d->paintBlankLine( Painter );
}

void KColumn::paintBlankLine( QPainter *Painter ) const
{
    d->paintBlankLine( Painter );
}


void KColumn::paintEmptyColumn( QPainter *Painter, const KPixelXs &Xs, const KPixelYs &Ys )
{
    d->paintEmptyColumn( Painter, Xs, Ys );
}

KColumn::~KColumn()
{
    delete d;
}

}
