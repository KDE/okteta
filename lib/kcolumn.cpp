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
#include <qpainter.h>
// lib specific
#include "kcolumnsview.h"
#include "kcolumn.h"

using namespace KHE;


KColumn::KColumn( KColumnsView *V )
 : View( V ),
   Visible( true ),  //TODO: would false be better?
   LineHeight( V->lineHeight() ),
   XSpan( 0,0,true )
{
  V->addColumn( this );
}


void KColumn::paintFirstLine( QPainter *P, KPixelXs, int /*FirstLine*/ )
{
  paintBlankLine( P );
}


void KColumn::paintNextLine( QPainter *P )
{
  paintBlankLine( P );
}


void KColumn::paintBlankLine( QPainter *P ) const
{
  if( LineHeight > 0 )
    P->fillRect( 0,0,width(),LineHeight, View->backgroundBrush() );
}


void KColumn::paintEmptyColumn( QPainter *P, KPixelXs Xs, KPixelYs Ys )
{
  Xs.restrictTo( XSpan );
  P->fillRect( Xs.start(), Ys.start(), Xs.width(), Ys.width(), View->backgroundBrush() );
}
