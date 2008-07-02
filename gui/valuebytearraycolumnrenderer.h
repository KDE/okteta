/*
    This file is part of the Okteta Gui library, part of the KDE project.

    Copyright 2003,2008 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef KHE_UI_VALUEBYTEARRAYCOLUMNRENDERER_H
#define KHE_UI_VALUEBYTEARRAYCOLUMNRENDERER_H

// lib
#include "abstractbytearraycolumnrenderer.h"
// Okteta core
#include <valuecodec.h>


namespace KHEUI
{

class KDataRanges;


/** buffer column which displays the numerical values of the bytes
  *
  *@author Friedrich W. H. Kossebau
  */
class ValueByteArrayColumnRenderer : public AbstractByteArrayColumnRenderer
{
  public:
    ValueByteArrayColumnRenderer( ColumnsView *columnsView,
        KHECore::KAbstractByteArrayModel *byteArrayModel, ByteArrayTableLayout *layout, KDataRanges *ranges );
    virtual ~ValueByteArrayColumnRenderer();

  public:
    void renderEditedByte( QPainter *painter, char byte, const QString &editBuffer );

  public: // modification access
    /**
      * returns true if there was a change
      */
    bool setValueCoding( KHECore::KCoding valueCoding );
    /** sets the spacing in the middle of a binary byte in the value column
      * @param BinaryGapW spacing in the middle of a binary in pixels
      * returns true if there was a change
      */
    bool setBinaryGapWidth( KPixelX binaryGapWidth );


  public: // value access
    KPixelX binaryGapWidth() const;
    KHECore::KCoding valueCoding() const;
    const KHECore::ValueCodec *valueCodec() const;


  protected: // AbstractByteArrayColumnRenderer API
    virtual void renderByteText( QPainter *painter, char Byte, KHECore::KChar byteChar, const QColor &color ) const;
    virtual void recalcByteWidth();

  protected:
    void renderCode( QPainter *painter, const QString &code, const QColor &color ) const;

  protected: // settings
    /** */
    KHECore::KCoding mValueCoding;
    /** */
    KHECore::ValueCodec *mValueCodec;
    /** */
    KPixelX mBinaryGapWidth;

  protected: // buffered data
    /** buffer to hold the formatted valueCoding */
    mutable QString mDecodedByteText;
    /** calculated: Offset in pixels of the second half of the binary */
    KPixelX mBinaryHalfOffset;
};


inline KPixelX ValueByteArrayColumnRenderer::binaryGapWidth()                 const { return mBinaryGapWidth; }
inline KHECore::KCoding ValueByteArrayColumnRenderer::valueCoding()           const { return mValueCoding; }
inline const KHECore::ValueCodec *ValueByteArrayColumnRenderer::valueCodec()  const { return mValueCodec; }

}

#endif
