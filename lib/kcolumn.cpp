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


#include "qpainter.h"

#include "kcolumnsview.h"
#include "kcolumn.h"

using namespace KHE;


KColumn::KColumn( KColumnsView *V )
 : View( V ),
   Visible( true ),  //TODO: would false be better?
   LineHeight( V->lineHeight() ),
   m_X( 0 ),
   m_RightX( 0 ),
   m_Width( 0 )
{
  V->addColumn( this );
}


void KColumn::paintFirstLine( QPainter *P, KPixelX , KPixelX, int /*FirstLine*/ )
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
  {
    const QBrush &Brush = View->backgroundBrush();
    P->fillRect( 0,0,width(),LineHeight, Brush );
  }
}
