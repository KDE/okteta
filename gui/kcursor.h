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

#ifndef OKTETA_KCURSOR_H
#define OKTETA_KCURSOR_H


// lib
#include "kadds.h"
// Qt
#include <QtGui/QPixmap>


namespace Okteta
{

/**
  *@author Friedrich W. H. Kossebau
  */
class KCursor
{
  public:
    KCursor();
    virtual ~KCursor();

  public:
    /** sets size of the full cursor */
    void setSize( PixelX Width, PixelY Height );
    /** sets the shape of the cursor to be drawn */
    void setShape( PixelX X, PixelX W );

  public: // access
    QPixmap &onPixmap();
    QPixmap &offPixmap();
    PixelX cursorX() const;
    PixelX cursorW() const;


  protected:
    QPixmap OnPixmap;
    QPixmap OffPixmap;

    PixelX CursorX;
    PixelX CursorW;
};


inline QPixmap &KCursor::onPixmap()  { return OnPixmap; }
inline QPixmap &KCursor::offPixmap() { return OffPixmap; }

inline PixelX KCursor::cursorX() const { return CursorX; }
inline PixelX KCursor::cursorW() const { return CursorW; }

}

#endif
