/***************************************************************************
                          bordercolumnrenderer.h  -  description
                             -------------------
    begin                : Mit Mai 21 2003
    copyright            : 2003 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License version 2 as published by the Free Software Foundation.       *
 *                                                                         *
 ***************************************************************************/


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
