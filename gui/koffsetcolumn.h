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

#ifndef KHE_UI_KOFFSETCOLUMN_H
#define KHE_UI_KOFFSETCOLUMN_H

// lib
#include "koffsetformat.h"
#include "kcolumn.h"


namespace KHEUI
{

/**
  *@author Friedrich W. H. Kossebau
  */

class KOffsetColumn : public KColumn
{
  public:
    KOffsetColumn( KColumnsView *V, int FLO, int D, KOffsetFormat::KFormat F );
    virtual ~KOffsetColumn();

  public:  // KColumn API
    virtual void paintFirstLine( QPainter *P, const KPixelXs &Xs, int FirstLine );
    virtual void paintNextLine( QPainter *P );
    virtual void paintEmptyColumn( QPainter *P, const KPixelXs &Xs, const KPixelYs &Ys );


  public:
    void setFirstLineOffset( int FLO );
    void setDelta( int D );

    void setFormat( KOffsetFormat::KFormat F );
    /** sets width of digits and recalculates depend sizes  */
    void setDigitWidth( KPixelX DW );
    /** */
    void setMetrics( KPixelX DW, KPixelY DBL );

  public: // read access
    int delta() const;
    int firstLineOffset() const;
    int codingWidth() const;
    KOffsetFormat::print printFunction() const;


  protected:
    /** recalculates all x values */
    void recalcX();
    /** paints full line */
    void paintLine( QPainter *P, int Line );


  protected: // user settings
    /** starting offset of the first line
      * if different from StartOffset results in leading space
      */
    int FirstLineOffset;
    /** offset delta per line */
    int Delta;

  protected: // pixel related
    /** size of the line margin */
    int Margin;
    /** */
    KPixelX DigitWidth;
    /** */
    KPixelY DigitBaseLine;

  protected: // general layout
    KOffsetFormat::KFormat Format;

    int CodingWidth;
    KOffsetFormat::print PrintFunction;

    /** buffer to hold the formatted coding */
    mutable char CodedOffset[KOffsetFormat::MaxFormatWidth+1];

  protected: // firstnext trips related
    /** */
    int PaintLine;
};


inline int KOffsetColumn::firstLineOffset()  const  { return FirstLineOffset; }
inline void KOffsetColumn::setFirstLineOffset( int FLO ) { FirstLineOffset = FLO; }
inline int KOffsetColumn::delta() const { return Delta; }
inline void KOffsetColumn::setDelta( int D )  { Delta = D; }

inline int KOffsetColumn::codingWidth()                       const { return CodingWidth; }
inline KOffsetFormat::print KOffsetColumn::printFunction()    const { return PrintFunction; }

}

#endif
