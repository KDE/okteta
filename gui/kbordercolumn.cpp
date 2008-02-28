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

#include "kbordercolumn.h"

// lib
#include "kcolumnsview.h"
// Qt
#include <QtGui/QPainter>
#include <QtGui/QStyle>


namespace KHEUI {

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
  if( lineHeight() > 0 )
  {
    KColumn::paintBlankLine( Painter );

    if( Middle )
    {
      const QWidget *Viewport = columnsView()->viewport();
      int GridColor = Viewport->style()->styleHint( QStyle::SH_Table_GridLineColor, 0, Viewport );

      Painter->setPen( GridColor != -1 ? (QRgb)GridColor : Viewport->palette().mid().color() );
      Painter->drawLine( LineX, 0, LineX, lineHeight()-1 ) ;
    }
  }
}


void KBorderColumn::paintFirstLine( QPainter *Painter, const KPixelXs &, int )
{
  paintLine( Painter );
}


void KBorderColumn::paintNextLine( QPainter *Painter )
{
  paintLine( Painter );
}

void KBorderColumn::paintEmptyColumn( QPainter *Painter, const KPixelXs &Xs, const KPixelYs &Ys )
{
  KColumn::paintEmptyColumn( Painter, Xs,Ys );

  KPixelX LX = x() + LineX;

  if( Middle && Xs.includes(LX) )
  {
    const QWidget *Viewport = columnsView()->viewport();
    int GridColor = Viewport->style()->styleHint( QStyle::SH_Table_GridLineColor, 0, Viewport );

    Painter->setPen( GridColor != -1 ? (QRgb)GridColor : Viewport->palette().mid().color() );
    Painter->drawLine( LX, Ys.start(), LX, Ys.end() ) ;
  }
}

}
