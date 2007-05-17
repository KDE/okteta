/***************************************************************************
                          kbordercolumn.h  -  description
                             -------------------
    begin                : Mit Mai 21 2003
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


#ifndef KHE_UI_KBORDERCOLUMN_H
#define KHE_UI_KBORDERCOLUMN_H


// lib
#include "kcolumn.h"


namespace KHEUI
{

/** column that does nothing but draw a vertical line in the middle of the column
  *
  *@author Friedrich W. H. Kossebau
  */

class KBorderColumn : public KColumn
{
  public:
    KBorderColumn( KColumnsView *V, bool M );
    ~KBorderColumn();

  public: // KColumn-API
    virtual void paintFirstLine( QPainter *P, const KPixelXs &Xs, int FirstLine );
    virtual void paintNextLine( QPainter *P );
    virtual void paintEmptyColumn( QPainter *P, const KPixelXs &Xs, const KPixelYs &Ys );

  protected:
   virtual void paintLine( QPainter *P );

  protected:
    /** true if we are between two columns and should show a line */
    bool Middle;
};

}

#endif
