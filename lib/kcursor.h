/***************************************************************************
                          kbuffercolumn.h  -  description
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

 
#ifndef KHE_KCURSOR_H
#define KHE_KCURSOR_H


#include <qpixmap.h>

#include "kadds.h"

namespace KHE
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
    void setSize( KPixelX Width, KPixelY Height );
    /** sets the shape of the cursor to be drawn */
    void setShape( KPixelX X, KPixelX W );

  public: // access
    const QPixmap &onPixmap() const;
    const QPixmap &offPixmap() const;
    KPixelX cursorX() const;
    KPixelX cursorW() const;


  protected:
    QPixmap OnPixmap;
    QPixmap OffPixmap;

    KPixelX CursorX;
    KPixelX CursorW;
};


inline const QPixmap &KCursor::onPixmap()  const { return OnPixmap; }
inline const QPixmap &KCursor::offPixmap() const { return OffPixmap; }

inline KPixelX KCursor::cursorX() const { return CursorX; }
inline KPixelX KCursor::cursorW() const { return CursorW; }

}

#endif
