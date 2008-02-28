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

#ifndef KHE_UI_KVALUECOLUMN_H
#define KHE_UI_KVALUECOLUMN_H

// lib
#include "kdatacolumn.h"
// Okteta core
#include <valuecodec.h>


namespace KHEUI
{

class KDataRanges;


/** buffer column which displays the numerical values of the bytes
  *
  *@author Friedrich W. H. Kossebau
  */
class KValueColumn : public KDataColumn
{
  public:
    KValueColumn( KColumnsView *CV, KHECore::KAbstractByteArrayModel *B, KDataLayout *L, KDataRanges *R );
    virtual ~KValueColumn();

  public:
    void paintEditedByte( QPainter *P, char Byte, const QString &EditBuffer );

  public: // modification access
    /**
      * returns true if there was a change
      */
    bool setCoding( KHECore::KCoding C );
    /** sets the spacing in the middle of a binary byte in the value column
      * @param BinaryGapW spacing in the middle of a binary in pixels
      * returns true if there was a change
      */
    bool setBinaryGapWidth( KPixelX BGW );


  public: // value access
    KPixelX binaryGapWidth()               const;
    KHECore::KCoding coding()              const;
    const KHECore::ValueCodec *byteCodec() const;


  protected: // KDataColumn API
    virtual void drawByte( QPainter *P, char Byte, KHECore::KChar B, const QColor &Color ) const;
    virtual void recalcByteWidth();

  protected:
    void drawCode( QPainter *P, const QString &Code, const QColor &Color ) const;

  protected: // set data
    /** */
    KHECore::KCoding Coding;
    /** */
    KHECore::ValueCodec *ByteCodec;
    /** */
    KPixelX BinaryGapWidth;

  protected: // buffered data
    /** buffer to hold the formatted coding */
    mutable QString CodedByte;
    /** calculated: Offset in pixels of the second half of the binary */
    KPixelX BinaryHalfOffset;
};


inline KPixelX KValueColumn::binaryGapWidth()        const { return BinaryGapWidth; }
inline KHECore::KCoding KValueColumn::coding()                const { return Coding; }
inline const KHECore::ValueCodec *KValueColumn::byteCodec()   const { return ByteCodec; }

}

#endif
