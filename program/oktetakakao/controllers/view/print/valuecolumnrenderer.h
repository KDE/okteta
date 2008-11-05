/*
    This file is part of the Okteta Kakao module, part of the KDE project.

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

#ifndef VALUECOLUMNRENDERER_H
#define VALUECOLUMNRENDERER_H

// lib
#include "bytecolumnrenderer.h"
// Okteta core
#include <valuecodec.h>


namespace KHEPrint
{

/** buffer column which displays the numerical values of the bytes
  *
  *@author Friedrich W. H. Kossebau
  */
class ValueColumnRenderer : public ByteColumnRenderer
{
  public:
    ValueColumnRenderer( AbstractColumnFrameRenderer *columnFrameRenderer,
                         const KHECore::AbstractByteArrayModel *byteArrayModel,
                         const KHE::Section &renderIndizes,
                         const KHEUI::ByteArrayTableLayout *layout );
    virtual ~ValueColumnRenderer();

  public: // modification access
    /**
      * returns true if there was a change
      */
    bool setValueCoding( KHECore::ValueCoding C );
    /** sets the spacing in the middle of a binary byte in the value column
      * @param BinaryGapW spacing in the middle of a binary in pixels
      * returns true if there was a change
      */
    bool setBinaryGapWidth( KPixelX BGW );


  public: // value access
    KPixelX binaryGapWidth()               const;
    KHECore::ValueCoding valueCoding()              const;
    const KHECore::ValueCodec *byteCodec() const;


  protected: // ByteColumnRenderer API
    virtual void drawByte( QPainter *painter, char byte, KHECore::KChar byteChar, const QColor &color ) const;
    virtual void recalcByteWidth();

  protected:
    void drawCode( QPainter *painter, const QString &code, const QColor &color ) const;

  protected: // set data
    /** */
    KHECore::ValueCoding mCoding;
    /** */
    KHECore::ValueCodec *mByteCodec;
    /** */
    KPixelX mBinaryGapWidth;

  protected: // buffered data
    /** buffer to hold the formatted valueCoding */
    mutable QString mCodedByte;
    /** calculated: Offset in pixels of the second half of the binary */
    KPixelX mBinaryHalfOffset;
};


inline KPixelX ValueColumnRenderer::binaryGapWidth()        const { return mBinaryGapWidth; }
inline KHECore::ValueCoding ValueColumnRenderer::valueCoding()                const { return mCoding; }
inline const KHECore::ValueCodec *ValueColumnRenderer::byteCodec()   const { return mByteCodec; }

}

#endif
