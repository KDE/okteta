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

// qt specific
#include <qstring.h>
// app specific
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
    /** sets whether "unprintable" chars (>32) should be displayed in the text column
      * with their corresponding character.
      * @param SU
      * returns true if there was a change
      */
    bool setShowUnprintable( bool SU = true );
    /** sets the substitute character for "unprintable" chars
      * returns true if there was a change
      */
    bool setSubstituteChar( QChar SC );

  public: // value access
    /** returns true if "unprintable" chars (>32) are displayed in the text column
      * with their corresponding character, default is false
      */
    bool showUnprintable() const;
    /** returns the actually used substitute character for "unprintable" chars, default is '.' */
    QChar substituteChar() const;


  protected: // KBufferColumn API
    virtual void drawByte( QPainter *P, char Byte, const QColor &Color ) const;

  protected:
    /** */
    bool ShowUnprintable;
    /** */
    QChar SubstituteChar;
};


inline bool KTextColumn::showUnprintable()  const { return ShowUnprintable; }
inline QChar KTextColumn::substituteChar()  const { return SubstituteChar; }

inline bool KTextColumn::setSubstituteChar( QChar SC )
{
  if( SubstituteChar == SC )
    return false;
  SubstituteChar = SC;
  return true;
}

inline bool KTextColumn::setShowUnprintable( bool SU )
{
  if( ShowUnprintable == SU )
    return false;
  ShowUnprintable = SU;
  return true;
}

}

#endif
