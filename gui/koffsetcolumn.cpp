/*
    This file is part of the Okteta Gui library, part of the KDE project.

    Copyright 2003 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "koffsetcolumn.h"

// lib
#include "kcolumnsview.h"
// Qt
#include <QtGui/QPainter>


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
