/***************************************************************************
                          koffsetcolumn.cpp  -  description
                             -------------------
    begin                : Mit Mai 14 2003
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
#include "qpainter.h"
// lib specific
#include "kcolumnsview.h"
#include "koffsetcolumn.h"

using namespace KHE;

KOffsetColumn::KOffsetColumn( KColumnsView *V, int FLO, int D, KOffsetFormat::KFormat F )
 : KColumn( V ),
   FirstLineOffset( FLO ),
   Delta( D ),
   Margin( 0 ),
   DigitWidth( 0 ),
   DigitBaseLine( 0 ),
   Format( KOffsetFormat::None )
{
  setFormat( F );
}


KOffsetColumn::~KOffsetColumn()
{
}


void KOffsetColumn::paintLine( QPainter *P, int Line )
{
  const QColor &ButtonColor = View->colorGroup().button();
  P->fillRect( 0,0,width(),LineHeight, QBrush(ButtonColor,Qt::SolidPattern) );

  printFunction()( CodedOffset,FirstLineOffset+Delta*Line );
  P->drawText( 0, DigitBaseLine, QString().append(CodedOffset) );
}


void KOffsetColumn::paintFirstLine( QPainter *P, KPixelXs, int FirstLine )
{
  PaintLine = FirstLine;
  paintLine( P, PaintLine++ );
}


void KOffsetColumn::paintNextLine( QPainter *P )
{
  paintLine( P, PaintLine++ );
}



void KOffsetColumn::paintEmptyColumn( QPainter *P, KPixelXs Xs, KPixelYs Ys )
{
  Xs.restrictTo( XSpan );

  const QColor &ButtonColor = View->colorGroup().button();
  P->fillRect( Xs.start(), Ys.start(), Xs.width(), Ys.width(), QBrush(ButtonColor,Qt::SolidPattern) );
}

void KOffsetColumn::setFormat( KOffsetFormat::KFormat F )
{
  // no changes?
  if( Format == F )
    return;

  Format = F;

  CodingWidth = KOffsetFormat::codingWidth( Format );
  PrintFunction = KOffsetFormat::printFunction( Format );

  recalcX();
}

void KOffsetColumn::setMetrics( KPixelX DW, KPixelY DBL )
{
  DigitBaseLine = DBL;
  setDigitWidth( DW );
}

void KOffsetColumn::setDigitWidth( KPixelX DW )
{
  // no changes?
  if( DigitWidth == DW )
    return;

  DigitWidth = DW;

  recalcX();
}

void KOffsetColumn::recalcX()
{
  // recalculate depend sizes
  setWidth( CodingWidth * DigitWidth );
}
