/***************************************************************************
                          kcharcolumn.h  -  description
                             -------------------
    begin                : Mit Sep 3 2003
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


#ifndef KHE_UI_KCHARCOLUMN_H
#define KHE_UI_KCHARCOLUMN_H


// lib
#include "kdatacolumn.h"
// Qt
#include <QtCore/QString>

class QPainter;
class QColor;


namespace KHEUI
{

/** buffer column that interprets the bytes as chars
  *
  *@author Friedrich W. H. Kossebau
  */
class KCharColumn : public KDataColumn
{
  public:
    KCharColumn( KColumnsView *CV, KHECore::KAbstractByteArrayModel *B, KDataLayout *L, KDataRanges *R );
    virtual ~KCharColumn();


  public: // modification access
    /** sets whether "unprintable" chars (>32) should be displayed in the char column
      * with their corresponding character.
      * @param SU
      * returns true if there was a change
      */
    bool setShowsNonprinting( bool SN = true );
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
    bool showsNonprinting() const;
    /** returns the actually used substitute character for "unprintable" chars, default is '.' */
    QChar substituteChar() const;
    /** returns the actually used undefined character for "undefined" chars, default is '?' */
    QChar undefinedChar() const;


  protected: // KDataColumn API
    virtual void drawByte( QPainter *P, char Byte, KHECore::KChar B, const QColor &Color ) const;

  protected:
    /** */
    bool ShowsNonprinting;
    /** */
    QChar SubstituteChar;
    /** */
    QChar UndefinedChar;
};


inline bool KCharColumn::showsNonprinting()  const { return ShowsNonprinting; }
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

inline bool KCharColumn::setShowsNonprinting( bool SN )
{
  if( ShowsNonprinting == SN )
    return false;
  ShowsNonprinting = SN;
  return true;
}

}

#endif
