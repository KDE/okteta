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
#include "QtGui/QPainter"
// lib specific
#include "kcolumnsview.h"
#include "koffsetcolumn.h"


namespace KHEUI {

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
  const QWidget *Viewport = columnsView()->viewport();

  const QBrush &ButtonBrush = Viewport->palette().button();
  P->fillRect( 0,0, width(),lineHeight(), ButtonBrush );

  printFunction()( CodedOffset,FirstLineOffset+Delta*Line );

  const QColor &ButtonColor = Viewport->palette().buttonText().color();
  P->setPen( ButtonColor );
  P->drawText( 0, DigitBaseLine, QString().append(CodedOffset) );
}


void KOffsetColumn::paintFirstLine( QPainter *P, const KPixelXs &, int FirstLine )
{
  PaintLine = FirstLine;
  paintLine( P, PaintLine++ );
}


void KOffsetColumn::paintNextLine( QPainter *P )
{
  paintLine( P, PaintLine++ );
}



void KOffsetColumn::paintEmptyColumn( QPainter *P, const KPixelXs &_Xs, const KPixelYs &Ys )
{
  KPixelXs Xs( _Xs );
  restrictToXSpan( &Xs );

  const QBrush &ButtonBrush = columnsView()->viewport()->palette().button();
  P->fillRect( Xs.start(), Ys.start(), Xs.width(), Ys.width(), ButtonBrush );
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

}
