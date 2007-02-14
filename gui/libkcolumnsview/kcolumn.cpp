/***************************************************************************
                          kcolumn.cpp  -  description
                             -------------------
    begin                : Mit Mai 21 2003
    copyright            : (C) 2003 by Friedrich W. H. Kossebau
    email                : Friedrich.W.H@Kossebau.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License version 2 as published by the Free Software Foundation.       *
 *                                                                         *
 ***************************************************************************/


// qt specific
#include <QtGui/QPainter>
// lib specific
#include "kcolumnsview.h"
#include "kcolumn.h"


namespace KHEUI {

KColumn::KColumn( KColumnsView *V )
 : View( V ),
   Visible( true ),  //TODO: would false be better?
   LineHeight( V->lineHeight() ),
   XSpan( KHE::KSection::fromWidth(0,0) )
{
  V->addColumn( this );
}


void KColumn::paintFirstLine( QPainter *Painter, const KPixelXs &, int /*FirstLine*/ )
{
  paintBlankLine( Painter );
}


void KColumn::paintNextLine( QPainter *Painter )
{
  paintBlankLine( Painter );
}


void KColumn::paintBlankLine( QPainter *Painter ) const
{
  if( LineHeight > 0 )
  {
      const QWidget *Viewport = View->viewport();
      Painter->fillRect( 0,0, width(),LineHeight,
                         Viewport->palette().brush(Viewport->backgroundRole()) );
  }
}


void KColumn::paintEmptyColumn( QPainter *Painter, const KPixelXs &_Xs, const KPixelYs &Ys )
{
  KPixelXs Xs( _Xs );
  Xs.restrictTo( XSpan );

  const QWidget *Viewport = View->viewport();
  Painter->fillRect( Xs.start(),Ys.start(), Xs.width(),Ys.width(),
                     Viewport->palette().brush(Viewport->backgroundRole()) );
}

}
