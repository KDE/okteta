/***************************************************************************
                          kbordercolumn.h  -  description
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


#ifndef KHE_KBORDERCOLUMN_H
#define KHE_KBORDERCOLUMN_H

#include "kcolumn.h"

namespace KHE
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
    virtual void paintFirstLine( QPainter *P, KPixelXs Xs, int FirstLine );
    virtual void paintNextLine( QPainter *P );
    virtual void paintEmptyColumn( QPainter *P, KPixelXs Xs, KPixelYs Ys );

  protected:
   virtual void paintLine( QPainter *P );

  protected:
    /** true if we are between two columns and should show a line */
    bool Middle;
};

}

#endif
