/***************************************************************************
                          kcharcolumn.h  -  description
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


#ifndef KHE_KCHARCOLUMN_H
#define KHE_KCHARCOLUMN_H

// qt specific
#include <qstring.h>
// lib specific
#include "kbuffercolumn.h"

class QPainter;
class QColor;


namespace KHE
{


/** buffer column that interprets the bytes as chars
  *
  *@author Friedrich W. H. Kossebau
  */
class KCharColumn : public KBufferColumn
{
  public:
    KCharColumn( KColumnsView *CV, KDataBuffer *B, KBufferLayout *L, KBufferRanges *R );
    virtual ~KCharColumn();


  public: // modification access
    /** sets whether "unprintable" chars (>32) should be displayed in the char column
      * with their corresponding character.
      * @param SU
      * returns true if there was a change
      */
    bool setShowUnprintable( bool SU = true );
    /** sets the substitute character for "unprintable" chars
      * returns true if there was a change
      */
    bool setSubstituteChar( QChar SC );
    /** sets the undefined character for "undefined" chars
      * returns true if there was a change
      */
    bool setUndefinedChar( QChar UC );


  public: // value access
    /** returns true if "unprintable" chars (>32) are displayed in the char column
      * with their corresponding character, default is false
      */
    bool showUnprintable() const;
    /** returns the actually used substitute character for "unprintable" chars, default is '.' */
    QChar substituteChar() const;
    /** returns the actually used undefined character for "undefined" chars, default is '?' */
    QChar undefinedChar() const;


  protected: // KBufferColumn API
    virtual void drawByte( QPainter *P, char Byte, KHEChar B, const QColor &Color ) const;

  protected:
    /** */
    bool ShowUnprintable;
    /** */
    QChar SubstituteChar;
    /** */
    QChar UndefinedChar;
};


inline bool KCharColumn::showUnprintable()  const { return ShowUnprintable; }
inline QChar KCharColumn::substituteChar()  const { return SubstituteChar; }
inline QChar KCharColumn::undefinedChar()   const { return UndefinedChar; }

inline bool KCharColumn::setSubstituteChar( QChar SC )
{
  if( SubstituteChar == SC )
    return false;
  SubstituteChar = SC;
  return true;
}

inline bool KCharColumn::setUndefinedChar( QChar UC )
{
  if( UndefinedChar == UC )
    return false;
  UndefinedChar = UC;
  return true;
}

inline bool KCharColumn::setShowUnprintable( bool SU )
{
  if( ShowUnprintable == SU )
    return false;
  ShowUnprintable = SU;
  return true;
}

}

#endif
