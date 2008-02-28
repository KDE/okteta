/*
    This file is part of the Okteta Gui library, part of the KDE project.

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
