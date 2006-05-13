/***************************************************************************
                          kbordercolumn.cpp  -  description
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
#include <QPainter>
#include <QStyle>
// lib specific
#include "kcolumnsview.h"
#include "kbordercolumn.h"


using namespace KHE;

static const KPixelX DefaultWidth = 9;
static const KPixelX LineX = DefaultWidth / 2;


KBorderColumn::KBorderColumn( KColumnsView *V, bool M )
 : KColumn( V ),
   Middle( M )
{
  setWidth( M?DefaultWidth:LineX-1 );
}

KBorderColumn::~KBorderColumn()
{
}


void KBorderColumn::paintLine( QPainter *Painter )
{
  if( LineHeight > 0 )
  {
    KColumn::paintBlankLine( Painter );

    if( Middle )
    {
      const QWidget *Viewport = View->viewport();
      int GridColor = Viewport->style()->styleHint( QStyle::SH_Table_GridLineColor, 0, Viewport );

      Painter->setPen( GridColor != -1 ? (QRgb)GridColor : Viewport->palette().mid().color() );
      Painter->drawLine( LineX, 0, LineX, LineHeight-1 ) ;
    }
  }
}


void KBorderColumn::paintFirstLine( QPainter *Painter, KPixelXs , int )
{
  paintLine( Painter );
}


void KBorderColumn::paintNextLine( QPainter *Painter )
{
  paintLine( Painter );
}

void KBorderColumn::paintEmptyColumn( QPainter *Painter, KPixelXs Xs, KPixelYs Ys )
{
  KColumn::paintEmptyColumn( Painter, Xs,Ys );

  KPixelX LX = x() + LineX;

  if( Middle && Xs.includes(LX) )
  {
    const QWidget *Viewport = View->viewport();
    int GridColor = Viewport->style()->styleHint( QStyle::SH_Table_GridLineColor, 0, Viewport );

    Painter->setPen( GridColor != -1 ? (QRgb)GridColor : Viewport->palette().mid().color() );
    Painter->drawLine( LX, Ys.start(), LX, Ys.end() ) ;
  }
}
