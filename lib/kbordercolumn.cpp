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
#include <qpainter.h>
#include <qstyle.h>
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


void KBorderColumn::paintLine( QPainter *P )
{
  if( LineHeight > 0 )
  {
    KColumn::paintBlankLine( P );

    if( Middle )
    {
      int GridColor = View->style().styleHint( QStyle::SH_Table_GridLineColor, View );
      P->setPen( GridColor != -1 ? (QRgb)GridColor : View->colorGroup().mid() );
      P->drawLine( LineX, 0, LineX, LineHeight-1 ) ;
    }
  }
}


void KBorderColumn::paintFirstLine( QPainter *P, KPixelXs , int )
{
  paintLine( P );
}


void KBorderColumn::paintNextLine( QPainter *P )
{
  paintLine( P );
}

void KBorderColumn::paintEmptyColumn( QPainter *P, KPixelXs Xs, KPixelYs Ys )
{
  KColumn::paintEmptyColumn( P,Xs,Ys );

  KPixelX LX = x() + LineX;
  if( Middle && Xs.includes(LX) )
  {
    int GridColor = View->style().styleHint( QStyle::SH_Table_GridLineColor, View );
    P->setPen( GridColor != -1 ? (QRgb)GridColor : View->colorGroup().mid() );
    P->drawLine( LX, Ys.start(), LX, Ys.end() ) ;
  }
}
