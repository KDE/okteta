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

  
#include <qpainter.h>

#include "kcolumnsview.h"
#include "kbordercolumn.h"


using namespace KHE;

static const KPixelX DefaultWidth = 9;
static const KPixelX LineX = DefaultWidth / 2;


KBorderColumn::KBorderColumn( KColumnsView *V )
 : KColumn( V )
{
  setWidth( DefaultWidth );
}

KBorderColumn::~KBorderColumn()
{
}


inline void KBorderColumn::paintLine( QPainter *P )
{
  if( LineHeight > 0 )
  {
    KColumn::paintBlankLine( P );

    P->drawLine( LineX, 0, LineX, LineHeight-1 ) ;
  }
}


void KBorderColumn::paintFirstLine( QPainter *P, KPixelX , KPixelX , int )
{
  paintLine( P );
}


void KBorderColumn::paintNextLine( QPainter *P )
{
  paintLine( P );
}
