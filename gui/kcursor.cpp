/***************************************************************************
                          kcursor.cpp  -  description
                             -------------------
    begin                : Mit Jun 26 2003
    copyright            : (C) 2003 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License version 2 as published by the Free Software Foundation.       *
 *                                                                         *
 ***************************************************************************/


#include "kcursor.h"


namespace KHEUI {

KCursor::KCursor()
: CursorX( 0 ),
  CursorW( -1 )
{
}


KCursor::~KCursor() {}


void KCursor::setSize( KPixelX Width, KPixelY Height )
{
  if( Width != OnPixmap.width() || Height != OnPixmap.height() )
  {
    OnPixmap = QPixmap( Width, Height );
    OffPixmap = QPixmap( Width, Height );
  }
}


void KCursor::setShape( KPixelX X, KPixelX W )
{
  CursorX = X;
  CursorW = W;
}

}
