/*
    This file is part of the Okteta Kakao module, part of the KDE project.

    Copyright 2003,2008 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef BORDERCOLUMNRENDERER_H
#define BORDERCOLUMNRENDERER_H


// lib
#include "abstractcolumnrenderer.h"

/** column that does nothing but draw a vertical line in the middle of the column
  *
  *@author Friedrich W. H. Kossebau
  */

class BorderColumnRenderer : public AbstractColumnRenderer
{
  public:
    BorderColumnRenderer( AbstractColumnFrameRenderer *columnFrameRenderer, bool middle, bool inEmpty = false );
    ~BorderColumnRenderer();

  public: // AbstractColumnRenderer API
//     virtual void renderFirstLine( QPainter *painter, const KPixelXs &Xs, int firstLine );
//     virtual void renderNextLine( QPainter *painter );
    virtual void renderColumn( QPainter *painter, const KPixelXs &Xs, const KPixelYs &Ys );
    virtual void renderEmptyColumn( QPainter *painter, const KPixelXs &Xs, const KPixelYs &Ys );

  protected:
   void renderLine( QPainter *painter );

  protected:
    /** true if we are between two columns and should show a line */
    bool mMiddle;
    /** true if the line is also drawn in empty space */
    bool mInEmpty;
};

#endif
