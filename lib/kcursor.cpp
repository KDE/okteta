/***************************************************************************
                          kcursor.cpp  -  description
                             -------------------
    begin                : Mit Jun 26 2003
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


#include "kcursor.h"

using namespace KHE;

KCursor::KCursor()
: CursorX( 0 ),
  CursorW( -1 )
{
}


KCursor::~KCursor()
{
}



void KCursor::setSize( KPixelX Width, KPixelY Height )
{
  if( Width != OnPixmap.width() || Height != OnPixmap.height() )
  {
    OnPixmap.resize( Width, Height );
    OffPixmap.resize( Width, Height );
  }
}


void KCursor::setShape( KPixelX X, KPixelX W )
{
  CursorX = X;
  CursorW = W;
}
