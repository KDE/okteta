/***************************************************************************
                          valuecolumnrenderer.h  -  description
                             -------------------
    begin                : Mit Sep 3 2003
    copyright            : 2003 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License version 2 as published by the Free Software Foundation.       *
 *                                                                         *
 ***************************************************************************/


#ifndef VALUECOLUMNRENDERER_H
#define VALUECOLUMNRENDERER_H

// lib
#include "bytecolumnrenderer.h"
// Okteta core
#include <valuecodec.h>


/** buffer column which displays the numerical values of the bytes
  *
  *@author Friedrich W. H. Kossebau
  */
class ValueColumnRenderer : public ByteColumnRenderer
{
  public:
    ValueColumnRenderer( AbstractColumnFrameRenderer *columnFrameRenderer,
                         const KHECore::KAbstractByteArrayModel *byteArrayModel,
                         const KHEUI::KDataLayout *layout );
    virtual ~ValueColumnRenderer();

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


  protected: // ByteColumnRenderer API
    virtual void drawByte( QPainter *painter, char byte, KHECore::KChar byteChar, const QColor &color ) const;
    virtual void recalcByteWidth();

  protected:
    void drawCode( QPainter *painter, const QString &code, const QColor &color ) const;

  protected: // set data
    /** */
    KHECore::KCoding mCoding;
    /** */
    KHECore::ValueCodec *mByteCodec;
    /** */
    KPixelX mBinaryGapWidth;

  protected: // buffered data
    /** buffer to hold the formatted coding */
    mutable QString mCodedByte;
    /** calculated: Offset in pixels of the second half of the binary */
    KPixelX mBinaryHalfOffset;
};


inline KPixelX ValueColumnRenderer::binaryGapWidth()        const { return mBinaryGapWidth; }
inline KHECore::KCoding ValueColumnRenderer::coding()                const { return mCoding; }
inline const KHECore::ValueCodec *ValueColumnRenderer::byteCodec()   const { return mByteCodec; }

#endif
