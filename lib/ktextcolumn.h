/***************************************************************************
                          ktextcolumn.h  -  description
                             -------------------
    begin                : Mit Sep 3 2003
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


#ifndef KTEXTCOLUMN_H
#define KTEXTCOLUMN_H

#include "kbuffercolumn.h"

class QPainter;
class QColor;

namespace KHE
{


/** base class of all buffer column displayers
  * holds all information about the vertical layout of a buffer column
  * knows how to paint the data and the editing things (focus, cursor, selection)
  * but does not offer
  *
  *@author Friedrich W. H. Kossebau
  */
class KTextColumn : public KBufferColumn
{
  public:
    KTextColumn( KColumnsView *CV, KDataBuffer *B, KBufferLayout *L, KBufferRanges *R );
    virtual ~KTextColumn();


  public: // modification access
    /** sets the spacing between the bytes in the hex column
      * @param ByteSpacingW spacing between the bytes in pixels
      * returns true if there was a change
      */
    bool setShowUnprintable( bool SU );
    void setSubstituteChar( char SC );

  public: // value access
    bool showUnprintable()  const;
    char substituteChar() const;


  protected: // KBufferColumn API
    virtual void drawByte( QPainter *P, char Byte, const QColor &Color ) const;

  protected:
    bool ShowUnprintable;
    char SubstituteChar;
};


inline bool KTextColumn::showUnprintable() const { return ShowUnprintable; }
inline char KTextColumn::substituteChar()  const { return SubstituteChar; }

inline void KTextColumn::setSubstituteChar( char SC ) { if( SC >= 32 ) SubstituteChar = SC; }

}

#endif
