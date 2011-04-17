/*
    This file is part of the Okteta Gui library, made within the KDE community.

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

#include "kcursor.h"

#ifdef __GNUC__
// Will remove this again very soon, Alex
#warning This warning is just for testing LABELS in cmake/cdash
#endif

namespace Okteta {

KCursor::KCursor()
: CursorX( 0 ),
  CursorW( -1 )
{
}


KCursor::~KCursor() {}


void KCursor::setSize( PixelX Width, PixelY Height )
{
  if( Width != OnPixmap.width() || Height != OnPixmap.height() )
  {
    OnPixmap = QPixmap( Width, Height );
    OffPixmap = QPixmap( Width, Height );
  }
}


void KCursor::setShape( PixelX X, PixelX W )
{
  CursorX = X;
  CursorW = W;
}

}
